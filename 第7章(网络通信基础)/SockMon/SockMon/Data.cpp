#include "StdAfx.h"
#include "Data.h"



CData::CData(void)
{
	this->nCount = 0 ;
	this->pSockTree = NULL ;

	this->hFile = this->hMapFile = NULL ;
	this->lpData = NULL ;

	this->ProcessTable.clear () ;
	this->SetupShareMemeory () ;
}

CData::~CData(void)
{
	this->ReleaseShareMemory () ;

	this->ProcessTable.clear () ;
}

VOID CData::SetTreeCtrl ( CTreeCtrl* pTree )
{
	this->pSockTree = pTree ;
	this->hRootTree = TVI_ROOT ;
}

VOID CData::ResetSockData ()
{
	// 记数清0
	((DWORD*)this->lpData)[0] = 0 ;

	this->nCount = 0 ;
	for ( UINT i = 0; i < this->ProcessTable.size (); i++ )
	{
		if ( !this->ProcessTable[i].SocketTable.empty() )
		{
			this->ProcessTable[i].SocketTable.clear () ;
			this->ProcessTable[i].nSocketCount = 0 ;
			this->pSockTree->DeleteItem ( this->ProcessTable[i].hTreeItem ) ;
		}
	}

	//this->ProcessTable.clear () ;
	this->pSockTree->DeleteAllItems () ;
	if ( this->pSockTree )
	{
		WCHAR szTempBuf[MAX_DATA_SIZE] = {0} ;
		for ( UINT i = 0; i < this->ProcessTable.size(); i++ )
		{
			wsprintf ( szTempBuf, L"[%04d]%s", \
				this->ProcessTable[i].ProcessInfo.dwProcessId, this->ProcessTable[i].ProcessInfo.lpPath ) ;
			this->ProcessTable[i].hTreeItem = this->pSockTree->InsertItem ( szTempBuf, this->hRootTree ) ;
		}
	}
}

VOID CData::UpdateInfo ()
{
	if ( this->lpData == NULL )
		return ;
	
	UINT nNewCount = *((DWORD*)this->lpData) ;
	LPPROCESS_INFO	lpProcessInfo = NULL ;
	LPSOCKET_INFO	lpSocketInfo = NULL ;
	LPOPERATE_INFO	lpOperateInfo = NULL ;
	for ( UINT i = this->nCount+1; i <= nNewCount; i++ )
	{
		UINT	nDataOffset = i * MAX_DATA_SIZE ;
		LPBYTE	lpInfo = &this->lpData[nDataOffset] ;
		DWORD	dwDataType = *((DWORD*)lpInfo) ;
		switch ( dwDataType )
		{
		case 1:
			lpProcessInfo = (LPPROCESS_INFO)&lpInfo[4] ;
			this->AddProcessItem ( lpProcessInfo ) ;
			break ;
		case 2:
			lpSocketInfo = (LPSOCKET_INFO)&lpInfo[4] ;
			this->AddSocketItem ( lpSocketInfo ) ;
			break ;
		case 3:
			lpOperateInfo = (LPOPERATE_INFO)&lpInfo[4] ;
			this->AddOperateItem ( lpOperateInfo ) ;
			break ;
		}	
		
		this->nCount = i ;
	}

}

VOID CData::AddProcessItem( LPPROCESS_INFO lpProcessInfo )
{
	UINT nTestIndex = this->GetProcessIndexById ( lpProcessInfo->dwProcessId ) ;
	if ( nTestIndex != 0xFFFFFFFF )
		return ;

	WCHAR szTempBuf[MAX_DATA_SIZE] = {0} ;
	UINT nIndex = this->ProcessTable.size () ;

	PROCESS_ITEM ProcessItem = {0} ;
	ProcessItem.nProcessIndex = nIndex ;
	ProcessItem.nSocketCount = 0 ;
	ProcessItem.SocketTable.clear () ;

	memcpy ( &ProcessItem.ProcessInfo, lpProcessInfo, sizeof(PROCESS_INFO) ) ;
	this->ProcessTable.push_back ( ProcessItem ) ;

	if ( this->pSockTree )
	{
		wsprintf ( szTempBuf, L"[%d][%04d]%s", this->nCount, lpProcessInfo->dwProcessId, lpProcessInfo->lpPath ) ;
		this->ProcessTable[nIndex].hTreeItem = this->pSockTree->InsertItem ( szTempBuf, this->hRootTree ) ;
	}
}

