#include "StdAfx.h"
#include "PEInfo.h"

CPEInfo::CPEInfo(void)
{
	this->isReady = FALSE ;
	this->pDosHeader = NULL ;
	this->pNtHeader = NULL ;
	this->pOptionalHeader = NULL ;
	this->lpResBase = NULL ;
	memset ( &MapItem, 0, sizeof(MapItem) ) ;
}

CPEInfo::~CPEInfo(void)
{
}

BOOL CPEInfo::CreateMap	( TCHAR* pPathName) 
{	
	MapItem.hFile =	CreateFile ( 
		pPathName,					
		GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL ,
		NULL 
		);
	if ( MapItem.hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	//创建文件映射内核对象
	MapItem.hMapFile = CreateFileMapping ( MapItem.hFile, NULL, PAGE_READONLY, 0, 0, NULL	) ;
	if ( MapItem.hMapFile == NULL )
	{
		CloseHandle ( MapItem.hFile ) ;
		return FALSE;
	}
	
	//创建文件视图
	MapItem.lpMapAddr = (PBYTE)MapViewOfFile ( MapItem.hMapFile, FILE_MAP_READ, 0, 0, 0 ) ;
	if ( MapItem.lpMapAddr == NULL )
	{
		DWORD dwErrorCode = GetLastError () ;
		CloseHandle ( MapItem.hMapFile ) ;
		CloseHandle ( MapItem.hFile ) ;
		return FALSE;
	}

	this->isReady = TRUE ;
	return TRUE ;
}

void  CPEInfo::DeleteMap ()
{
	UnmapViewOfFile ( MapItem.lpMapAddr ) ;
	CloseHandle ( MapItem.hMapFile ) ;
	CloseHandle ( MapItem.hFile ) ;
	this->isReady = FALSE ;
}

BOOL  CPEInfo::FlushData ( LPVOID lpAddr, UINT nNumToFlush )
{
	return FlushViewOfFile ( lpAddr, nNumToFlush ) ;
}

VOID  CPEInfo::GetResInfo ()
{	
	memset ( &ResDirectory, 0, sizeof(ResDirectory) ) ;
	for ( int i = 0; i < MAX_RES_TYPES; i++ )
		this->ResList[i].clear() ;

	this->pDosHeader	= (PIMAGE_DOS_HEADER)MapItem.lpMapAddr ;
	this->pNtHeader		= (PIMAGE_NT_HEADERS)((DWORD)MapItem.lpMapAddr + this->pDosHeader->e_lfanew ) ;
	this->pOptionalHeader	= &(this->pNtHeader->OptionalHeader ) ;
	this->lpResBaseRva	= (LPVOID)this->pOptionalHeader->DataDirectory[2].VirtualAddress ;
	this->lpResBase		= ImageRvaToVa ( this->pNtHeader, this->MapItem.lpMapAddr, \
		this->pOptionalHeader->DataDirectory[2].VirtualAddress, NULL ) ;

	int i, j ;
	PIMAGE_RESOURCE_DIRECTORY pSubDir, pCurDir, pRootDir = (PIMAGE_RESOURCE_DIRECTORY)this->lpResBase ;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pSubEntry, pCurEntry, pRootEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY) \
		( (DWORD)pRootDir + sizeof(IMAGE_RESOURCE_DIRECTORY) ) ;
	this->dwNamedEntry	= pRootDir->NumberOfNamedEntries ;
	this->dwIdEntry		= pRootDir->NumberOfIdEntries ;
	DWORD dwTypeNum = pRootDir->NumberOfNamedEntries + pRootDir->NumberOfIdEntries ;
	for ( i = 0; i < dwTypeNum; i++ )
	{
		this->ResDirectory[i].dwNameOrId		= pRootEntry[i].Name ;
		this->ResDirectory[i].dwOffsetToData	= pRootEntry[i].OffsetToData ;

		pSubDir		= (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pRootDir + ( this->ResDirectory[i].dwOffsetToData & 0x7fffffff ) ) ;

		this->ResDirectory[i].wNumOfNamedEntry	= pSubDir->NumberOfNamedEntries ;
		this->ResDirectory[i].wNumOfIdEntry		= pSubDir->NumberOfIdEntries ;
		
		DWORD	dwResNum = pSubDir->NumberOfNamedEntries + pSubDir->NumberOfIdEntries ;
		pSubEntry	= (PIMAGE_RESOURCE_DIRECTORY_ENTRY) ( (DWORD)pSubDir + sizeof(IMAGE_RESOURCE_DIRECTORY) ) ;
		
		for ( j = 0; j < dwResNum; j++ )
		{
			RES_ITEM	CurResItem ;
			CurResItem.dwNameOrId		= pSubEntry[j].Name ;
			CurResItem.dwOffsetToData	= pSubEntry[j].OffsetToData ;

			pCurDir = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pRootDir + (pSubEntry[j].OffsetToData & 0x7fffffff ) ) ;

			CurResItem.wNumOfNamedEntry = pCurDir->NumberOfNamedEntries ;
			CurResItem.wNumOfIdEntry	= pCurDir->NumberOfIdEntries ;

			pCurEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)( (DWORD)pCurDir + sizeof(IMAGE_RESOURCE_DIRECTORY) ) ;

			PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)pRootDir+pCurEntry->OffsetToData) ;
			CurResItem.dwDataOffset	= pDataEntry->OffsetToData ;
			CurResItem.dwDataSize	= pDataEntry->Size ;

			ResList[i].push_back ( CurResItem ) ;
		}
	}
}

void CPEInfo::GetResItemByIndex ( int nRoot, int nSub, PRES_ITEM pResItem )
{
	list<RES_ITEM>::iterator p = this->ResList[nRoot-1].begin() ;
	for ( int i = 2; i <= nSub; i++ )
		p++ ;
	pResItem->dwDataOffset	= (*p).dwDataOffset ;
	pResItem->dwDataSize	= (*p).dwDataSize ;
}

BOOL CPEInfo::DumpResource ( LPTSTR lpTarFile, LPVOID lpResStart, DWORD dwResSize ) 
{
	HANDLE hFile =	CreateFile ( 
		lpTarFile,					
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL ,
		NULL 
		);
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	//创建文件映射内核对象
	HANDLE hMapFile = CreateFileMapping ( hFile, NULL, PAGE_READWRITE, 0, dwResSize, NULL	) ;
	if ( hMapFile == NULL )
	{
		CloseHandle (hFile ) ;
		return FALSE;
	}
	
	//创建文件视图
	LPVOID lpMapAddr = (PBYTE)MapViewOfFile ( hMapFile, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, dwResSize ) ;
	if ( lpMapAddr == NULL )
	{
		DWORD dwErrorCode = GetLastError () ;
		CloseHandle ( hMapFile ) ;
		CloseHandle ( hFile ) ;
		return FALSE;
	}

	memcpy ( lpMapAddr, lpResStart, dwResSize ) ;
	this->FlushData ( lpMapAddr, dwResSize ) ;
	UnmapViewOfFile ( lpMapAddr ) ;
	CloseHandle ( hMapFile ) ;
	CloseHandle ( hFile ) ;
	return TRUE ;
}