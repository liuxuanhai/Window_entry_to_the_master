#pragma once 


#ifndef _EXPORT
#define VARABLE_EXPORT	extern "C" __declspec(dllimport)
#define FUNCTION_EXPORT extern "C" __declspec(dllimport)
#define CLASS_DECLARE	__declspec(dllimport)
#else 
#define VARABLE_EXPORT	extern "C" __declspec(dllexport)
#define FUNCTION_EXPORT extern "C" __declspec(dllexport)
#define CLASS_DECLARE	__declspec(dllexport)
#endif

typedef struct _POSITION {
	int x ;
	int y ;
} POSITION, *PPOSITION ;

// 定义导出变量
VARABLE_EXPORT int		value ;	// 导出普通变量
VARABLE_EXPORT POSITION	pos ;	// 导出结构体变量

// 定义导出函数
FUNCTION_EXPORT int FUN_Add ( int a, int b ) ;
FUNCTION_EXPORT int FUN_Mul ( int a, int b ) ;

// 定义导出类
// 定义CPreson类
class CLASS_DECLARE CPerson {
private:
	char	szName[128] ;
	int		age ;
public:
	CPerson() ;
	~CPerson() ;
	CPerson ( char* lpName, int nAge ) ;
public:
	char* GetName () ;
	int GetAge () ;
} ;