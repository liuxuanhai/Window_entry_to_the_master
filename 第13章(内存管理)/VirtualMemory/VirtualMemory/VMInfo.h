#pragma once

class CVMInfo
{
public:
	CVMInfo(void);
public:
	~CVMInfo(void);

public:
	SYSTEM_INFO		SysInfo ;
	MEMORYSTATUS	MemStatus ;
	list<MEMORY_BASIC_INFORMATION>	MemList ;

public:
	VOID	VM_GetSysInfo () ;
	VOID	VM_GetMemStatus () ;
	BOOL	VM_GetMemInfoList ( DWORD dwProcessId ) ;
};
