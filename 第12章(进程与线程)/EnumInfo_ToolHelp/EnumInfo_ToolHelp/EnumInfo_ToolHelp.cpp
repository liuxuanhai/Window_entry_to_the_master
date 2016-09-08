// EnumInfo_ToolHelp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
using namespace std ;

#define MAX_BUF_SIZE 2048

// 枚举所有进程信息
BOOL EnumProcessInfo () ;
// 枚举所有线程
BOOL EnumThreadInfo () ;

int _tmain( )
{
	if ( EnumProcessInfo () == FALSE )
	{
		cout << "EnumProcessInfo failed!" << endl ;
	}
	if ( EnumThreadInfo () == FALSE )
	{
		cout << "EnumThreadInfo failed!" << endl ;
	}
	return 0;
}

// 枚举系统当前所有进程信息
// 并把信息输出到工程目录下EnumInfo_ToolHelp_process.txt
BOOL EnumProcessInfo()
{
	// 定义进程信息结构
	PROCESSENTRY32 pe32 = {sizeof(pe32)} ;

	// 创建系统当前进程快照
	HANDLE hProcessShot = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 ) ;
	if ( hProcessShot == INVALID_HANDLE_VALUE )
		return FALSE ;

	// 输出进程信息到文件
	ofstream fout ( "EnumInfo_ToolHelp_process.txt" ) ;

	// 循环枚举进程信息
	char szBuf[MAX_BUF_SIZE] = {0} ;
	if ( Process32First ( hProcessShot, &pe32 ) )
	{
		do {
			memset ( szBuf, 0, sizeof(szBuf) ) ;
			// 把宽字符的进程名转化为ANSI字符串
			WideCharToMultiByte (CP_ACP, 0, pe32.szExeFile, wcslen(pe32.szExeFile), szBuf, sizeof(szBuf), NULL, NULL );
			
			fout << "Process: " << szBuf << endl ;
			fout << '\t' << "Usage           : " << pe32.cntUsage << endl ;
			fout << '\t' << "ProcessID       : " << pe32.th32ProcessID << endl ;
			fout << '\t' <<	"DefaultHeapID   : " << (ULONG_PTR)pe32.th32DefaultHeapID << endl ;
			fout << '\t' << "ModuleID        : " << pe32.th32ModuleID << endl ;
			fout << '\t' << "ThreadNum       : " << pe32.cntThreads	<< endl ;
			fout << '\t' << "ParentProcessID : " << pe32.th32ParentProcessID << endl ;
			fout << '\t' << "PriClassBase    : " << pe32.pcPriClassBase << endl ;
		}while ( Process32Next ( hProcessShot, &pe32 ) ) ;
	}

	fout.close () ;
	CloseHandle ( hProcessShot ) ;
	return TRUE ;
}

// 枚举系统当前所有线程信息
// 并把信息输出到工程目录下EnumInfo_ToolHelp_thread.txt
BOOL EnumThreadInfo ()
{
	// 定义线程信息结构
	THREADENTRY32 te32 = {sizeof(THREADENTRY32)} ;

	//创建系统线程快照
	HANDLE hThreadSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPTHREAD, 0 ) ;
	if ( hThreadSnap == INVALID_HANDLE_VALUE )
		return FALSE ;

	// 输出线程信息到文件
	ofstream fout ( "EnumInfo_ToolHelp_thread.txt" ) ;

	// 循环枚举线程信息
	if ( Thread32First ( hThreadSnap, &te32 ) )
	{
		do{
			fout << "ThreadId : "   << te32.th32ThreadID << endl ;
			fout << '\t' << "OwnerProcessID : " << te32.th32OwnerProcessID << endl ;
			fout << '\t' << "Usage          : "	<< te32.cntUsage << endl ;
			fout << '\t' << "Delta Priority : " << te32.tpDeltaPri << endl ;
			fout << '\t' << "Base Priority  : " << te32.tpBasePri << endl ;
		}while ( Thread32Next ( hThreadSnap, &te32 ) ) ;
	}

	fout.close () ;
	CloseHandle ( hThreadSnap ) ;
	return TRUE ;
}