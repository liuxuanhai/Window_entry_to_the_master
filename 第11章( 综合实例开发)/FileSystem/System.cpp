#include "stdafx.h"
#include "System.h"

 
BOOL CSystem::SysInstall () 
{
	if ( !this->SysCreateMap ( CREATE_ALWAYS ) )
		return FALSE ;
	if ( !this->SysInitial () )
		return FALSE ;
	return TRUE ;
}

BOOL CSystem::SysUnstall ()
{
	SysDeleteMap () ;
	return DeleteFile ( SYSTEM_SPACE_NAME ) ;
}

BOOL CSystem::SysInitial () 
{
	//
	// 建立根目录
	//
	DIRECTORY_ITEM	DirItem ;
	ZeroMemory ( &DirItem, sizeof(DIRECTORY_ITEM) ) ;
	memcpy ( DirItem.szFileName, "system", 6 ) ;
	DirItem.szFileExt[0]	= 0 ;
	DirItem.bAttribute		= 0x4 ;
	DirItem.nStartClu		= FIRST_VALID_CLUSTER ;
	DirItem.nFileSize		= 0 ;
	memcpy ( DirItem.szUserName, "root", 4 ) ;

	// 设置后继簇
	FATManager.SetNextCluster ( DirItem.nStartClu, 0xFFFF ) ;
	this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(DirItem.nStartClu), 2 ) ;

	// 把根目录信息COPY到文件系统起始位置
	memcpy ( SysHandle.lpFileMapping, &DirItem, DIRECTORY_ITEM_SIZE ) ;

	//
	// 建立默认用户
	//
	USER_INFO UserItem ;
	ZeroMemory ( &UserItem, sizeof(USER_INFO) ) ;
	memcpy ( UserItem.szUserName,	"root", 4 ) ;
	memcpy ( UserItem.szUserPwd,	"root", 4 ) ;

	// 把用户信息表放在根目录后
	memcpy ( LPVOID(DWORD(SysHandle.lpFileMapping)+DIRECTORY_ITEM_SIZE), &UserItem, USER_INFO_SIZE ) ;

	// 刷新内存修改到磁盘文件
	this->SysFlushData ( SysHandle.lpFileMapping, DIRECTORY_ITEM_SIZE + USER_INFO_SIZE ) ;

	//
	// 在根目录中建立子目录"."和".."
	//
	DIRECTORY_ITEM	SubDirItemA, SubDirItemB ;
	ZeroMemory ( &SubDirItemA, sizeof(DIRECTORY_ITEM) ) ;
	ZeroMemory ( &SubDirItemB, sizeof(DIRECTORY_ITEM) ) ;
	SubDirItemA.szFileName[0] = '.' ;
	SubDirItemB.szFileName[0] = SubDirItemB.szFileName[1] = '.' ; 
	memcpy ( SubDirItemA.szUserName, "all", 3 ) ;
	memcpy ( SubDirItemB.szUserName, "all", 3 ) ;

	UINT nAddr = FATManager.GetAddByIndex ( DirItem.nStartClu ) ;
	memcpy ( (LPVOID)nAddr, &SubDirItemA, DIRECTORY_ITEM_SIZE ) ;
	memcpy ( (LPVOID)(nAddr+DIRECTORY_ITEM_SIZE), &SubDirItemB, DIRECTORY_ITEM_SIZE ) ;

	// 刷新子目录信息
	this->SysFlushData ( (LPVOID)nAddr, 2 * DIRECTORY_ITEM_SIZE ) ;

	return TRUE ;
}

UINT CSystem::SysGetUserInfo ( PUSER_INFO pItem, UINT nCurAddr = DIRECTORY_ITEM_SIZE ) 
{
	if ( nCurAddr == DIRECTORY_ITEM_SIZE )
		nCurAddr += (UINT)SysHandle.lpFileMapping ;

	if ( ((PBYTE)nCurAddr)[0] == 0 )
		return 0 ;

	while ( ((PBYTE)nCurAddr)[0] == 0xE5 )
		nCurAddr += USER_INFO_SIZE ;

	memcpy ( pItem, (LPVOID)nCurAddr, USER_INFO_SIZE ) ;
	return ( nCurAddr + USER_INFO_SIZE ) ;
}

