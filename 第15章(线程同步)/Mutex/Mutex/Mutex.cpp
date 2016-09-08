// Mutex.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"



#include <windows.h>

#include <iostream>
using namespace std;

#define DEF_DATA_SIZE	1024			// 定义数据大小
#define DEF_THREAD_NUM	30				// 定义线程数量

// 定义共享数据
typedef struct _SHARE_DATA {
	LONG	nCount ;					// 操作次数
	BYTE	data[DEF_DATA_SIZE] ;		// 共享数据主体
} SHARE_DATA ;
SHARE_DATA	ShareData = {0};

// 定义互斥对象
HANDLE	hMutex = NULL ;

// 工作线程
DWORD WINAPI WorkThread ( LPVOID lParam )
{
	for ( int i = 0; i < 100; i++ )
	{
		// 获取互斥对象
		WaitForSingleObject ( hMutex, INFINITE ) ;

		// 操作共享数据
		ShareData.nCount ++ ;
		for ( int i = 0; i < DEF_DATA_SIZE; i++ )
			ShareData.data[i] = (BYTE)lParam ;

		// 释放互斥对象
		ReleaseMutex ( hMutex ) ;
	}
	return 0 ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 创建互斥对象
	hMutex = CreateMutex ( NULL, FALSE, NULL ) ;
	if ( hMutex == NULL )
	{
		cout << "创建互斥对象失败！" << endl ;
		return 0 ;
	}

	int i = 0 ;
	HANDLE hThread[DEF_THREAD_NUM] = {0} ;
	// 创建多个线程
	for ( i = 0; i < DEF_THREAD_NUM; i++ )
	{
		hThread[i] = CreateThread ( NULL, 0, WorkThread, (LPVOID)i, 0, NULL ) ;
		if ( hThread[i] == NULL )
		{
			cout << "创建线程失败" << endl ;
			break ;
		}
	}

	if ( i == DEF_THREAD_NUM )
	{
		// 等待所有线程结束
		WaitForMultipleObjects ( DEF_THREAD_NUM, hThread, TRUE, INFINITE ) ;
		cout << "count=" << ShareData.nCount << endl ;
	}

	// 关闭所有线程句柄
	for ( i = 0; i < DEF_THREAD_NUM; i++ )
	{
		if ( hThread[i] )
			CloseHandle ( hThread[i] ) ;
	}

	// 删除互斥对象
	CloseHandle ( hMutex ) ;
	return 0;
}


