#ifndef _DIR_MANAGER_H
#define _DIR_MANAGER_H

#include "Info.h"

class CDirManager {
public:
	LPVOID	lpBase ;
public:
	// 在指定的目录簇中添加目录项
	UINT AddDirectoryItem ( UINT nCluIndex, PDIRECTORY_ITEM pItem )
	{
		UINT nCurAddr = (UINT)lpBase + nCluIndex * CLUSTER_SIZE ;
		for ( UINT i = 0; i < MAX_DIR_ITEM_IN_PAGE; i++ )
		{
			if ( ((PBYTE)nCurAddr)[0] == 0xE5 || ((PBYTE)nCurAddr)[0] == 0 )
			{
				memcpy ( (LPVOID)nCurAddr, (LPVOID)pItem, DIRECTORY_ITEM_SIZE ) ;
				break ;
			}
			nCurAddr += DIRECTORY_ITEM_SIZE ;
		}
		return nCurAddr ;
	}

	// 在指定的目录簇中删除指定的文件目录项（同时可以返回该目录项）
	UINT DelDirectoryItem ( UINT nCluIndex, LPSTR lpDirName, PDIRECTORY_ITEM pItem = NULL )
	{
		UINT nCurAddr = (UINT)lpBase + nCluIndex * CLUSTER_SIZE ;
		for ( UINT i = 0; i < MAX_DIR_ITEM_IN_PAGE; i++ )
		{
			if ( memcmp ( (LPVOID)nCurAddr, lpDirName, strlen(lpDirName) ) == 0 )
			{
				if ( pItem != NULL )
					memcpy ( pItem, (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
				((PBYTE)nCurAddr)[0] = 0xE5 ;
				break ;
			}
			nCurAddr += DIRECTORY_ITEM_SIZE ;
		}
		return nCurAddr ;
	}

	// 在指定目录簇中取得文件目录项
	BOOL GetDirectoryItem ( UINT nCluIndex, LPSTR lpName, PDIRECTORY_ITEM pItem )
	{	
		UINT nCurAddr = (UINT)lpBase + nCluIndex * CLUSTER_SIZE ;
		for ( UINT i = 0; i < MAX_DIR_ITEM_IN_PAGE; i++ )
		{
			if ( memcmp ( (LPVOID)nCurAddr, lpName, strlen(lpName) ) == 0 )
			{
				memcpy ( pItem, (LPVOID)nCurAddr, DIRECTORY_ITEM_SIZE ) ;
				break ;
			}
			nCurAddr += DIRECTORY_ITEM_SIZE ;
		}
		return TRUE ;
	}

	// 检测该目录项是否属于文件
	BOOL IsDirItemFile ( PDIRECTORY_ITEM pItem )
	{
		return ( pItem->bAttribute & 0x3 ) ? TRUE : FALSE ;
	}

	// 检测该目录项是否属于子目录
	BOOL IsDirItemDir ( PDIRECTORY_ITEM pItem ) 
	{
		return ( pItem->bAttribute & 0x4 ) ? TRUE : FALSE ;
	}
} ;

#endif