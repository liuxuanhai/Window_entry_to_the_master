// CreateThreadSample.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "windows.h"

//DWORD WINAPI NewThread ( LPVOID lpParam )
//{
//	// 取得工作线程标识
//	UINT nThreadId = *((UINT*)lpParam ) ;
//
//	// 根据不同的标识处理不同的数据或者事务
//	switch ( nThreadId )
//	{
//	case 0:	{ /*处理0号数据或事务*/	} break ;
//	case 1:	{ /*处理1号数据或事务*/	} break ;
//	case 2:	{ /*处理2号数据或事务*/	} break ;
//	// ……
//	}
//	return 0 ;
//}
//
//VOID CreateNewThread ()
//{
//	const UINT WORK_THREAD_NUM = 4 ;			// 定义工作线程的数量
//	DWORD dwProcessId[WORK_THREAD_NUM] = {0} ;	// 用来存储线程ID
//
//	for ( UINT i = 0; i < WORK_THREAD_NUM; i++ )
//		CreateThread ( NULL, 0, NewThread, &i, 0, &dwProcessId[i] ) ;
//}

typedef struct _CLIENT_INFO {
	// ……
} CLIENT_INFO, *PCLIENT_INFO ;

DWORD WINAPI NewThread ( LPVOID lpParam )
{
	// 取得客户信息结构
	PCLIENT_INFO pClientInfo = (PCLIENT_INFO)lpParam ;

	// 处理客户请求
	return 0 ;
}

VOID Server ()
{
	// 不断地接受客户端请求
	while ( TRUE )
	{
		// 设置客户信息结构，省略对ClientInfo结构的填充
		CLIENT_INFO ClientInfo ;

		// 创建工作线程处理客户端请求
		CreateThread ( NULL, 0, NewThread, &ClientInfo, 0, NULL ) ;
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

