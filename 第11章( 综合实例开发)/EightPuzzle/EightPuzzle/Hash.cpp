#include "StdAfx.h"
#include "Hash.h"

CHash::CHash(void)
{
	memset ( this->nCount, 0, sizeof(this->nCount) ) ;

	for ( int i = 0; i < MAX_HASH_RANGE; i++ )
	{
		this->pHashTable[i] = new int [MAX_CRASH_NUM] ;
	}
}

CHash::~CHash(void)
{
	for ( int i = 0; i < MAX_HASH_RANGE; i++ )
	{
		delete this->pHashTable[i] ;
	}
}

BOOL CHash::IsValueExist ( int nValue )
{
	int nHashValue = nValue % MAX_HASH_RANGE ;
	for ( int i = 0; i < this->nCount[nHashValue]; i++ )
	{
		if ( this->pHashTable[nHashValue][i] == nValue )
			return TRUE ;
	}
	return FALSE ;
}

VOID CHash::AddValue ( int nValue )
{
	if ( this->IsValueExist(nValue ) == TRUE )
		return ;

	int nHashValue = nValue % MAX_HASH_RANGE ;
	this->pHashTable[nHashValue][this->nCount[nHashValue]++] = nValue ;
}
