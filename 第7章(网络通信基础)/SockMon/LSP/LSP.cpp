// LSP.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Data.h"

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CLSPApp

BEGIN_MESSAGE_MAP(CLSPApp, CWinApp)
END_MESSAGE_MAP()


// CLSPApp 构造

CLSPApp::CLSPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLSPApp 对象

//////////////////////////////////////////////////////////////////////////////////////
CLSPApp theApp;
char	szLogBuf[MAX_BUF_SIZE] = {0} ;
WCHAR	szLogBufW[MAX_BUF_SIZE] = {0} ;

CData			data ;
CProvider		Provider ;
WSPPROC_TABLE	ProcTable ;	// 下层函数列表


// CLSPApp 初始化

BOOL CLSPApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


BOOL CLSPApp::ExitInstance ()
{
	return CWinApp::ExitInstance () ;
}

VOID MY_OutputDebugStringA ( const char* szFormat,...)
{
#ifdef LOG_OUT
	va_list argList ;
	va_start ( argList, szFormat ) ;
	vsprintf ( szLogBuf, szFormat, argList ) ;
	va_end ( argList ) ;

	OutputDebugStringA ( szLogBuf ) ;
#endif
}

VOID MY_OutputDebugStringW ( const wchar_t* szFormat,...)
{
#ifdef LOG_OUT

	va_list argList ;
	va_start ( argList, szFormat ) ;
	vswprintf ( szLogBufW, sizeof(szLogBufW), szFormat, argList ) ;
	va_end ( argList ) ;

	OutputDebugStringW ( szLogBufW ) ;
#endif
}

SOCKET WSPAPI WSPSocket( int af, int type,int protocol,\
	LPWSAPROTOCOL_INFO lpProtocolInfo,GROUP g,DWORD dwFlags,LPINT lpErrno )
{
	SOCKET s = ProcTable.lpWSPSocket ( af, type, protocol, lpProtocolInfo, g, dwFlags, lpErrno ) ;

	MY_OutputDebugStringA ( szLogBuf, "[WSPSocket]socket=0x%08X", s ) ;

	SOCKET_INFO SockInfo = {0} ;
	SockInfo.s = s ;
	SockInfo.protocol = protocol ;
	SockInfo.dwProcessId = GetCurrentProcessId() ;
	GetLocalTime ( &SockInfo.CreateTime ) ; 
	data.AddSocketInfo ( &SockInfo ) ;
	return s ;
}


int WSPAPI WSPSend(
				   SOCKET s,
				   LPWSABUF lpBuffers,
				   DWORD dwBufferCount,
				   LPDWORD lpNumberOfBytesSent,
				   DWORD dwFlags,
				   LPWSAOVERLAPPED lpOverlapped,
				   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
				   LPWSATHREADID lpThreadId,
				   LPINT lpErrno
				   )
{
	MY_OutputDebugStringA ( szLogBuf, "[WSPSend]socket=0x%08X", s ) ;

	int ret= ProcTable.lpWSPSend ( s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, \
		dwFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno ) ;


	OPERATE_INFO operate = {0} ;
	operate.s = s ;
	operate.dwProcessId = GetCurrentProcessId () ;
	operate.dwThreadId = GetCurrentThreadId() ;
	operate.nErrorCode = *lpErrno ;
	operate.dwDataSize = *lpNumberOfBytesSent ;
	operate.lpData	   = NULL ;	
	wcscpy ( operate.szOperate, L"WSPSend" ) ;

	int namelen = sizeof(sockaddr) ;
	getsockname ( s, (sockaddr*)&operate.LocalAddr, &namelen ) ;
	getpeername ( s, (sockaddr*)&operate.RemoteAddr, &namelen ) ;
	data.AddOperateInfo ( &operate ) ;

	return ret ;
}