BOOL CSystem::SysIsUserExist ( LPSTR lpUserName )
{
	USER_INFO Item ;
	UINT nCurAddr = this->SysGetUserInfo ( &Item ) ;
	while ( nCurAddr != 0 )
	{
		if ( !strcmp( (char*)Item.szUserName, lpUserName ) )
		{
			return TRUE ;
		}
		nCurAddr = this->SysGetUserInfo ( &Item, nCurAddr ) ;
	}

	return FALSE ;
}

BOOL CSystem::SysAddUserInfo ( LPSTR lpUserName, LPSTR lpUserPwd )
{
	USER_INFO UserItem ;
	ZeroMemory ( &UserItem, USER_INFO_SIZE ) ;
	memcpy ( &UserItem.szUserName,	lpUserName, strlen( lpUserName ) ) ;
	memcpy ( &UserItem.szUserPwd,	lpUserPwd,	strlen( lpUserPwd ) ) ;

	UINT nCurAddr = (UINT)SysHandle.lpFileMapping + DIRECTORY_ITEM_SIZE ;
	while ( ((PBYTE)nCurAddr)[0] != 0 && ((PBYTE)nCurAddr)[0] != 0xE5 )
		nCurAddr += USER_INFO_SIZE ;

	memcpy ( (LPVOID)nCurAddr, &UserItem, USER_INFO_SIZE ) ;
	this->SysFlushData ( (LPVOID)nCurAddr, USER_INFO_SIZE ) ;
	return TRUE;
}

BOOL CSystem::SysDeleteUser	( LPSTR lpUserName )
{
	USER_INFO Item ;
	UINT nCurAddr = this->SysGetUserInfo ( &Item ) ;
	while ( nCurAddr != 0 )
	{
		if ( !strcmp( (char*)Item.szUserName, lpUserName ) )
		{
			((PBYTE)(nCurAddr-USER_INFO_SIZE))[0] = 0XE5 ;
			this->SysFlushData ( (LPVOID)(nCurAddr-USER_INFO_SIZE), 1 ) ;
			return TRUE ;
		}
		nCurAddr = this->SysGetUserInfo ( &Item, nCurAddr ) ;
	}

	return FALSE ;
}

BOOL CSystem::SysCheckLogInfo ( LPSTR lpUserName, LPSTR lpUserPwd )
{
	USER_INFO Item ;
	UINT nCurAddr = this->SysGetUserInfo ( &Item ) ;
	while ( nCurAddr != 0 )
	{
		if ( !memcmp( (char*)Item.szUserName, lpUserName,	strlen(lpUserName) ) && \
			 !memcmp( (char*)Item.szUserPwd,  lpUserPwd,	strlen(lpUserPwd) ) )
		{ 
			this->lpCurUser = lpUserName ;
			return TRUE ;
		}
		nCurAddr = this->SysGetUserInfo ( &Item, nCurAddr ) ;
	}

	this->lpCurUser = NULL ;
	return FALSE ;
}

BOOL CSystem::SysCheckUserPow	( LPSTR lpPath, LPSTR lpName )
{
	DIRECTORY_ITEM Item ;
	UINT nCluIndex = FATManager.GetCluIndexByPathName ( lpPath ) ;
	DirManager.GetDirectoryItem ( nCluIndex, lpName, &Item ) ;

	if ( memcmp ( this->lpCurUser, Item.szUserName, strlen(this->lpCurUser) ) )
		return FALSE ;
	return TRUE ;
}

