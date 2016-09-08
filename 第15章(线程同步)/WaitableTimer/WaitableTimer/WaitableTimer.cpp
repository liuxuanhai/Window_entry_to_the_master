// WaitableTimer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
	// 创建等待定时器
	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL );
	if ( !hTimer )
	{
		cout << "创建等待定时器失败！(" << GetLastError() << ")" << endl ;
		return 0 ;
	}

	// 设置等待定时器，相对时间，5S
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart=-50000000;
	if ( !SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0) )
	{
		cout << "设置等待定时器失败！(" << GetLastError() << ")" << endl ;
		return 0 ;
	}

	cout << "等待5秒……" << endl ;

	// 等待受信
	if ( WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0 )
		cout << "等待失败！(" << GetLastError() << ")" << endl ;
	else
		cout << "接受到等待定时器信号！" << endl ;

	CloseHandle ( hTimer ) ;
	return 0;
}

