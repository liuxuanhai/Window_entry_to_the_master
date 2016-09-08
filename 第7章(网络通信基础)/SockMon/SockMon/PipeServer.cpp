#include "StdAfx.h"
#include "PipeServer.h"

CPipeServer::CPipeServer(void)
{
	this->pSockTree = NULL ;
}

CPipeServer::~CPipeServer(void)
{
}

UINT ServerThread ( LPVOID lpParameter )
{
	CPipeServer* pServer = (CPipeServer*)lpParameter ;

	UINT	nProcessIndex, nSocketIndex, nOperateIndex ;
	DWORD	cbTrans = 0, dwCompletionKey, nReadByte = 0, nWriteByte = 0 ;	
	char	szBuf[MAX_DATA_SIZE] = {0} ;

	BOOL bRet ;
	LPOVERLAPPED	lpov ;
	LPPROCESS_INFO	lpProcessInfo = NULL ;
	LPSOCKET_INFO	lpSocketInfo = NULL ;
	LPOPERATE_INFO	lpOperateInfo = NULL ;

	WCHAR	szTempBuf[MAX_BUF_SIZE] = {0} ;

	while ( true )
	{
		GetQueuedCompletionStatus ( pServer->hCompletionPort, &cbTrans, &dwCompletionKey, &lpov, INFINITE ) ;
		if ( dwCompletionKey == -1 )
			break ;

		// 从管道中读取客户端的请求信息
		ReadFile ( (pServer->PipeInst[dwCompletionKey]).hPipe, szBuf, MAX_DATA_SIZE, &nReadByte, NULL ) ;

		DWORD	dwDataType = *((DWORD*)szBuf) ;
		switch ( dwDataType )
		{
		case 1:
			lpProcessInfo = (LPPROCESS_INFO)&szBuf[4] ;
			SockData.AddProcessItem ( lpProcessInfo, nProcessIndex ) ;

			if ( pServer->pSockTree )
			{
				wsprintf ( szTempBuf, L"[%d]%s", lpProcessInfo->dwProcessId, lpProcessInfo->lpPath ) ;
				pServer->pSockTree->InsertItem ( szTempBuf, pServer->hTreeRoot ) ;
			}
			break ;
		case 2:
			lpSocketInfo = (LPSOCKET_INFO)&szBuf[4] ;
			SockData.AddSocketItem ( lpSocketInfo, nProcessIndex, nSocketIndex ) ;

			if ( pServer->pSockTree )
			{
				switch ( lpSocketInfo->protocol )
				{
				case IPPROTO_UDP:wcscpy ( szTempBuf, L"[UDP]" ) ;break ;
				case IPPROTO_TCP:wcscpy ( szTempBuf, L"[TCP]" ) ;break ;
				case IPPROTO_RAW:wcscpy ( szTempBuf, L"[RAW]" ) ;break ;
				}
				wsprintf ( &szTempBuf[5], L"Socket=0x%08X (%04d-%02d-%02d %02d:%02d:%02d)",\
					lpSocketInfo->s, lpSocketInfo->CreateTime.wYear,lpSocketInfo->CreateTime.wMonth, \
					lpSocketInfo->CreateTime.wDay, lpSocketInfo->CreateTime.wHour, \
					lpSocketInfo->CreateTime.wMonth, lpSocketInfo->CreateTime.wSecond ) ;
				
				HTREEITEM hTreeItem = pServer->pSockTree->GetNextItem( pServer->hTreeRoot, TVGN_CHILD ) ;
				if ( hTreeItem )
				{
					while ( nProcessIndex > 0 )
					{
						hTreeItem = pServer->pSockTree->GetNextItem ( hTreeItem, TVGN_NEXT ) ;
						nProcessIndex-- ;
					}

					pServer->pSockTree->InsertItem ( szTempBuf, hTreeItem ) ;
				}
			}
			break ;
		case 3:
			lpOperateInfo = (LPOPERATE_INFO)&szBuf[4] ;
			SockData.AddOperateItem ( lpOperateInfo, nProcessIndex, nSocketIndex, nOperateIndex ) ;
			break ;
		}	

		// 把反馈信息写入管道
		WriteFile ( (pServer->PipeInst[dwCompletionKey]).hPipe, szBuf, 1, &nWriteByte, NULL ) ;

		bRet = DisconnectNamedPipe ( pServer->PipeInst[dwCompletionKey].hPipe ) ;
	}
	return 0 ;
}

BOOL CPipeServer::StartServer( CTreeCtrl* pTree )
{
	this->pSockTree = pTree ;
	if ( this->pSockTree )
	{
		this->hTreeRoot = this->pSockTree->InsertItem ( L"进程列表" ) ;
	}

	this->hCompletionPort = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, MAX_THREAD_NUM ) ;

	for ( UINT i = 0; i < MAX_PIPE_NUM; i++ )
	{
		PipeInst[i].hPipe =	CreateNamedPipe ( PIPE_NAME, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, \
			PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, MAX_PIPE_NUM, 0, 0, INFINITE, NULL ) ;
		if ( PipeInst[i].hPipe == INVALID_HANDLE_VALUE )
			return FALSE ;

		HANDLE hRet = CreateIoCompletionPort ( PipeInst[i].hPipe, hCompletionPort, i, MAX_THREAD_NUM ) ;
		if ( hRet == NULL )
		{
			CloseHandle ( hCompletionPort ) ;
			return FALSE;
		}

		PipeInst[i].ov.hEvent = CreateEvent ( NULL, FALSE, FALSE, NULL ) ;
		ConnectNamedPipe ( PipeInst[i].hPipe, &(PipeInst[i].ov) ) ;
	}

	for ( UINT i = 0; i < MAX_THREAD_NUM; i++ )
	{
		hThread[i] = AfxBeginThread ( ServerThread, this, THREAD_PRIORITY_NORMAL ) ;
		if ( hThread[i] == NULL )
			return FALSE ;
	}

	return TRUE ;
}

VOID CPipeServer::StopServer()
{
	for ( UINT i = 0; i < MAX_THREAD_NUM; i++ )
	{
		PostQueuedCompletionStatus ( this->hCompletionPort, 0, -1, NULL ) ;
		if ( hThread[i] )
			CloseHandle ( hThread[i] ) ;
	}

	for ( UINT i = 0; i < MAX_PIPE_NUM; i++ )
	{
		if ( this->PipeInst[i].hPipe )
		{
			DisconnectNamedPipe ( this->PipeInst[i].hPipe ) ;
			CloseHandle ( this->PipeInst[i].hPipe ) ;
		}
	}
}

BOOL CPipeServer::GetServerStatus()
{
	return this->bServerStatus ;
}