UINT CSystem::SysFirstEnumDir	( LPSTR lpPath, PDIRECTORY_ITEM pItem ) 
{
	UINT nCluIndex	= FATManager.GetCluIndexByPathName ( lpPath ) ;
	UINT nCurAddr	= (UINT)(SysHandle.lpFileMapping) + CLUSTER_SIZE * nCluIndex ;
	memcpy ( pItem, (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
	return ( nCurAddr + DIRECTORY_ITEM_SIZE ) ;
}

UINT CSystem::SysNextEnumDir	( UINT nCurAddr, PDIRECTORY_ITEM pItem )
{
	while ( ((PBYTE)nCurAddr)[0] == 0XE5 )
		nCurAddr += DIRECTORY_ITEM_SIZE ;
	if ( ((PBYTE)nCurAddr)[0] == 0 )
		return 0 ;
	memcpy ( pItem, (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
	return ( nCurAddr + DIRECTORY_ITEM_SIZE ) ;
}


BOOL CSystem::SysIsFileExist ( LPSTR lpPath, LPSTR lpName )
{	
	DIRECTORY_ITEM Item ;
	UINT nCurAddr = this->SysFirstEnumDir ( lpPath, &Item ) ;
	while ( nCurAddr != 0 )
	{
		if ( !memcmp ( Item.szFileName, lpName, strlen(lpName) ) )
			return TRUE ;

		nCurAddr = this->SysNextEnumDir ( nCurAddr, &Item ) ;
	}

	return FALSE ;
}

BOOL CSystem::SysCreateFile	( LPSTR lpPath, LPSTR lpName, LPSTR lpExt, LPSTR lpContent )
{
	DIRECTORY_ITEM Item ;
	ZeroMemory ( &Item, DIRECTORY_ITEM_SIZE ) ;
	memcpy ( Item.szFileName, lpName, strlen(lpName) ) ;
	memcpy ( Item.szFileExt,  lpExt, strlen(lpExt) ) ;
	Item.bAttribute	= 0x2 ;
	Item.nStartClu	= FATManager.GetIdleCluster () ;
	Item.nFileSize	= strlen ( lpContent ) ;
	memcpy ( Item.szUserName, this->lpCurUser, strlen(lpCurUser) ) ;

	UINT nCluIndex	= FATManager.GetCluIndexByPathName ( lpPath ) ;
	UINT nCurAddr	= DirManager.AddDirectoryItem ( nCluIndex, &Item ) ;
	if ( !this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) )
		return FALSE ;

	UINT	nCurPos = 0 ;
	UINT	nContentLength = Item.nFileSize ;

	nCluIndex = Item.nStartClu ;
	FATManager.SetNextCluster ( nCluIndex, 0xFFFF ) ;
	this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nCluIndex), 2 ) ;

	while ( true )
	{
		UINT nCurAddr = FATManager.GetAddByIndex ( nCluIndex ) ;

		if ( nContentLength - nCurPos >= CLUSTER_SIZE )
		{
			memcpy ( (LPVOID)nCurAddr, lpContent, CLUSTER_SIZE ) ;
			this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
		}
		else
		{
			memcpy ( (LPVOID)nCurAddr, lpContent, CLUSTER_SIZE ) ;
			this->SysFlushData ( (LPVOID)nCurAddr, nContentLength - nCurPos ) ;
		}

		nCurPos += CLUSTER_SIZE ;

		if ( nContentLength > nCurPos )
		{
			UINT nNewCluIndex = FATManager.GetIdleCluster () ;
			FATManager.SetNextCluster ( nNewCluIndex, 0xFFFF ) ;
			this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nNewCluIndex), 2 ) ;
			
			FATManager.SetNextCluster ( nCluIndex, nNewCluIndex ) ;	
			this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nCluIndex), 2 ) ;	
			
			nCluIndex	= nNewCluIndex ;
		}
		else
			break ;
	}

	return TRUE ;
}

BOOL CSystem::SysDeleteFile	( LPSTR lpPath, LPSTR lpName )
{
	DIRECTORY_ITEM Item ;
	UINT nCluIndex	= FATManager.GetCluIndexByPathName ( lpPath ) ;
	UINT nCurAddr	= DirManager.DelDirectoryItem ( nCluIndex, lpName, &Item ) ;
	if ( !this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) )
		return FALSE ;
	
	nCluIndex = Item.nStartClu ;
	UINT nNewCluIndex  ;
	while ( true )
	{
		if ( FATManager.IsTailCluster ( nCluIndex ) )
		{
			FATManager.SetIdleCluster ( nCluIndex ) ;
			this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nCluIndex), 2 ) ;
			break ;
		}
		else
		{
			nNewCluIndex = FATManager.GetNextCluster ( nCluIndex ) ;
			FATManager.SetIdleCluster ( nCluIndex ) ;
			this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nCluIndex), 2 ) ;
			nCluIndex	= nNewCluIndex ;
		}
	}

	return TRUE ;
}

