#include "StdAfx.h"
#include "PEInfo.h"

CPEInfo::CPEInfo(void)
{
	this->isReady = FALSE ;
	memset ( &this->BaseInfo, 0, sizeof(BASE_INFO_ITEM) ) ;
	memset ( &this->NewSection, 0, sizeof(IMAGE_SECTION_HEADER) ) ;
}

CPEInfo::~CPEInfo(void)
{
}

BOOL CPEInfo::CreateMap	( TCHAR* pPathName, PMAP_ITEM pMapItem ) 
{	
	pMapItem->hFile =	CreateFile ( 
		pPathName,					
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL ,
		NULL 
		);
	if ( pMapItem->hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	//创建文件映射内核对象
	pMapItem->hMapFile = CreateFileMapping ( pMapItem->hFile, NULL, PAGE_READONLY, 0, 0, NULL	) ;
	if ( pMapItem->hMapFile == NULL )
	{
		CloseHandle ( pMapItem->hFile ) ;
		return FALSE;
	}
	
	//创建文件视图
	pMapItem->lpMapAddr = (PBYTE)MapViewOfFile ( pMapItem->hMapFile, FILE_MAP_READ, 0, 0, 0 ) ;
	if ( pMapItem->lpMapAddr == NULL )
	{
		DWORD dwErrorCode = GetLastError () ;
		CloseHandle ( pMapItem->hMapFile ) ;
		CloseHandle ( pMapItem->hFile ) ;
		return FALSE;
	}
	
	return TRUE ;
}

BOOL CPEInfo::CreateNewMap	( TCHAR* pPathName, PMAP_ITEM pMapItem, DWORD dwFileSize ) 
{	
	pMapItem->hFile =	CreateFile ( 
		pPathName,					
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL ,
		NULL 
		);
	if ( pMapItem->hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	//创建文件映射内核对象
	pMapItem->hMapFile = CreateFileMapping ( pMapItem->hFile, NULL, \
							PAGE_READWRITE, 0, dwFileSize, NULL	) ;
	if ( pMapItem->hMapFile == NULL )
	{
		CloseHandle ( pMapItem->hFile ) ;
		return FALSE;
	}
	
	//创建文件视图
	pMapItem->lpMapAddr = (PBYTE)MapViewOfFile ( pMapItem->hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) ;
	if ( pMapItem->lpMapAddr == NULL )
	{
		DWORD dwErrorCode = GetLastError () ;
		CloseHandle ( pMapItem->hMapFile ) ;
		CloseHandle ( pMapItem->hFile ) ;
		return FALSE;
	}
	
	return TRUE ;
}

void  CPEInfo::DeleteMap ( PMAP_ITEM pMapItem )
{
	UnmapViewOfFile ( pMapItem->lpMapAddr ) ;
	CloseHandle ( pMapItem->hMapFile ) ;
	CloseHandle ( pMapItem->hFile ) ;
}

BOOL  CPEInfo::FlushData ( LPVOID lpAddr, UINT nNumToFlush )
{
	return FlushViewOfFile ( lpAddr, nNumToFlush ) ;
}

BOOL  CPEInfo::IsValidPE		( PMAP_ITEM pMapItem )
{
	return TRUE ;
}
 
VOID  CPEInfo::GetBaseInfo		( PMAP_ITEM pMapItem )
{
	PIMAGE_DOS_HEADER	pDosHeader	= (PIMAGE_DOS_HEADER)(pMapItem->lpMapAddr) ;
	PIMAGE_NT_HEADERS	pNtHeader	= (PIMAGE_NT_HEADERS)((LONG)(pMapItem->lpMapAddr)+ (pDosHeader->e_lfanew)) ;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNtHeader->OptionalHeader) ;

	this->BaseInfo.dwSectionAlign	= pOptionalHeader->SectionAlignment ;
	this->BaseInfo.dwFileAlign		= pOptionalHeader->FileAlignment ;
	this->BaseInfo.dwSectionNum		= pNtHeader->FileHeader.NumberOfSections ;
	this->BaseInfo.dwHeadSize		= pOptionalHeader->SizeOfHeaders ;
	this->BaseInfo.dwSectionBase	= pDosHeader->e_lfanew + 0x18 + pNtHeader->FileHeader.SizeOfOptionalHeader ;
	this->BaseInfo.dwRealHeadSize	= this->BaseInfo.dwSectionBase + this->BaseInfo.dwSectionNum * 0x28 ;
	this->BaseInfo.dwImageSize		= pOptionalHeader->SizeOfImage ;

	PIMAGE_SECTION_HEADER LastSec	= (PIMAGE_SECTION_HEADER)((LONG)(pMapItem->lpMapAddr) + this->BaseInfo.dwSectionBase + 0x28 * ( this->BaseInfo.dwSectionNum - 1 ) ) ;
	this->BaseInfo.dwFileSize		= LastSec->PointerToRawData + \
		this->FormatInt ( LastSec->SizeOfRawData, pOptionalHeader->FileAlignment ) ;

}

VOID  CPEInfo::InitNewSection	()
{
	strcpy ( (char*)this->NewSection.Name, ".NewSec" ) ;
	this->NewSection.Misc.VirtualSize	= this->BaseInfo.dwSectionAlign ;
	this->NewSection.VirtualAddress		= this->BaseInfo.dwImageSize ;
	this->NewSection.SizeOfRawData		= this->BaseInfo.dwFileAlign ;
	this->NewSection.PointerToRawData	= this->BaseInfo.dwFileSize ;
	this->NewSection.Characteristics	= 0xE0000020 ;
}

DWORD CPEInfo::FormatInt ( DWORD dwValue, DWORD dwAlign )
{
	if ( dwValue % dwAlign )
		return ( dwValue / dwAlign + 1 ) * dwAlign ;
	return dwValue / dwAlign * dwAlign ;
}

void CPEInfo::AdjustSectionSize ()
{
	if ( this->NewSection.Misc.VirtualSize < this->NewSection.SizeOfRawData )
		this->NewSection.Misc.VirtualSize = this->NewSection.SizeOfRawData ;

	if ( this->NewSection.Misc.VirtualSize % this->BaseInfo.dwSectionAlign )
		this->NewSection.Misc.VirtualSize = this->NewSection.Misc.VirtualSize / this->BaseInfo.dwSectionAlign + 1 ;
	else
		this->NewSection.Misc.VirtualSize = this->NewSection.Misc.VirtualSize / this->BaseInfo.dwSectionAlign ;
	this->NewSection.Misc.VirtualSize *= this->BaseInfo.dwSectionAlign ;

	if ( this->NewSection.SizeOfRawData % this->BaseInfo.dwFileAlign )
		this->NewSection.SizeOfRawData = this->NewSection.SizeOfRawData / this->BaseInfo.dwFileAlign + 1 ;
	else
		this->NewSection.SizeOfRawData = this->NewSection.SizeOfRawData / this->BaseInfo.dwFileAlign ;
	this->NewSection.SizeOfRawData *= this->BaseInfo.dwFileAlign ;
}

void CPEInfo::AddNewSection ( PMAP_ITEM pOldMap, PMAP_ITEM pNewMap, DWORD dwNewFileSize )
{
	memcpy ( LPVOID(pNewMap->lpMapAddr), LPVOID(pOldMap->lpMapAddr), this->NewSection.PointerToRawData ) ;

	// Fix PE Head's some item
	PIMAGE_DOS_HEADER	pDosHeader	= (PIMAGE_DOS_HEADER)(pNewMap->lpMapAddr) ;
	PIMAGE_NT_HEADERS	pNtHeader	= (PIMAGE_NT_HEADERS)((LONG)(pNewMap->lpMapAddr)+ (pDosHeader->e_lfanew)) ;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNtHeader->OptionalHeader) ; 

	// Increase section numbers
	pNtHeader->FileHeader.NumberOfSections ++ ;

	// Write into new IMAGE_SECTION_HEADER
	memcpy ( LPVOID(DWORD(pDosHeader)+this->BaseInfo.dwRealHeadSize), &(this->NewSection), 0x28 ) ;

	// Modify IMAGE SIZE
	pOptionalHeader->SizeOfImage += this->NewSection.Misc.VirtualSize ;

	this->FlushData ( pNewMap->lpMapAddr, dwNewFileSize ) ;
}