VOID CData::AddSocketItem( LPSOCKET_INFO lpSocketInfo )
{
	WCHAR szTempBuf[MAX_DATA_SIZE] = {0} ;
	UINT nIndex = this->GetProcessIndexById ( lpSocketInfo->dwProcessId ) ;
	if ( nIndex == 0xFFFFFFFF )
		return ;
	
	UINT nTestIndex = this->GetSocketIndexByHandle ( nIndex, lpSocketInfo->s ) ;
	if ( nTestIndex != 0xFFFFFFFF )
		return ;

	UINT nSockIndex = this->ProcessTable[nIndex].nSocketCount ;

	SOCKET_ITEM	SocketItem = {0} ;
	SocketItem.nSocketIndex = nSockIndex ;
	SocketItem.nOperateCount = 0 ;
	SocketItem.OperateTable.clear () ;

	memcpy ( &SocketItem.SocketInfo, lpSocketInfo, sizeof(SOCKET_INFO) ) ;
	this->ProcessTable[nIndex].SocketTable.push_back ( SocketItem ) ;
	this->ProcessTable[nIndex].nSocketCount ++ ;

	if ( this->pSockTree )
	{
		switch ( lpSocketInfo->protocol )
		{
		case IPPROTO_IP:		wcscpy ( szTempBuf, L"[IP]" ) ;		break ;//0
		case IPPROTO_ICMP:		wcscpy ( szTempBuf, L"[ICMP]" ) ;	break ;//1
		case IPPROTO_IGMP:		wcscpy ( szTempBuf, L"[IGMP]" ) ;	break ;//2
		case IPPROTO_GGP:		wcscpy ( szTempBuf, L"[GGP]" ) ;	break ;//3
		case IPPROTO_IPV4:		wcscpy ( szTempBuf, L"[IPV4]" ) ;	break ;//4
		case IPPROTO_TCP:		wcscpy ( szTempBuf, L"[TCP]" ) ;	break ;//6
		case IPPROTO_PUP:		wcscpy ( szTempBuf, L"[PUP]" ) ;	break ;//12
		case IPPROTO_UDP:		wcscpy ( szTempBuf, L"[UDP]" ) ;	break ;//17
		case IPPROTO_IDP:		wcscpy ( szTempBuf, L"[IDP]" ) ;	break ;//22
		case IPPROTO_IPV6:		wcscpy ( szTempBuf, L"[IPV6]" ) ;	break ;//41
		case IPPROTO_ROUTING:	wcscpy ( szTempBuf, L"[ROUTING]" ) ;break ;//43
		case IPPROTO_FRAGMENT:	wcscpy ( szTempBuf, L"[FRAGMENT]" ) ;break ;//44
		case IPPROTO_ESP:		wcscpy ( szTempBuf, L"[ESP]" ) ;	break ;//50
		case IPPROTO_AH:		wcscpy ( szTempBuf, L"[AH]" ) ;		break ;//51
		case IPPROTO_ICMPV6:	wcscpy ( szTempBuf, L"[ICMPV6]" ) ;	break ;//58
		case IPPROTO_NONE:		wcscpy ( szTempBuf, L"[NONE]" ) ;	break ;//59
		case IPPROTO_DSTOPTS:	wcscpy ( szTempBuf, L"[DSTOPTS]" ) ;break ;//60
		case IPPROTO_ND:		wcscpy ( szTempBuf, L"[ND]" ) ;		break ;//77
		case IPPROTO_ICLFXBM:	wcscpy ( szTempBuf, L"[ICLFXBM]" ) ;break ;//78
		case IPPROTO_RAW:		wcscpy ( szTempBuf, L"[RAW]" ) ;	break ;//255
		}
		int len = wcslen(szTempBuf) ;
		wsprintf ( &szTempBuf[len], L"[%d]Socket=0x%08X  %02d:%02d:%02d",this->nCount,\
			lpSocketInfo->s, lpSocketInfo->CreateTime.wHour, \
			lpSocketInfo->CreateTime.wMonth, lpSocketInfo->CreateTime.wSecond ) ;

		this->ProcessTable[nIndex].SocketTable[nSockIndex].hTreeItem = \
			this->pSockTree->InsertItem ( szTempBuf, this->ProcessTable[nIndex].hTreeItem ) ;
	}
}

