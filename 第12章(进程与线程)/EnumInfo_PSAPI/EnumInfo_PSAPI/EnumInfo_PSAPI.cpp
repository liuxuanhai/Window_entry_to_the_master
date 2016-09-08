// EnumInfo_PSAPI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>

#include "psapi.h"
#pragma comment ( lib, "psapi.lib" )

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std ;

#define MAX_BUF_SIZE 1024

BOOL EnumProcessInformation ()
{
	ofstream fout ( "EnumInfo_psapi.txt" ) ;

    DWORD dwProcessId[1024], cbNeededProcess;
	// 枚举进程
    if ( !EnumProcesses( dwProcessId, sizeof(dwProcessId), &cbNeededProcess ) )
        return FALSE;

	// 枚举进程信息
    for ( unsigned int i = 0; i < ( cbNeededProcess/sizeof(DWORD) ); i++ )
	{
		char szProcessName[MAX_PATH] = "unknown";

		// 通过进程ID打开进程
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE, dwProcessId[i] );
		if ( hProcess != NULL )
		{	
			HMODULE hMods[MAX_BUF_SIZE] = {0} ;
			DWORD cbNeededModule = 0 ;
			char szModuleName[MAX_BUF_SIZE] = { 0 } ;
			PROCESS_MEMORY_COUNTERS ProcessMemCounters ;

			// 枚举进程模块信息
			EnumProcessModules( hProcess, hMods, sizeof(hMods), &cbNeededModule ) ;
			// 取得主模块全名，每个进程的第一模块即为进程主模块，这里使用ANSI版
			::GetModuleFileNameExA ( hProcess, hMods[0], szModuleName, sizeof(szModuleName) ) ;
			// 取得进程的内存使用信息
			GetProcessMemoryInfo ( hProcess, &ProcessMemCounters, sizeof(ProcessMemCounters) ) ;

			// 输出进程相关信息到文件
			fout << "ProcessId=" << dwProcessId[i] << " " << szModuleName << endl ;
			fout << '\t' << "PageFaultCount                 :" << \
				hex << setw(8) << ProcessMemCounters.PageFaultCount << endl ;
			fout << '\t' << "PeakWorkingSetSize             :" << \
				hex << setw(8) << ProcessMemCounters.PeakWorkingSetSize	<< endl ;
			fout << '\t' << "WorkingSetSize                 :" << \
				hex << setw(8) << ProcessMemCounters.WorkingSetSize << endl ;
			fout << '\t' << "QuotaPeakPagedPoolUsage        :" << \
				hex << setw(8) << ProcessMemCounters.QuotaPeakPagedPoolUsage << endl ;
			fout << '\t' << "QuotaPagedPoolUsage            :" << \
				hex << setw(8) << ProcessMemCounters.QuotaPagedPoolUsage << endl ;
			fout << '\t' << "QuotaPeakNonPagedPoolUsage     :" << \
				hex << setw(8) << ProcessMemCounters.QuotaPeakNonPagedPoolUsage	<< endl ;
			fout << '\t' << "QuotaNonPagedPoolUsage         :" << \
				hex << setw(8) << ProcessMemCounters.QuotaNonPagedPoolUsage	<< endl ;
			fout << '\t' << "PagefileUsage                  :" << \
				hex << setw(8) << ProcessMemCounters.PagefileUsage << endl ;
			fout << '\t' << "PeakPagefileUsage              :" << \
				hex << setw(8) << ProcessMemCounters.PeakPagefileUsage << endl ;

			// 关闭进程句柄
			CloseHandle( hProcess );
		}
	}

	// 关闭文件输出流句柄
	fout.close() ;
	return TRUE ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if ( EnumProcessInformation () == FALSE )
		cout << "Enum process information error!" << endl ;
	return 0;
}

