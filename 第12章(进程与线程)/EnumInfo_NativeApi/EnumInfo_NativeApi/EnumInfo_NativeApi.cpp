// EnumInfo_NativeApi.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>

#include <stdio.h>
#include <iostream>
using namespace std ;

#include "MyNtdll.h"

#define DEF_BUF_SIZE	0x1000

PFNNtQuerySystemInformation NtQuerySystemInformation ;

// 步骤2：取得NtQuerySystemInformation函数地址
BOOL Initial ()
{
	// 检测当前进程中是否存在ntdll.dll
	HMODULE	hMod = GetModuleHandle ( L"ntdll.dll" ) ;
	if ( hMod == NULL )
	{
		// 如果不存在，就使用LoadLibrary来加载
		hMod = LoadLibrary ( L"ntdll.dll" ) ;
		if ( hMod == NULL )
			return FALSE ;
	}

	// 取得函数地址
	NtQuerySystemInformation = (PFNNtQuerySystemInformation)GetProcAddress ( hMod, "NtQuerySystemInformation" ) ;
	return TRUE ;
}

// 输出进程信息
VOID DisplayProcessInformation ( LPBYTE lpBuf )
{	
	cout << "///////////////////////////////////////////////////////////////////////" << endl ;
	cout << "//                       Process Information                         //" << endl ;
	cout << "///////////////////////////////////////////////////////////////////////" << endl ;
	PSYSTEM_PROCESSES pSysProcess = (PSYSTEM_PROCESSES)lpBuf ;
	while ( TRUE )
	{
		cout << "ProcessName:" << '\t' ; 
		if ( pSysProcess->ProcessName.Buffer != NULL )
			printf ( "%30S", pSysProcess->ProcessName.Buffer ) ;
		cout << endl ;
		
		cout << "InheritedFromProcessId:\t\t" << pSysProcess->InheritedFromProcessId << endl ;
		cout << "ProcessId:\t\t\t" << pSysProcess->ProcessId << endl ;
		cout << "HandleCount:\t\t\t" << pSysProcess->HandleCount << endl ;
		cout << "ThreadCount:\t\t\t" << pSysProcess->ThreadCount << endl ;

		cout << "-------------------------------------------------------------------------" << endl ;
		if ( pSysProcess->NextEntryDelta == 0 )
			break ;
		pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta ) ;
	}
}

// 步骤3：枚举进程信息
VOID QuerySystemInformation ( SYSTEM_INFORMATION_CLASS SystemInformationClass )
{
	NTSTATUS	status ;
	UINT		nSize = DEF_BUF_SIZE ;
	LPBYTE		lpBuf = NULL ;

	// 由于事先并不知道需要多少空间来存储进程信息
	// 因而采用循环测试法，
	while ( TRUE )
	{
		// 动态分配空间，用来存储进程信息
		if ( ( lpBuf = new BYTE [ nSize ] ) == NULL )
		{
			cout << "Allocate memory failed!" << endl ;
			return ;
		}
		
		// 枚举进程信息
		status = NtQuerySystemInformation ( SystemInformationClass, lpBuf, nSize, 0 ) ;
		if ( !NT_SUCCESS(status) )
		{
			// 检测是否返回缓冲区不够大
			if ( status == STATUS_INFO_LENGTH_MISMATCH )
			{
				nSize += DEF_BUF_SIZE ;
				delete lpBuf ;
				continue ;
			}
			else
			{
				cout << "NtQuerySystemInformation ErrorCode=0x" << hex << status << endl ;
				return ;
			}
		}
		else
			break ;
	}

	// 输出进程信息
	DisplayProcessInformation ( lpBuf ) ;
	delete lpBuf ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if ( Initial() == FALSE )
	{
		cout << "Initial failed!" << endl ;
		return 0 ;
	}

	QuerySystemInformation ( SystemProcessesAndThreadsInformation ) ;

	cin.get() ;
	return 0;
}
