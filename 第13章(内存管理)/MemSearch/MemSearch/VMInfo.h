#pragma once

#define MAX_ADDR_NUM 10000

class CVMInfo
{
public:
	CVMInfo(void);
	~CVMInfo(void);

public:
	SYSTEM_INFO		SysInfo ;
	MEMORYSTATUS	MemStatus ;
	list<MEMORY_BASIC_INFORMATION>	MemList ;
	list<DWORD>		AddrList ;

public:
	VOID	VM_GetSysInfo () ;
	VOID	VM_GetMemStatus () ;
	BOOL	VM_GetMemInfoList ( DWORD dwProcessId ) ;

public:
	UINT	VM_FirstSearch ( DWORD dwProcessId, DWORD dwValue ) ;
	UINT	VM_ReSearch ( DWORD dwProcessId, DWORD dwValue ) ;
	BOOL	VM_Modify ( DWORD dwProcessId, DWORD dwTagAddr, DWORD dwValue ) ;
};
