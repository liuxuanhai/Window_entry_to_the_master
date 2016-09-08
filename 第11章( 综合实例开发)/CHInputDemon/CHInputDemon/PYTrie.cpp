#include "StdAfx.h"
#include "PYTrie.h"

#include <stdio.h>
#include <fstream>

CPYTrie::CPYTrie(void)
{
	for ( int i = 0; i < MAX_PY_NUM; i++ )
		this->WordVect[i].clear() ;
}

CPYTrie::~CPYTrie(void)
{
}

BOOL CPYTrie::PYT_LoadDic ( LPSTR lpDicName )
{
	FILE* pFile = fopen ( lpDicName, "r" ) ;
	if ( pFile == NULL )
		return FALSE ;

	UINT nCount = 0 ;
	char szPY[MIN_BUF_SIZE], szWordA[MAX_BUF_SIZE] ;
	WCHAR szWordW[MAX_BUF_SIZE] = {0} ;

	while ( fscanf ( pFile, "%s %s", szPY, szWordA ) != EOF )
	{
		this->BTW ( szWordA, szWordW ) ;

		UINT uLen = (UINT)wcslen ( szWordW ) ;
		for ( UINT i = 0; i < uLen; i++ )
			this->WordVect[nCount].push_back ( szWordW[i] ) ;
		this->PYT_AddItem ( szPY, nCount ) ;
		nCount++ ;
	}

	fclose ( pFile ) ;
	return TRUE ;
}

VOID CPYTrie::PYT_AddItem(LPSTR lpPY, UINT nIndex )
{
	int nLen = (int)strlen ( lpPY ) ;
	PNODE pCurNode = &this->RootNode ;
	for ( int i = 0; i < nLen; i++ )
	{
		if ( pCurNode->lpNode[lpPY[i]-'a'] == NULL )
			pCurNode->lpNode[lpPY[i]-'a'] = new NODE ;
		pCurNode = pCurNode->lpNode[lpPY[i]-'a'] ;
	}
	pCurNode->nWordIndex = nIndex ;
}

BOOL CPYTrie::PYT_SearchWord ( LPSTR lpPY, LPWSTR lpWord, UINT &nWordNum )
{
	if ( this->PYT_IsValidPY ( lpPY ) == FALSE )
		return FALSE ;

	int nLen = (int)strlen ( lpPY ) ;
	PNODE pCurNode = &this->RootNode ;
	for ( int i = 0; i < nLen ; i++ )
	{
		if ( pCurNode->lpNode[lpPY[i]-'a'] == NULL )
			return FALSE ;
		pCurNode = pCurNode->lpNode[lpPY[i]-'a'] ;
	}

	vector<WCHAR>	SearchResultWords ;
	vector<UINT>	SearchResultIndex ;
	SearchResultWords.clear () ;
	SearchResultIndex.clear () ;
	list<PNODE> Queue ;		// ¹ãËÑ¸¨Öú¶ÓÁÐ
	Queue.clear() ;
	Queue.push_back ( pCurNode ) ;
	while ( !Queue.empty() )
	{
		PNODE pHead = Queue.front() ;
		Queue.pop_front() ;

		if ( pHead->nWordIndex >= 0 )
			SearchResultIndex.push_back ( pHead->nWordIndex ) ;

		for ( int i = 0; i < 26; i++ )
		{
			if ( pHead->lpNode[i] )
				Queue.push_back ( pHead->lpNode[i] ) ;
		}
	}

	nWordNum = 0 ;
	vector<UINT>::iterator pIndex ;
	for ( pIndex = SearchResultIndex.begin(); pIndex != SearchResultIndex.end(); pIndex++ )
		nWordNum += (UINT)this->WordVect[*pIndex].size() ;

	UINT nIndex = 0 ;
	memset ( lpWord, 0, sizeof(lpWord) ) ;
	for ( pIndex = SearchResultIndex.begin(); pIndex != SearchResultIndex.end(); pIndex++ )
	{
		vector<WCHAR>::iterator p ;
		for ( p = this->WordVect[*pIndex].begin(); p != this->WordVect[*pIndex].end(); p++ )
			lpWord[nIndex++] = *p ;
	}

	return TRUE ;
}

BOOL CPYTrie::BTW(LPSTR lpSrc,LPWSTR lpDst)
{
	__try{
		return MultiByteToWideChar( CP_ACP, 0, lpSrc, -1, lpDst, MAX_PATH) != 0;
	}
	__except(TRUE){
		return FALSE;
	}
}

BOOL CPYTrie::WTB(LPWSTR lpSrc,LPSTR lpDst)
{
	__try{
		return WideCharToMultiByte( CP_ACP, 0, lpSrc, -1, lpDst, MAX_PATH,NULL,NULL) != 0;
	}
	__except(TRUE){
		return FALSE;
	}
}

BOOL CPYTrie::PYT_IsValidPY ( LPSTR lpPY )
{
	int nLen = (int)strlen(lpPY) ;
	for ( int i = 0; i < nLen; i++ )
	{
		if ( lpPY[i] < 'a' || lpPY[i] > 'z' )
			return FALSE ;
	}
	return TRUE ;
}