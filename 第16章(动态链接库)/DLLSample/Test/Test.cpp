// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include "..\\DLLSample\\Interface.h"

#include <iostream>
using namespace std ;

int _tmain(int argc, _TCHAR* argv[])
{
	// 测试导出变量
	cout << "value=" << value << endl ;
	cout << "pos=(" << pos.x << "," << pos.y << ")" << endl ;

	// 测试导出函数
	cout << "FUN_Add(5,6)=" << FUN_Add(5,6) << endl ;
	cout << "FUN_Mul(5,6)=" << FUN_Mul(5,6) << endl ;

	// 测试导出类
	CPerson person ( "Zhang", 23 ) ;
	cout << "Name=" << person.GetName() << endl ;
	cout << "Age=" << person.GetAge() << endl ;

	cin.get() ;
	return 0;
}