int WSPAPI WSPSendTo(
					 SOCKET s,
					 LPWSABUF lpBuffers,
					 DWORD dwBufferCount,
					 LPDWORD lpNumberOfBytesSent,
					 DWORD dwFlags,
					 const struct sockaddr* lpTo,
					 int iTolen,
					 LPWSAOVERLAPPED lpOverlapped,
					 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
					 LPWSATHREADID lpThreadId,
					 LPINT lpErrno
					 )
{
	MY_OutputDebugStringA ( szLogBuf, "[WSPSendTo]socket=0x%08X", s ) ;

	int ret = ProcTable.lpWSPSendTo ( s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, \
		dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno ) ;

	OPERATE_INFO operate = {0} ;
	operate.s = s ;
	operate.dwDataSize = *lpNumberOfBytesSent ;
	operate.dwProcessId = GetCurrentProcessId () ;
	operate.dwThreadId = GetCurrentThreadId() ;
	operate.nErrorCode = *lpErrno ;
	operate.lpData	   = NULL ;	
	wcscpy ( operate.szOperate, L"WSPSendTo" ) ;

	int namelen = sizeof(sockaddr) ;
	getsockname ( s, (sockaddr*)&operate.LocalAddr, &namelen ) ;
	getpeername ( s, (sockaddr*)&operate.RemoteAddr, &namelen ) ;
	data.AddOperateInfo ( &operate ) ;

	return ret ;
}

int WSPAPI WSPRecv(
				   SOCKET s,
				   LPWSABUF lpBuffers,
				   DWORD dwBufferCount,
				   LPDWORD lpNumberOfBytesRecvd,
				   LPDWORD lpFlags,
				   LPWSAOVERLAPPED lpOverlapped,
				   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
				   LPWSATHREADID lpThreadId,
				   LPINT lpErrno )
{
	MY_OutputDebugStringA ( szLogBuf, "[WSPRecv]socket=0x%08X", s ) ;

	int ret = ProcTable.lpWSPRecv ( s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, \
		lpFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno ) ;

	OPERATE_INFO operate = {0} ;
	operate.s = s ;
	operate.dwDataSize = *lpNumberOfBytesRecvd ;
	operate.dwProcessId = GetCurrentProcessId () ;
	operate.dwThreadId = GetCurrentThreadId() ;
	operate.nErrorCode = *lpErrno ;
	operate.lpData	   = NULL ;	
	wcscpy ( operate.szOperate, L"WSPRecv" ) ;

	int namelen = sizeof(sockaddr) ;
	getsockname ( s, (sockaddr*)&operate.LocalAddr, &namelen ) ;
	getpeername ( s, (sockaddr*)&operate.RemoteAddr, &namelen ) ;
	data.AddOperateInfo ( &operate ) ;

	return ret ;
}

int WSPAPI WSPRecvFrom(
					   SOCKET s,
					   LPWSABUF lpBuffers,
					   DWORD dwBufferCount,
					   LPDWORD lpNumberOfBytesRecvd,
					   LPDWORD lpFlags,
struct sockaddr* lpFrom,
	LPINT lpFromlen,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
	LPWSATHREADID lpThreadId,
	LPINT lpErrno
	)
{
	MY_OutputDebugStringA ( szLogBuf, "[WSPRecvFrom]socket=0x%08X", s ) ;

	int ret = ProcTable.lpWSPRecvFrom ( s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, \
		lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno ) ;

	OPERATE_INFO operate = {0} ;
	operate.s = s ;
	operate.dwDataSize = *lpNumberOfBytesRecvd ;
	operate.dwProcessId = GetCurrentProcessId () ;
	operate.dwThreadId = GetCurrentThreadId() ;
	operate.nErrorCode = *lpErrno ;
	operate.lpData	   = NULL ;	
	wcscpy ( operate.szOperate, L"WSPRecvFrom" ) ;

	int namelen = sizeof(sockaddr) ;
	getsockname ( s, (sockaddr*)&operate.LocalAddr, &namelen ) ;
	getpeername ( s, (sockaddr*)&operate.RemoteAddr, &namelen ) ;
	data.AddOperateInfo ( &operate ) ;

	return ret ;
}


