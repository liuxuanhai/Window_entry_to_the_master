// TLS_Dynamic.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <windows.h>
#include <iostream>
using namespace std ;

// 全局变量保存TLS时隙索引值
// 约定所有线程都使用TLS的这个时隙
UINT	nTlsIndex = 0 ;

DWORD WINAPI NewThread ( LPVOID lParam )
{
	// 设置子线程TLS值
	TlsSetValue ( nTlsIndex, (LPVOID)10 ) ;
	// 取得子线程TLS值
	cout << "子线程" << nTlsIndex << "号TLS时隙值为" << (DWORD)TlsGetValue(nTlsIndex ) << endl ;
	return 0 ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 动态分配TLS时隙
	nTlsIndex = TlsAlloc () ;
	if ( nTlsIndex != TLS_OUT_OF_INDEXES )
	{
		cout << "TLS时隙为" << nTlsIndex << endl ;

		// 主线程TLS指定时隙的值
		TlsSetValue ( nTlsIndex, (LPVOID)5 ) ;

		// 创建子线程
		HANDLE hThread = CreateThread ( NULL, 0, NewThread, NULL, 0, NULL ) ;
		if ( hThread )
		{
			// 等待子线程结束
			WaitForSingleObject ( hThread, INFINITE ) ;
			// 取得主线程TLS的值，观察是否被改变
			cout << "主线程" << nTlsIndex << "号TLS时隙值为" << (DWORD)TlsGetValue(nTlsIndex ) << endl ;
		}

		// 不使用时应该释放TLS时隙
		TlsFree ( nTlsIndex ) ;
	}
	else
	{
		cout << "没有可用的TLS时隙！" << endl ;
	}

	cin.get () ;
	return 0;
}

