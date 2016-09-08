// TLS_Static.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>
#include <iostream>
using namespace std ;

// 定义静态TLS全局变量
__declspec(thread) int value = 0 ;

DWORD WINAPI NewThread ( LPVOID lParam )
{
	// 设置子线程value为10，并不影响主线程
	value = 10 ;
	// 取得子线程静态TLS的值
	cout << "子线程 value=" << value << endl ;
	return 0 ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 设置主线程静态TLS的value为5
	value = 5 ;
	// 创建子线程
	HANDLE hThread = CreateThread ( NULL, 0, NewThread, NULL, 0, NULL ) ;
	if ( hThread )
	{
		// 等待直到子线程结束
		WaitForSingleObject ( hThread, INFINITE ) ;
		// 取得主线程静态TLS的值
		cout << "主线程 value=" << value << endl ;
	}
	return 0;
}

