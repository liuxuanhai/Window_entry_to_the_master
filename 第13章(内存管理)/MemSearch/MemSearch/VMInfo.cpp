#include "StdAfx.h"
#include "VMInfo.h"

char szLogBuf[2048] ;

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

UINT CVMInfo::VM_FirstSearch ( DWORD dwProcessId, DWORD dwValue )
{
	if ( this->MemList.empty () )
		return 0;

	this->AddrList.clear () ;

	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if ( dwProcessId != GetCurrentProcessId() )
	{
		// 打开目标进程
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessId );
		if ( hProcess == NULL )
			return 0 ;
	}

	LPBYTE lpBuf = new BYTE[this->SysInfo.dwPageSize] ;	// 页面缓冲区
	list<MEMORY_BASIC_INFORMATION>::iterator p ;
	// 遍历内存区域
	for ( p = this->MemList.begin(); p != this->MemList.end(); p++ )
	{
		// 检测是否为“提交”状态
		if ( p->State != MEM_COMMIT )
			continue ;

		sprintf ( szLogBuf, "Base=0x%08X Size=0x%08X", p->BaseAddress, p->RegionSize ) ;
		OutputDebugStringA ( szLogBuf ) ;

		DWORD dwSize = 0, dwReadBytes = 0  ;
		// 遍历当前内存区域内所有页面
		while ( dwSize < p->RegionSize )
		{
			LPVOID lpAddr = (LPVOID)((DWORD)p->BaseAddress+dwSize) ;
			if ( ReadProcessMemory ( hProcess, lpAddr, lpBuf, this->SysInfo.dwPageSize, &dwReadBytes ) == FALSE )
			{
				dwSize += this->SysInfo.dwPageSize ;
				continue ;
			}

			// 在当前页中搜索目标值
			for ( int i = 0; i < this->SysInfo.dwPageSize-3; i++ )
			{
				// 把目标地址添加到地址链表中
				if ( *((DWORD*)(lpBuf+i)) == dwValue )
					this->AddrList.push_back ( (DWORD)lpAddr+i ) ;

				// 当符合条件的目标地址数目太多时，进行限制
				if ( this->AddrList.size() == MAX_ADDR_NUM )
				{
					delete []lpBuf ;
					CloseHandle ( hProcess ) ;
					return MAX_ADDR_NUM ;
				}
			}

			dwSize += this->SysInfo.dwPageSize ;
		}
	}

	delete []lpBuf ;
	CloseHandle ( hProcess ) ;
	return this->AddrList.size () ;
}

UINT CVMInfo::VM_ReSearch ( DWORD dwProcessId, DWORD dwValue )
{
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if ( dwProcessId != GetCurrentProcessId() )
	{
		// 打开目标进程
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessId );
		if ( hProcess == NULL )
			return FALSE ;
	}

	list<DWORD> TempList ;
	list<DWORD>::iterator p ;
	DWORD dwReadBytes = 0, dwCurValue = 0 ;
	for ( p = this->AddrList.begin(); p != this->AddrList.end (); p++ )
	{
		ReadProcessMemory ( hProcess, (LPVOID)*p, &dwCurValue, sizeof(DWORD), &dwReadBytes ) ;
		if ( dwCurValue == dwValue )
			TempList.push_back ( *p ) ;
	}
	this->AddrList.clear () ;
	this->AddrList.insert ( this->AddrList.begin(), TempList.begin(), TempList.end() ) ;

	CloseHandle ( hProcess ) ;
	return this->AddrList.size() ;
}

BOOL CVMInfo::VM_Modify ( DWORD dwProcessId, DWORD dwTagAddr, DWORD dwValue )
{
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	if ( dwProcessId != GetCurrentProcessId() )
	{
		// 打开目标进程
		hProcess = OpenProcess( PROCESS_ALL_ACCESS,FALSE,dwProcessId );
		if ( hProcess == NULL )
			return FALSE ;
	}

	DWORD dwWriteBytes = 0, dwOldProtect ;
	// 修改目标地址的保护方式为可读可写
	VirtualProtectEx ( hProcess, (LPVOID)dwTagAddr, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect ) ; 
	// 修改数据
	WriteProcessMemory ( hProcess, (LPVOID)dwTagAddr, &dwValue, sizeof(DWORD), &dwWriteBytes ) ;
	// 还原目标地址的保护方式
	VirtualProtectEx ( hProcess, (LPVOID)dwTagAddr, sizeof(DWORD), dwOldProtect, NULL ) ; 

	CloseHandle ( hProcess ) ;
	return TRUE ;
}