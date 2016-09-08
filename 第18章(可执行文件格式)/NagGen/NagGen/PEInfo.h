#pragma once


typedef struct _MAP_ITEM {
	HANDLE	hFile ;
	HANDLE	hMapFile ;
	LPVOID	lpMapAddr ;
} MAP_ITEM ;
typedef MAP_ITEM* PMAP_ITEM ;

typedef struct _BASE_INFO_ITEM {
	DWORD	dwSectionAlign ;
	DWORD	dwFileAlign ;
	WORD	dwSectionBase ;
	WORD	dwSectionNum ;
	DWORD	dwHeadSize ;
	DWORD	dwRealHeadSize ;
	DWORD	dwImageSize ;
	DWORD	dwFileSize ;
} BASE_INFO_ITEM ;
typedef BASE_INFO_ITEM* PBASE_INFO_ITEM ;


class CPEInfo
{
public:
	CPEInfo(void);
public:
	~CPEInfo(void);

public:
	BOOL					isReady ;
	BASE_INFO_ITEM			BaseInfo ;
	IMAGE_SECTION_HEADER	NewSection ;	

public:
	BOOL	CreateMap	( TCHAR* pPathName, PMAP_ITEM pMapItem ) ;
	BOOL	CreateNewMap	( TCHAR* pPathName, PMAP_ITEM pMapItem, DWORD dwFileSize ) ;
	VOID	DeleteMap	( PMAP_ITEM pMapItem ) ;
	BOOL	FlushData	( LPVOID lpAddr, UINT nNumToFlush ) ;

public:
	BOOL	IsValidPE		( PMAP_ITEM pMapItem ) ;
	VOID	GetBaseInfo		( PMAP_ITEM pMapItem ) ;
	VOID	InitNewSection	() ;
	VOID	AdjustSectionSize () ;		// Contain image size and file size
	VOID	AddNewSection	( PMAP_ITEM pOldMap, PMAP_ITEM pNewMap, DWORD dwNewFileSize ) ;

public:
	DWORD	FormatInt ( DWORD dwValue, DWORD dwAlign ) ;

};