int WSPAPI WSPStartup( WORD wVersionRequested, LPWSPDATA lpWSPData, \
					  LPWSAPROTOCOL_INFO lpProtocolInfo, WSPUPCALLTABLE UpcallTable, LPWSPPROC_TABLE lpProcTable )
{
	if ( lpProtocolInfo->ProtocolChain.ChainLen <= 1 )
	{
		return WSAEPROVIDERFAILEDINIT ;
	}

	// 枚举服务提供者
	Provider.GetProvider() ;

	// 取得下层协议提供者入口信息
	DWORD dwNextEntryId = lpProtocolInfo->ProtocolChain.ChainEntries[1] ;
	LPWSAPROTOCOL_INFOW lpNextProviderInfo = Provider.SearchProviderByEntryId ( dwNextEntryId ) ;
	if ( lpNextProviderInfo == NULL )
	{
		return WSAEPROVIDERFAILEDINIT ;
	}

	int ret = 0 ;
	int nErrorCode = 0 ;
	int nLen = MAX_BUF_SIZE ;
	WCHAR szNextProviderPath[MAX_BUF_SIZE] = {0} ;

	// 取得下层提供者的DLL路径
	ret = ::WSCGetProviderPath(&lpNextProviderInfo->ProviderId, szNextProviderPath, &nLen, &nErrorCode ) ;
	if ( ret != SOCKET_ERROR )
	{
		DWORD dwRet = ::ExpandEnvironmentStrings ( szNextProviderPath, szNextProviderPath, MAX_BUF_SIZE ) ;
		if ( dwRet == 0 )
		{
			return WSAEPROVIDERFAILEDINIT ;
		}
	}
	else
	{
		return WSAEPROVIDERFAILEDINIT ;
	}

	// 加载下层提供者
	HMODULE hModule = ::LoadLibrary ( szNextProviderPath ) ;
	if ( hModule == NULL )
	{
		return WSAEPROVIDERFAILEDINIT ;
	}

	// 导入下层提供者的WSPStartup函数
	LPWSPSTARTUP pfnWSPStartup = (LPWSPSTARTUP)::GetProcAddress ( hModule, "WSPStartup" ) ;
	if ( pfnWSPStartup == NULL )
	{
		return WSAEPROVIDERFAILEDINIT ;
	}

	// 调用下层提供的WSPStartup函数
	// 如果下层为基础服务提供者，则传递其本身的协议信息
	// 如果下层为分层服务提供者（LSP），则传递当前协议信息
	LPWSAPROTOCOL_INFOW lpInfo = NULL ;
	if ( lpNextProviderInfo->ProtocolChain.ChainLen == BASE_PROTOCOL )
		lpInfo = lpNextProviderInfo ;
	else
		lpInfo = lpProtocolInfo ;
	ret = pfnWSPStartup ( wVersionRequested, lpWSPData, lpInfo, UpcallTable, lpProcTable ) ;
	if ( ret == ERROR_SUCCESS )
	{
		// 保存下层函数列表
		ProcTable = *lpProcTable ;

		// 修改传递给上层的函数列表，HOOK感兴趣的函数
		lpProcTable->lpWSPSend = WSPSend ;
		lpProcTable->lpWSPSendTo = WSPSendTo ;
		lpProcTable->lpWSPRecvFrom = WSPRecvFrom ;
		lpProcTable->lpWSPRecv = WSPRecv ;
		lpProcTable->lpWSPSocket = WSPSocket ;

		// 记录进程信息
		PROCESS_INFO	pi ;
		pi.dwProcessId = GetCurrentProcessId () ;
		GetModuleFileName ( GetModuleHandle(NULL), pi.lpPath, MAX_BUF_SIZE ) ;
		data.AddProcessInfo ( &pi ) ;
	}

	return ret ;
}