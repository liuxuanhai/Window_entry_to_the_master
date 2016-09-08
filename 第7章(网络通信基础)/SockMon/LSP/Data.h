#pragma once

// 定义进程信息结构
typedef struct _PROCESS_INFO {
	DWORD	dwProcessId ;					// 进程ID
	WCHAR	lpPath[MAX_PATH_LENGTH] ;		// 进程路径全名
} PROCESS_INFO, *LPPROCESS_INFO ;
		
// 定义套接字信息结构
typedef struct _SOCKET_INFO {
	SOCKET		s ;							// 套接字句柄
	int			protocol ;					// 协议类型
	DWORD		dwProcessId ;				// 进程ID
	SYSTEMTIME	CreateTime ;				// 创建时间
} SOCKET_INFO, *LPSOCKET_INFO ;

// 定义操作信息结构
typedef struct _OPERATE_INFO {
	SOCKET		s ;							// 套接字句柄
	WCHAR		szOperate[64] ;				// 操作类型，即所调用的函数名
	DWORD		dwProcessId ;				// 进程ID
	DWORD		dwThreadId ;				// 线程ID
	sockaddr_in	LocalAddr ;					// 本地地址信息
	sockaddr_in RemoteAddr ;				// 远程地址信息
	int			nErrorCode ;				// 操作返回的错误码
	DWORD		dwDataSize ;				// 传输数据的大小
	LPBYTE		lpData ;					// 指向传输数据
} OPERATE_INFO, *LPOPERATE_INFO ;



class CData
{
public:
	CData(void);
public:
	~CData(void);

private:
	HANDLE				hFile ;
	HANDLE				hMapFile ;
	LPBYTE				lpData ;
	UINT				nCount ;
	CRITICAL_SECTION	cs ;

public:
	VOID	AddProcessInfo ( LPPROCESS_INFO lpProcessInfo ) ;
	VOID	AddSocketInfo ( LPSOCKET_INFO lpSockInfo ) ;
	VOID	AddOperateInfo ( LPOPERATE_INFO lpOperateInfo ) ;

private:
	VOID	SendDataToClient ( LPBYTE lpData, DWORD dwSize ) ;
	VOID	SendNotifyMessage ( WPARAM wParam, LPARAM lParam ) ;
	UINT	GetIndex () ;

private:
	BOOL	SetupShareMemeory () ;
	VOID	ReleaseShareMemory () ;
};
