#pragma once

#include <dbghelp.h>
#pragma comment ( lib, "dbghelp.lib" )

#include <list>
using namespace std ;

// 定义内存映射结构
typedef struct _MAP_ITEM {
	HANDLE	hFile ;				// 文件内核对象句柄
	HANDLE	hMapFile ;			// 文件映射对象句柄
	LPVOID	lpMapAddr ;			// 映射基地址
} MAP_ITEM ;
typedef MAP_ITEM* PMAP_ITEM ;

#define MAX_RES_TYPES 64

// 定义资源数据信息
typedef struct _RES_ITEM {
	DWORD	dwNameOrId ;		// 资源名称偏移量或资源ID
	DWORD	dwOffsetToData ;	// 资源数据偏移量
	DWORD	dwDataOffset ;		// 只对IMAGE_DATA_DIRECTORY_ENTRY有效
	DWORD	dwDataSize ;		// 只对IMAGE_DATA_DIRECTORY_ENTRY有效
	WORD	wNumOfNamedEntry ;	// 名称入口数
	WORD	wNumOfIdEntry ;		// ID入口数
	struct _RES_ITEM () {
		dwNameOrId = dwOffsetToData = dwDataOffset = dwDataSize = wNumOfNamedEntry = wNumOfIdEntry = 0 ;
	}
} RES_ITEM ;
typedef RES_ITEM* PRES_ITEM ;


class CPEInfo
{
public:
	CPEInfo(void);
public:
	~CPEInfo(void);

public:
	BOOL						isReady ;						// 标识PE文件是否已经映射
	MAP_ITEM					MapItem ;						// 映射结构
	PIMAGE_DOS_HEADER			pDosHeader ;					// 指向DOS文件头
	PIMAGE_NT_HEADERS			pNtHeader ;						// 指向NT文件头
	PIMAGE_OPTIONAL_HEADER32	pOptionalHeader ;				// 指向可选文件头
	LPVOID						lpResBaseRva ;					// 资源RVA
	LPVOID						lpResBase ;						// 资源VA
	DWORD						dwNamedEntry, dwIdEntry ;		// 资源根目录
	RES_ITEM					ResDirectory[MAX_RES_TYPES] ;	// 资源类型数组，记录该类型的基本信息
	list<RES_ITEM>				ResList[MAX_RES_TYPES] ;		// 资源链表，每一类型的资源对应一个链表


public:
	// 使用内存映射方式把PE文件映射到当前进程
	BOOL	CreateMap	( TCHAR* pPathName ) ;
	// 卸载映射文件
	VOID	DeleteMap	( ) ;
	// 把映射区域的数据写入到文件
	BOOL	FlushData	( LPVOID lpAddr, UINT nNumToFlush ) ;

public:
	// 取得资源的基本信息
	void	GetResInfo	() ;
	// 根据资源类型索引及其在该类型中的索引，取得资源的基本信息，保存到pResItem
	void	GetResItemByIndex ( int nRoot, int nSub, PRES_ITEM pResItem ) ;
	// 把指定的资源写入的lpTarFile
	BOOL	DumpResource ( LPTSTR lpTarFile, LPVOID lpResStart, DWORD dwResSize ) ;
};
