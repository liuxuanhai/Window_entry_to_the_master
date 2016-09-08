// Event.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include <iostream>
using namespace std;

HANDLE hEvent[2] = {0} ;
HANDLE hThread[2] = {0} ;

// 线程1
DWORD WINAPI FirstThread ( LPVOID lParam )
{
	// 等待线程2的事件通知
	WaitForSingleObject ( hEvent[0], INFINITE ) ;

	// 接收到线程2的事件通知，处理任务
	// ……

	// 通知线程2，当前的任务已经处理完成
	SetEvent ( hEvent[1] ) ;
	return 0 ;
}

// 线程2
DWORD WINAPI SecondThread ( LPVOID lParam )
{
	// 通知线程1，使其开始工作
	SetEvent ( hEvent[0] ) ;

	// 等待线程1工作的结束
	WaitForSingleObject ( hEvent[1], INFINITE ) ;

	// 接收到线程1的任务完成通知
	// ……
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 创建内核事件对象
	hEvent[0] = CreateEvent ( NULL, TRUE, FALSE, NULL ) ;
	hEvent[1] = CreateEvent ( NULL, TRUE, FALSE, NULL ) ;

	// 创建线程
	hThread[0] = CreateThread ( NULL, 0, FirstThread, NULL, 0, NULL ) ;
	hThread[1] = CreateThread ( NULL, 0, SecondThread, NULL, 0, NULL ) ;

	// 等待2个线程结束
	WaitForMultipleObjects ( 2, hThread, TRUE, INFINITE ) ;
	cout << "finish!" << endl ;

	CloseHandle ( hEvent[0] ) ;
	CloseHandle ( hEvent[1] ) ;
	CloseHandle ( hThread[0] ) ;
	CloseHandle ( hThread[1] ) ;
	return 0;
}

