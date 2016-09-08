#include "StdAfx.h"
#include "Data.h"

extern VOID MY_OutputDebugStringA ( const char* szFormat,...) ;
extern VOID MY_OutputDebugStringW ( const wchar_t* szFormat,...) ;

#define PIPE_NAME		L"\\\\.\\Pipe\\SockDataInfo"
#define SHARE_FILE_NAME	L"SOCK_INFO_DATA"
#define NOTIFY_MESSAGE	(WM_USER+2048)

CData::CData(void)
{
	InitializeCriticalSection ( &this->cs ) ;
	this->SetupShareMemeory () ;
}

CData::~CData(void)
{
	this->ReleaseShareMemory () ;
}

VOID CData::AddProcessInfo( LPPROCESS_INFO lpProcessInfo )
{
	char szData[MAX_DATA_SIZE] = {0} ;
	*((DWORD*)szData) = 1 ;
	memcpy ( &szData[4], lpProcessInfo, sizeof(PROCESS_INFO) ) ;

	DWORD dwTotalSize = 4 + sizeof(PROCESS_INFO) ;
	this->SendDataToClient ( (LPBYTE)szData, dwTotalSize ) ;
}

VOID CData::AddSocketInfo( LPSOCKET_INFO lpSockInfo )
{
	char szData[MAX_DATA_SIZE] = {0} ;
	*((DWORD*)szData) = 2 ;
	memcpy ( &szData[4], lpSockInfo, sizeof(SOCKET_INFO) ) ;

	DWORD dwTotalSize = 4 + sizeof(SOCKET_INFO) ;
	this->SendDataToClient ( (LPBYTE)szData, dwTotalSize ) ;

}

VOID CData::AddOperateInfo( LPOPERATE_INFO lpOperateInfo )
{
	char szData[MAX_DATA_SIZE] = {0} ;
	*((DWORD*)szData) = 3 ;
	memcpy ( &szData[4], lpOperateInfo, sizeof(OPERATE_INFO) ) ;


	DWORD dwTotalSize = 4 + sizeof(OPERATE_INFO) ;
	if ( lpOperateInfo->dwDataSize > 0 && lpOperateInfo->lpData != NULL )
	{
		memcpy ( &szData[dwTotalSize], lpOperateInfo->lpData, lpOperateInfo->dwDataSize ) ;
		dwTotalSize += lpOperateInfo->dwThreadId ;
	}
	this->SendDataToClient ( (LPBYTE)szData, dwTotalSize ) ;
}

VOID CData::SendDataToClient( LPBYTE lpData, DWORD dwSize )
{
	if ( this->lpData == NULL && this->SetupShareMemeory()==FALSE )
		return ;

	UINT nIndex = this->GetIndex() ;
	if ( nIndex == 0xFFFFFFFF || ((DWORD*)this->lpData)[0] >= MAX_ITEM_NUM )
	{
		MY_OutputDebugStringA ( "数据空间不足！" ) ;
		return ;
	}

	if ( dwSize > MAX_DATA_SIZE )
		dwSize = MAX_DATA_SIZE ;

	UINT nDataOffset = MAX_DATA_SIZE * nIndex ;
	__try{
		memcpy ( &this->lpData[nDataOffset], lpData, dwSize ) ;
	}
	__except ( EXCEPTION_EXECUTE_HANDLER){
		this->hMapFile = NULL ;
		this->lpData = NULL ;
		MY_OutputDebugStringA ( "Write Data Error!(index=%d)", nIndex ) ;
	}

	this->SendNotifyMessage ( 0, 0 ) ;
}

UINT CData::GetIndex () 
{
	volatile LONG& nCount = *((LONG*)this->lpData) ;
	return InterlockedIncrement ( &nCount ) ;

	//UINT nIndex = 0xFFFFFFFF ;
	//EnterCriticalSection ( &this->cs ) ;
	//if ( *pCount < MAX_ITEM_NUM-1 )
	//	nIndex = (++(*pCount)) ;
	//LeaveCriticalSection ( &this->cs ) ;
	//return nIndex ;
}

BOOL CData::SetupShareMemeory ()
{
	WCHAR szPath[MAX_BUF_SIZE] = {0} ;
	GetModuleFileName ( GetModuleHandle(L"LSP.dll"), szPath, MAX_BUF_SIZE ) ;
	szPath[wcslen(szPath)-7] = 0 ;
	wcscat ( szPath, L"SockInfo.data" ) ;
	OutputDebugStringW ( szPath ) ;

	this->hFile = CreateFile ( szPath, FILE_GENERIC_READ|FILE_GENERIC_WRITE, \
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL ) ;
	if ( this->hFile == INVALID_HANDLE_VALUE )
		return FALSE ;

	this->hMapFile = CreateFileMapping ( this->hFile, NULL, \
		PAGE_READWRITE | SEC_COMMIT, 0, MAX_SHARE_MEMORY_SIZE, SHARE_FILE_NAME ) ;
	if ( this->hMapFile == NULL )
		return FALSE ;

	this->lpData = (LPBYTE)MapViewOfFile ( this->hMapFile, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0 ) ;
	if ( this->lpData == NULL )
		return FALSE ;

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

VOID CData::SendNotifyMessage( WPARAM wParam, LPARAM lParam )
{
	HWND hWnd = FindWindow ( NULL, L"[第7章]使用分层服务提供者（LSP）截取网络数据包" ) ;
	if ( hWnd )
	{
		PostMessage ( hWnd, NOTIFY_MESSAGE, wParam, lParam )  ;
	}
	else
	{
		MY_OutputDebugStringA ( "没有找到窗口!" ) ;
	}
}