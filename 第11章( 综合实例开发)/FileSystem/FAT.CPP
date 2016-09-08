#include "stdafx.h"
#include "FAT.h"

CFAT::CFAT()
{
	nCluPos = FIRST_VALID_CLUSTER ;
	lpBase	= 0 ;
}

CFAT::~CFAT()	
{}

UINT CFAT::GetIdleCluster ()
{
	while ( ++nCluPos )
	{
		if ( nCluPos == 0xFFFF )
			nCluPos = FIRST_VALID_CLUSTER ;
		UINT nCluAddr = GetCluPointer ( nCluPos ) ;
		if ( ((PBYTE)nCluAddr)[0] == 0 && ((PBYTE)nCluAddr)[1] == 0 )
			break ;
	}
	return nCluPos ;
}

void CFAT::SetIdleCluster	( UINT nCluIndex )
{
	UINT nCluAddr = GetCluPointer ( nCluIndex ) ;
	((PBYTE)nCluAddr)[0] = ((PBYTE)nCluAddr)[1] = 0 ;
}

UINT CFAT::GetNextCluster ( UINT nCluIndex )
{
	UINT nCluAddr = GetCluPointer ( nCluIndex ) ;
	return ( ( ((PBYTE)nCluAddr)[1] << 8 ) + ((PBYTE)nCluAddr)[0] ) ;
}

void CFAT::SetNextCluster	( UINT nCluIndex, UINT nNewCluIndex )
{
	UINT nCluAddr = GetCluPointer ( nCluIndex ) ;
	((PBYTE)nCluAddr)[0] = ((PBYTE)(&nNewCluIndex))[0] ;
	((PBYTE)nCluAddr)[1] = ((PBYTE)(&nNewCluIndex))[1] ;
}

UINT CFAT::GetCluPointer ( UINT nCluIndex )
{
	return ( (UINT)lpBase + nCluIndex * FAT_BIT16 + 1 * CLUSTER_SIZE ) ;
} 

UINT CFAT::GetAddByIndex ( UINT nCluIndex )
{
	return ( (UINT)lpBase + nCluIndex * CLUSTER_SIZE ) ;
} 

UINT CFAT::GetCluIndexByPathName ( LPSTR lpPathName ) 
{
	UINT nBeg = 9, nEnd = 9, nCluIndex = FIRST_VALID_CLUSTER ;
	while ( nEnd <= strlen(lpPathName) && lpPathName[nEnd] != '\0' )
	{
		while ( lpPathName[nEnd] != '\\' )
		{
			nEnd++ ;
		}
	
		BYTE bTarName[8] = {0} ;
		memcpy ( bTarName, &(lpPathName[nBeg]), nEnd - nBeg ) ;
		
		UINT nTempAddr, nAddr = GetAddByIndex ( nCluIndex ) ;
		for ( UINT i = 0; i < MAX_DIR_ITEM_IN_PAGE; i++ )
		{
			nTempAddr = nAddr + DIRECTORY_ITEM_SIZE * i ;
			if ( memcmp ( bTarName, (LPVOID)nTempAddr, 8 ) == 0 ) 
				break ;
		}

		nCluIndex = ((PDIRECTORY_ITEM)nTempAddr)->nStartClu ;
		nBeg = nEnd = nEnd + 1 ;
	}
	return nCluIndex ;
}

BOOL CFAT::IsTailCluster	( UINT nCluIndex )
{
	UINT nCurAddr = this->GetCluPointer ( nCluIndex ) ;
	if ( ((PBYTE)nCurAddr)[0] == 0xFF && ((PBYTE)nCurAddr)[1] == 0xFF )
		return TRUE ;
	return FALSE ;
}