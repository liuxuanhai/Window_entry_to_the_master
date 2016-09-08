
#include  <windows.h>

#define _EXPORT
#include "Interface.h"

int			value ;
POSITION	pos ;

BOOL WINAPI DllMain ( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
    // 分别处理各种通知
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            // 为新进程初始化，只能进行1次
            // 如果加载失败则返回FALSE
			value = 5 ;
			pos.x = 6 ;
			pos.y = 7 ;
            break;
        case DLL_THREAD_ATTACH:
            // 执行线程初始化操作
            break;
        case DLL_THREAD_DETACH:
            // 执行线程清理操作
            break;
        case DLL_PROCESS_DETACH:
            // 执行清理操作
            break;
    }
    return TRUE;
}


int FUN_Add ( int a, int b )
{
	return a + b ;
}

int FUN_Mul ( int a, int b ) 
{
	return a * b ;
}

CPerson::CPerson ()
{
}

CPerson::~CPerson ()
{
}

CPerson::CPerson ( char* lpName, int nAge )
{
	this->age = nAge ;
	if ( lpName )
	{
		int len = strlen(lpName) ;
		if ( len >= 127 )
			len = 127 ;
		memcpy ( this->szName, lpName, len ) ;
		this->szName[len] = 0 ;
	}
}

char* CPerson::GetName ()
{
	return this->szName ;
}

int CPerson::GetAge ()
{
	return this->age ;
}