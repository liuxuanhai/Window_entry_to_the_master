// WaitableTimerApc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <iostream>
using namespace std;

#define REPEAT_COUNTS	5

// 等待定时器完成例程
VOID CALLBACK TimerAPCProc( LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue )
{
	DWORD dwIndex = *((DWORD*)lpArg) ;
	cout << "第" << dwIndex << "次报时！" << endl ;
}

void main( void ) 
{
	// 创建等待定时器
	HANDLE hTimer = CreateWaitableTimer ( NULL, FALSE, NULL ) ;
	if ( hTimer == NULL )
		return ;

	// 第一次报时在5S后，以后每隔1S报时1次
	LARGE_INTEGER   liDueTime;
	liDueTime.QuadPart  = -50000000 ;
	
	// 设置等待定时器
	DWORD dwParam = 1 ;
	if ( SetWaitableTimer ( hTimer, &liDueTime, 1000, TimerAPCProc, &dwParam, FALSE ) )
	{
		cout << "等待5S后开始报时……" << endl ;
		for ( int i = 0; i < REPEAT_COUNTS; i++, dwParam++ )
		{
			// 进入可变等待状态
			SleepEx ( INFINITE, TRUE ) ;
		}
	} 

	// 取消等待定时器
	CancelWaitableTimer ( hTimer ) ;
	CloseHandle( hTimer );
	return ;
}

