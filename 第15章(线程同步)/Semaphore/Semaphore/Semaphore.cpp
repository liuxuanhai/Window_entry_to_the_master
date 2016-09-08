// Semaphore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>

#include <iostream>
using namespace std ;

#include "stdio.h"

#define DEF_DATA_SIZE		128
#define MAX_ACCESS_COUNT	3
#define MAX_THREAD_COUNT	5
#define SEMPHORE_NAME		L"TestSemphore"

BYTE data[DEF_DATA_SIZE] ;

DWORD WINAPI WorkThread ( LPVOID lParam )
{
	// 取得线程索引号
	DWORD dwIndex = *((DWORD*)lParam) ;

	// 打开信标内核对象
	HANDLE hSem = OpenSemaphore ( SEMAPHORE_ALL_ACCESS, FALSE, SEMPHORE_NAME ) ;
	if ( hSem )
	{
		// 等待信标资源
		printf ( "[%d]等待信标资源\n", dwIndex ) ;
		if ( WaitForSingleObject ( hSem, INFINITE ) == WAIT_OBJECT_0 )
		{
			printf ( "[%d]取得信标资源，可以访问数据！\n", dwIndex ) ;

			// 到这里，此线程取得数据访问权，假设访问数据data需要3S时间
			// ……
			Sleep ( 3000 ) ;
		}

		// 释放信标资源
		printf ( "[%d]释放信标资源！\n", dwIndex ) ;
		ReleaseSemaphore ( hSem, 1, NULL ) ;
		CloseHandle ( hSem ) ;
	}
	else
	{
		printf ( "[%d]无法打开信标内核对象！\n", dwIndex ) ;
	}
	return 0 ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 初始化数据
	for ( BYTE i = 0; i < DEF_DATA_SIZE; i++ )
		data[i] = i ;

	// 创建信标内核对象
	HANDLE hSem = CreateSemaphore ( NULL, 0, MAX_ACCESS_COUNT, SEMPHORE_NAME ) ;
	if ( hSem == NULL )
		return 0;

	// 创建工作线程
	DWORD	dwIndex[MAX_THREAD_COUNT] = {0} ;
	HANDLE	hThread[MAX_THREAD_COUNT] = {0} ;
	for ( int i = 0; i < MAX_THREAD_COUNT; i++ )
	{
		dwIndex[i] = i + 1 ;
		hThread[i] = CreateThread ( NULL, 0, WorkThread, &dwIndex[i], 0, NULL ) ;
		if ( hThread[i] == NULL )
			return 0 ;
	}

	// Sleep一段时间，使各个工作线程都处于就绪状态
	// 即使所有工作线程都处于等待信标资源的状态
	Sleep ( 2000 ) ;

	// 设置信号量，激活工作线程
	printf ( "设置信号量，激活工作线程\n" ) ;
	ReleaseSemaphore ( hSem, MAX_ACCESS_COUNT, NULL ) ;

	// 等待线程结束
	WaitForMultipleObjects ( MAX_THREAD_COUNT, hThread, TRUE, INFINITE ) ;
	printf ( "所有线程都已经结束！\n" ) ;

	for ( int i = 0; i < MAX_THREAD_COUNT; i++ )
		CloseHandle ( hThread[i] ) ;
	CloseHandle ( hSem ) ;
	return 0;
}