VOID CData::AddOperateItem( LPOPERATE_INFO lpOperateInfo )
{
	UINT nProcessIndex = this->GetProcessIndexById ( lpOperateInfo->dwProcessId ) ;
	if ( nProcessIndex == 0xFFFFFFFF )
		return ;

	UINT nSocketIndex = this->GetSocketIndexByHandle ( nProcessIndex, lpOperateInfo->s ) ;
	if ( nSocketIndex == 0xFFFFFFFF )
		return ;

	OPERATE_ITEM OperateItem = {0} ;
	OperateItem.nOperateIndex = this->ProcessTable[nProcessIndex].SocketTable[nSocketIndex].nOperateCount ;
	
	memcpy ( &OperateItem.OperateInfo, lpOperateInfo, sizeof(OPERATE_INFO) ) ;
	this->ProcessTable[nProcessIndex].SocketTable[nSocketIndex].OperateTable.push_back ( OperateItem ) ;
	this->ProcessTable[nProcessIndex].SocketTable[nSocketIndex].nOperateCount ++ ;


	WCHAR szTempBuf[MAX_DATA_SIZE] = {0} ;
	in_addr* p1 = &lpOperateInfo->LocalAddr.sin_addr ;
	in_addr* p2 = &lpOperateInfo->RemoteAddr.sin_addr ;
	wsprintf ( szTempBuf, L"[%d][%s] %Local=%d:%d:%d:%d:%d Remote=%d:%d:%d:%d:%d DataSize=%4d ErrorCode=%d", this->nCount,lpOperateInfo->szOperate,\
		p1->S_un.S_un_b.s_b1,p1->S_un.S_un_b.s_b2,p1->S_un.S_un_b.s_b3,p1->S_un.S_un_b.s_b4,ntohs(lpOperateInfo->LocalAddr.sin_port), \
		p2->S_un.S_un_b.s_b1,p2->S_un.S_un_b.s_b2,p2->S_un.S_un_b.s_b3,p2->S_un.S_un_b.s_b4,ntohs(lpOperateInfo->RemoteAddr.sin_port), \
		lpOperateInfo->dwDataSize, lpOperateInfo->nErrorCode ) ;

	this->pSockTree->InsertItem ( szTempBuf, this->ProcessTable[nProcessIndex].SocketTable[nSocketIndex].hTreeItem ) ;
}

UINT CData::GetProcessIndexById( UINT nProcessId )
{
	for ( UINT i = 0; i < this->ProcessTable.size(); i++ )
	{
		if ( this->ProcessTable[i].ProcessInfo.dwProcessId == nProcessId )
			return i ;
	}
	return 0xFFFFFFFF ;
}

UINT CData::GetSocketIndexByHandle( UINT nProcessIndex, SOCKET s )
{
	for ( UINT i = 0; i < this->ProcessTable[nProcessIndex].SocketTable.size(); i++ )
	{
		if ( this->ProcessTable[nProcessIndex].SocketTable[i].SocketInfo.s == s )
			return i ;
	}

	return 0xFFFFFFFF ;
}

BOOL CData::SetupShareMemeory ()
{
	this->hFile = CreateFile ( L"SockInfo.data", FILE_GENERIC_READ|FILE_GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL ) ;
	if ( this->hFile == INVALID_HANDLE_VALUE )
		return FALSE ;

	this->hMapFile = CreateFileMapping ( this->hFile, NULL, \
		PAGE_READWRITE | SEC_COMMIT, 0, MAX_SHARE_MEMORY_SIZE, SHARE_FILE_NAME ) ;
	if ( this->hMapFile == NULL )
		return FALSE ;

	this->lpData = (LPBYTE)MapViewOfFile ( this->hMapFile, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0 ) ;
	if ( this->lpData == NULL )
		return FALSE ;

	// 记数清0
	((DWORD*)this->lpData)[0] = 0 ;
	return TRUE ;
}

VOID CData::ReleaseShareMemory ()
{
	if ( this->lpData )
		UnmapViewOfFile ( this->lpData ) ;

	if ( this->hMapFile )
		CloseHandle ( this->hMapFile ) ;

	if ( this->hFile )
		CloseHandle ( this->hFile ) ;
}

BOOL CData::IsFull()
{
	return ( ((DWORD*)this->lpData)[0] == MAX_ITEM_NUM-1 ) ? TRUE : FALSE ;
}

VOID CData::FlushSockData()
{
	this->nCount = 0 ;
	for ( UINT i = 0; i < this->ProcessTable.size (); i++ )
	{
		if ( !this->ProcessTable[i].SocketTable.empty() )
		{
			this->ProcessTable[i].SocketTable.clear () ;
			this->ProcessTable[i].nSocketCount = 0 ;
		}
	}
	this->pSockTree->DeleteAllItems () ;
	if ( this->pSockTree )
	{
		WCHAR szTempBuf[MAX_DATA_SIZE] = {0} ;
		for ( UINT i = 0; i < this->ProcessTable.size(); i++ )
		{
			wsprintf ( szTempBuf, L"[%04d]%s", \
				this->ProcessTable[i].ProcessInfo.dwProcessId, this->ProcessTable[i].ProcessInfo.lpPath ) ;
			this->ProcessTable[i].hTreeItem = this->pSockTree->InsertItem ( szTempBuf, this->hRootTree ) ;
		}
	}

	this->UpdateInfo () ;

	return ;
}