#include "StdAfx.h"
#include "LXWord.h"

#include <fstream>
#include <stdio.h>
#include <stdlib.h>

// 邻接表
static vector<ITEM>	WordVect[MAX_WORD_NUM] ;
// 临时存储结构，保存搜索结果，即联系字列表
static TEMP_ARRAY	TempArray[MAX_WORD_NUM] ;

CLXWord::CLXWord(void)
{
	this->pLXItem = NULL ;
	for ( int i = 0; i < MAX_WORD_NUM; i++ )
		WordVect[i].clear() ;
}

CLXWord::~CLXWord(void)
{
	if ( this->pLXItem != NULL )
		delete this->pLXItem ;
	for ( int i = 0; i < MAX_WORD_NUM; i++ )
		WordVect[i].clear() ;
}

BOOL CLXWord::LX_LoadDic ( LPSTR lpDicName )
{
	this->pLXItem = new LX_ITEM[MAX_LXWORD_NUM] ;
	if ( this->pLXItem == NULL )
		return FALSE ;

	FILE* pFile = fopen ( lpDicName, "r" ) ;
	if ( pFile == NULL )
		return FALSE ;

	UINT	nIndex = 0 ;
	char	szWord[64] ;
	while ( fscanf ( pFile, "%s %d", szWord, &this->pLXItem[nIndex].nFreq ) != EOF )
	{
		this->LX_BTW ( szWord, this->pLXItem[nIndex].CHWord ) ;

		WCHAR* pWord = this->pLXItem[nIndex].CHWord ;
		UINT nLen = (UINT)wcslen ( pWord ) ;

		for ( UINT i = 0; i < nLen-1; i++ )	// 除去最后一个字
			WordVect[pWord[i]].push_back ( ITEM(i,nIndex) ) ;

		nIndex ++ ;
	}

	fclose ( pFile ) ;
	return TRUE ;
}

BOOL CLXWord::LX_SearchWord ( LPWSTR lpWord, LPWSTR lpLXWord, UINT &nWordNum )
{
	nWordNum = 0 ;
	if ( lpWord == NULL )
		return FALSE ;

	UINT nWordLen	= (UINT)wcslen(lpWord) ;
	UINT nVertIndex = lpWord[nWordLen-1] ;
	UINT nLXWordNum = (UINT)WordVect[nVertIndex].size() ;
	if ( nLXWordNum == 0 )
		return TRUE ;

	UINT i, j ;
	for ( i = 0; i < MAX_WORD_NUM; i++ )
	{
		TempArray[i].nIndex = i ;
		TempArray[i].nFreq	= 0 ;
	}

	PITEM pItem = NULL ;
	for ( i = 0; i < nLXWordNum; i++ )
	{
		pItem = &WordVect[nVertIndex].at ( i ) ;

		LPWSTR	lpTempWord	= this->pLXItem[pItem->nLXWordIndex].CHWord ;
		UINT	nTempPos	= pItem->nMatchPos ;
		UINT	nTempFreq	= this->pLXItem[pItem->nLXWordIndex].nFreq ;
		UINT	nTempLen	= (UINT)wcslen ( lpTempWord ) ;
		UINT	nNextWordHash = lpTempWord[nTempPos+1] ;
		if ( nWordLen <= nTempPos )
			continue ;

		if ( nTempPos == 0 )
		{
			TempArray[nNextWordHash].nFreq += nTempFreq ;
			continue ;
		}

		for ( j = 0; j < nTempPos; j++ )
		{
			if ( lpTempWord[j] != lpWord[(nWordLen-1)-nTempPos+j] )
				break ;
		}
		if ( j >= nTempPos )
		{
			TempArray[nNextWordHash].nFreq += nTempFreq ;
		}
	}

	qsort ( TempArray, MAX_WORD_NUM, sizeof(TEMP_ARRAY), SortCmp ) ;
/*
	while ( nTempArray[nWordNum][1] > 0 )
	{
		lpLXWord[nWordNum] = this->LX_HashToWChar ( nTempArray[nWordNum][0] ) ;
		nWordNum++ ;		
	}
	*/

	for ( i = 0; i < MAX_WORD_NUM; i++ )
	{
		if ( TempArray[i].nFreq > 0 )
			lpLXWord[nWordNum++] = TempArray[i].nIndex ;
	}

	return TRUE ;
}

BOOL CLXWord::LX_BTW(LPSTR lpSrc,LPWSTR lpDst)
{
	__try{
		return MultiByteToWideChar( CP_ACP, 0, lpSrc, -1, lpDst, MAX_PATH) != 0;
	}
	__except(TRUE){
		return FALSE;
	}
}

BOOL CLXWord::LX_WTB(LPWSTR lpSrc,LPSTR lpDst)
{
	__try{
		return WideCharToMultiByte( CP_ACP, 0, lpSrc, -1, lpDst, MAX_PATH,NULL,NULL) != 0;
	}
	__except(TRUE){
		return FALSE;
	}
}

int SortCmp ( const void* a, const void* b )
{
	return ((PTEMP_ARRAY)b)->nFreq - ((PTEMP_ARRAY)a)->nFreq ;
}