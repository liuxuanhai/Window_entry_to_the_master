// ShowExceptFrame1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include <windows.h>
#include <stdio.h>

// 
// SCOPETABLE
//
typedef struct _SCOPETABLE{
	DWORD	previousTryLevel;
	DWORD	lpfnFilter;
	DWORD	lpfnHandler;
} SCOPETABLE, *PSCOPETABLE ;

//
// VC编译器扩展的异常帧
//
typedef struct _VC_EXCEPTION_REGISTRATION{
	struct _VC_EXCEPTION_REGISTRATION* prev;
	DWORD	handler;
	PSCOPETABLE  scopetable;
	int		trylevel;
	int		_ebp;
} VC_EXCEPTION_REGISTRATION, *PVC_EXCEPTION_REGISTRATION ;

//
// 显示ScopeTable信息
//
void SEHShowScopeTable( PVC_EXCEPTION_REGISTRATION pVCExcRec )
{
	printf( "Frame: %08X  Handler: %08X  Prev: %08X  Scopetable: %08X\n", \
		pVCExcRec, pVCExcRec->handler, pVCExcRec->prev,	pVCExcRec->scopetable );
	
	PSCOPETABLE pScopeTableEntry = pVCExcRec->scopetable;
	
	for ( int i = 0; i <= pVCExcRec->trylevel; i++ )
	{
		printf( "    scopetable[%u] PrevTryLevel: %08X  "
			"filter: %08X  __except: %08X\n", i,
			pScopeTableEntry->previousTryLevel,
			pScopeTableEntry->lpfnFilter,
			pScopeTableEntry->lpfnHandler );
		
		pScopeTableEntry++;
	}
	
	printf( "\n" );
}   

//
//显示异常帧信息
//
void SEHShowExcptFrames( )
{
	PVC_EXCEPTION_REGISTRATION	pVCExcRec;
	
	// 取得异常帧链首地址，保存在pVCExcRec
	__asm   mov eax, FS:[0]
	__asm   mov [pVCExcRec], EAX
		
	// 遍历异常帧链
	while ( (unsigned)pVCExcRec != 0xFFFFFFFF )
	{
		SEHShowScopeTable( pVCExcRec );
		pVCExcRec = (PVC_EXCEPTION_REGISTRATION)(pVCExcRec->prev);
	}       
}

void Function()
{
	__try 
	{
		__try 
		{
			__try 
			{
				SEHShowExcptFrames();
			}
			__except( EXCEPTION_CONTINUE_SEARCH )
			{
				// ……
			}
		}
		__except( EXCEPTION_CONTINUE_SEARCH )
		{
			// ……
		}
	}
	__except ( EXCEPTION_CONTINUE_SEARCH )
	{
		// ……
	}
}

int main()
{
	__try 
	{
		Function();
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		//……
	}
	
	getchar () ;
	return 0;
}