CString CSystem::SysReadFile ( LPSTR lpPath, LPSTR lpName )
{
	UINT nCluIndex = FATManager.GetCluIndexByPathName ( lpPath ) ;
	
	DIRECTORY_ITEM Item ;
	DirManager.GetDirectoryItem ( nCluIndex, lpName, &Item ) ;

	UINT nCurLength = 0, nConLength = Item.nFileSize ;
	nCluIndex = Item.nStartClu ;

	CString szContent, TempStr ;
	while ( true )
	{
		BYTE byte[CLUSTER_SIZE+1] = {0} ;
		UINT nCurAddr = FATManager.GetAddByIndex ( nCluIndex ) ;

		if ( nConLength - nCurLength >= CLUSTER_SIZE )
		{
			memset ( byte, 0, sizeof(byte) ) ;
			memcpy ( byte, (LPVOID)nCurAddr, CLUSTER_SIZE ) ;	
			TempStr.Format ( "%s", byte ) ;
			szContent += TempStr ;
		}
		else 
		{
			memset ( byte, 0, sizeof(byte) ) ;
			memcpy ( byte, (LPVOID)nCurAddr, nConLength-nCurLength ) ;	
			TempStr.Format ( "%s", byte ) ;
			szContent += TempStr ;
		}

		nCurLength += CLUSTER_SIZE ;
		if ( nCurLength >= nConLength )
			break ;
		else
		{
			nCluIndex = FATManager.GetNextCluster ( nCluIndex ) ;
		}
	}

	return szContent ;
}

BOOL CSystem::SysCreateSubDir	( LPSTR lpPath, LPSTR lpSubDirName )
{
	//
	// 构造文件目录项
	//
	DIRECTORY_ITEM NewItem ;
	ZeroMemory ( &NewItem, DIRECTORY_ITEM_SIZE ) ;
	memcpy ( NewItem.szFileName, lpSubDirName, strlen ( lpSubDirName ) ) ;
	NewItem.bAttribute	= 0X4 ;
	NewItem.nStartClu	= FATManager.GetIdleCluster () ;
	memcpy ( NewItem.szUserName, this->lpCurUser, strlen(this->lpCurUser) ) ;
	
	// 设置默认后继簇
	FATManager.SetNextCluster ( NewItem.nStartClu, 0xFFFF ) ;
	this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(NewItem.nStartClu), 2 ) ;

	//
	// 在lpPath路径目录里新建子目录
	//
	UINT nCluIndex	= FATManager.GetCluIndexByPathName ( lpPath ) ;
	UINT nCurAddr	= DirManager.AddDirectoryItem ( nCluIndex, &NewItem ) ;

	// 刷新目录项
	if ( !this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) )
		return FALSE ;

	//
	// 添加"."和".."子目录
	//
	DIRECTORY_ITEM	SubDirItemA, SubDirItemB ;
	ZeroMemory ( &SubDirItemA, sizeof(DIRECTORY_ITEM) ) ;
	ZeroMemory ( &SubDirItemB, sizeof(DIRECTORY_ITEM) ) ;
	SubDirItemA.szFileName[0] = '.' ;
	SubDirItemB.szFileName[0] = SubDirItemB.szFileName[1] = '.' ; 
	memcpy ( SubDirItemA.szUserName, this->lpCurUser, strlen(lpCurUser) ) ;
	memcpy ( SubDirItemB.szUserName, this->lpCurUser, strlen(lpCurUser) ) ;

	nCurAddr = DirManager.AddDirectoryItem ( NewItem.nStartClu, &SubDirItemA ) ;
	this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;

	nCurAddr = DirManager.AddDirectoryItem ( NewItem.nStartClu, &SubDirItemB ) ;
	this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;

	return TRUE ;
}

