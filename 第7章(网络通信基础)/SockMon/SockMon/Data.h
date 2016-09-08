#pragma once



typedef struct _OPERATE_INFO {
	SOCKET		s ;
	WCHAR		szOperate[64] ;
	DWORD		dwProcessId ;
	DWORD		dwThreadId ;
	sockaddr_in	LocalAddr ;
	sockaddr_in RemoteAddr ;
	int			nErrorCode ;
	DWORD		dwDataSize ;
	LPBYTE		lpData ;
} OPERATE_INFO, *LPOPERATE_INFO ;

typedef struct _OPERATE_ITEM {
	UINT			nOperateIndex ;				// 在所属套接字的操作索引
	OPERATE_INFO	OperateInfo ;				// 操作信息结构
} OPERATE_ITEM, *LPOPERATE_ITEM ;

typedef struct _SOCKET_INFO {
	SOCKET		s ;
	int			protocol ;
	DWORD		dwProcessId ;
	SYSTEMTIME	CreateTime ;
} SOCKET_INFO, *LPSOCKET_INFO ;

typedef struct _SOCKET_ITEM {
	UINT					nSocketIndex ;		// 在所属进程的套接字索引
	UINT					nOperateCount ;		// 该套接字所包含的操作数
	SOCKET_INFO				SocketInfo ;		// 套接字信息
	vector<OPERATE_ITEM>	OperateTable ;		// 操作信息表
	HTREEITEM				hTreeItem ;			// 树形控件子项句柄
} SOCKET_ITEM, *LPSOCKET_ITEM ;

typedef struct _PROCESS_INFO {
	DWORD	dwProcessId ;
	WCHAR	lpPath[MAX_PATH_LENGTH] ;
} PROCESS_INFO, *LPPROCESS_INFO ;

typedef struct _PROCESS_ITEM {
	UINT					nProcessIndex ;		// 进程索引
	UINT					nSocketCount ;		// 该进程所包含的套接字数
	PROCESS_INFO			ProcessInfo ;		// 进程信息
	vector<SOCKET_ITEM>		SocketTable ;		// 套接字信息表
	HTREEITEM				hTreeItem ;			// 树形控件子项句柄
} PROCESS_ITEM, *LPPROCESS_ITEM ;


class CData
{
public:
	CData(void);
public:
	~CData(void);

private:
	//volatile LONG			nCount ;
	LONG					nCount ;
	HANDLE					hFile ;
	HANDLE					hMapFile ;
	LPBYTE					lpData ;
	CTreeCtrl*				pSockTree ;
	HTREEITEM				hRootTree ;

	vector<PROCESS_ITEM>	ProcessTable ;

public:
	VOID SetTreeCtrl ( CTreeCtrl* pTree ) ;
	VOID UpdateInfo () ;
	VOID ResetSockData () ;
	VOID FlushSockData () ;
	BOOL IsFull () ;

private:
	VOID AddProcessItem( LPPROCESS_INFO lpProcessInfo ) ;
	VOID AddSocketItem( LPSOCKET_INFO lpSocketInfo ) ;
	VOID AddOperateItem( LPOPERATE_INFO lpOperateInfo ) ;

private:
	UINT GetProcessIndexById ( UINT nProcessId ) ;
	UINT GetSocketIndexByHandle ( UINT nProcessIndex,  SOCKET s ) ;

private:
	BOOL SetupShareMemeory () ;
	VOID ReleaseShareMemory () ;
};