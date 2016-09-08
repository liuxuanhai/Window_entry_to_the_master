#pragma once

#include <windows.h>
#include <vector>
#include <list>
#include <string>
using namespace std ;

#define MIN_BUF_SIZE	32
#define MAX_BUF_SIZE	2048
#define MAX_PY_NUM		512

typedef struct _NODE {
	struct _NODE*	lpNode[26] ;	
	int				nWordIndex ;	// 中文字列表索引
	struct _NODE () {
		nWordIndex = -1 ;
		for ( int i = 0; i <  26; i++ )
			lpNode[i] = NULL ;
	}
} NODE, *PNODE ;


class CPYTrie
{
private:
	NODE			RootNode ;				// Trie树根结点
	vector<WCHAR>	WordVect[MAX_PY_NUM] ;	// 存储中文字列表

public:
	// 加载字典
	BOOL PYT_LoadDic ( LPSTR lpDicName ) ;
	// 查找拼音，定位拼音所对应的字
	BOOL PYT_SearchWord ( LPSTR lpPY, LPWSTR lpWord, UINT &nWordNum ) ;

private:
	// 添加Trie树的结点项
	VOID PYT_AddItem ( LPSTR lpPY, UINT nIndex ) ;
	// 检测拼音是否合法
	BOOL PYT_IsValidPY ( LPSTR lpPY ) ;
	// 字符串转化：单字符转化为宽字符
	BOOL BTW(LPSTR lpSrc,LPWSTR lpDst) ;
	// 字符串转化：宽字符转化为单字符
	BOOL WTB(LPWSTR lpSrc,LPSTR lpDst) ;

public:
	CPYTrie(void);
public:
	~CPYTrie(void);
};