BOOL CSystem::SysDeleteSubDir	( LPSTR lpPath, LPSTR lpSubDirName )
{
	// 子目录的路径
	char szPath[MAX_PATH_LENGTH] = {0} ;
	wsprintf ( szPath, "%s%s\\", lpPath, lpSubDirName ) ;

	UINT nCurAddr = 0, nCluIndex = FATManager.GetCluIndexByPathName ( szPath ) ;
	FATManager.SetIdleCluster ( nCluIndex ) ;
	this->SysFlushData ( (LPVOID)FATManager.GetCluPointer(nCluIndex), 2 ) ;

	DIRECTORY_ITEM Item ;
	nCurAddr = this->SysFirstEnumDir ( szPath, &Item ) ;

	while ( nCurAddr != 0 )
	{
		char szName[9] = {0} ;
		memcpy ( szName, &Item, 8 ) ;

		if ( DirManager.IsDirItemFile ( &Item ) )
			this->SysDeleteFile ( szPath, szName ) ;
		else if ( DirManager.IsDirItemDir ( &Item ) )
			this->SysDeleteSubDir ( szPath, szName ) ;
		else 
		{
			((PBYTE)(nCurAddr-DIRECTORY_ITEM_SIZE))[0] = 0XE5 ;
			this->SysFlushData ( (LPVOID)(nCurAddr-DIRECTORY_ITEM_SIZE), 1 ) ;
		}

		nCurAddr = this->SysNextEnumDir ( nCurAddr, &Item ) ;
	}

	nCluIndex	= FATManager.GetCluIndexByPathName ( lpPath ) ;
	nCurAddr	= DirManager.DelDirectoryItem ( nCluIndex, lpSubDirName ) ;
	return this->SysFlushData ( (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
}

BOOL CSystem::SysLowFormat	()
{
	ZeroMemory ( SysHandle.lpFileMapping, DEFAULT_SYSTEM_SPACE ) ;
	if ( !this->SysInitial () )
		return FALSE ;
	return this->SysFlushData ( SysHandle.lpFileMapping, DEFAULT_SYSTEM_SPACE ) ;
}

BOOL CSystem::SysHighFormat	()
{
	ZeroMemory ( SysHandle.lpFileMapping, FIRST_VALID_CLUSTER ) ;
	if ( !this->SysInitial () )
		return FALSE ;
	return this->SysFlushData ( SysHandle.lpFileMapping, FIRST_VALID_CLUSTER ) ;
}

BOOL CSystem::SysCreateMap	( DWORD dwCreateFlag ) 
{	
	SysHandle.hFile =	CreateFile ( 
		SYSTEM_SPACE_NAME,					
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,  
		NULL, 
		dwCreateFlag, 
		FILE_ATTRIBUTE_NORMAL ,
		NULL 
		);
	if ( SysHandle.hFile == INVALID_HANDLE_VALUE )
	{
		return FALSE;
	}
	
	//创建文件映射内核对象
	SysHandle.hMapFile = CreateFileMapping ( SysHandle.hFile, NULL, \
							PAGE_READWRITE, 0, DEFAULT_SYSTEM_SPACE, NULL	) ;
	if ( SysHandle.hMapFile == NULL )
	{
		CloseHandle ( SysHandle.hFile ) ;
		return FALSE;
	}
	
	//创建文件视图
	SysHandle.lpFileMapping = (PBYTE)MapViewOfFile ( SysHandle.hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) ;
	if ( SysHandle.lpFileMapping == NULL )
	{
		DWORD dwErrorCode = GetLastError () ;
		CloseHandle ( SysHandle.hMapFile ) ;
		CloseHandle ( SysHandle.hFile ) ;
		return FALSE;
	}

	FATManager.lpBase	= SysHandle.lpFileMapping ;
	DirManager.lpBase	= SysHandle.lpFileMapping ;
	
	return TRUE ;
}

void  CSystem::SysDeleteMap	 ()
{
	UnmapViewOfFile ( SysHandle.lpFileMapping ) ;
	CloseHandle ( SysHandle.hMapFile ) ;
	CloseHandle ( SysHandle.hFile ) ;
}

BOOL  CSystem::SysFlushData ( LPVOID lpAddr, UINT nNumToFlush )
{
	return FlushViewOfFile ( lpAddr, nNumToFlush ) ;
}