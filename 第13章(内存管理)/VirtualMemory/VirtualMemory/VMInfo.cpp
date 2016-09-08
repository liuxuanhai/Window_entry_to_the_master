#include "StdAfx.h"
#include "VMInfo.h"

CVMInfo::CVMInfo(void)
{
}

CVMInfo::~CVMInfo(void)
{
}

VOID CVMInfo::VM_GetSysInfo()
{
	GetSystemInfo ( &this->SysInfo ) ;
}

VOID CVMInfo::VM_GetMemStatus()
{
	GlobalMemoryStatus ( &this->MemStatus ) ;
}

BOOL CVMInfo::VM_GetMemInfoList ( DWORD dwProcessId )
{
	// 清空虚拟内存信息链表
	this->MemList.clear () ;

	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if ( dwProcessId != GetCurrentProcessId() )
	{
		// 打开目标进程
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessId );
		if ( hProcess == NULL )
			return FALSE ;
	}

	MEMORY_BASIC_INFORMATION	MemBaseInfo ;
	DWORD dwInfoSize = sizeof(MEMORY_BASIC_INFORMATION) ;
	DWORD dwCurPos = (DWORD)this->SysInfo.lpMinimumApplicationAddress ;
	while ( dwCurPos < (DWORD)this->SysInfo.lpMaximumApplicationAddress )
	{
		// 查询指定进程的指定地址的状态信息
		VirtualQueryEx ( hProcess, (LPVOID)dwCurPos, &MemBaseInfo, dwInfoSize ) ;
		// 把状态信息添加到链表
		this->MemList.push_back(MemBaseInfo) ;
		// 定位到下一个区域
		dwCurPos = (DWORD)MemBaseInfo.BaseAddress + MemBaseInfo.RegionSize ;
	}
	return TRUE ;
}