#pragma once


#include <vector>
using namespace std ;

#define MAX_WORD_NUM	65536			// 最大文字数
#define MAX_LXWORD_NUM	200000			// 最大词汇量（实际157200左右）
#define MAX_WORD_LEN	16				// 最大词汇长度

// 邻接表结点
typedef struct _ITEM {
	UINT	nMatchPos ;				// nLXWordIndex所表示词汇的字索引
	UINT	nLXWordIndex ;			// 词汇索引
	struct _ITEM () {
		nMatchPos = 0 ;
		nLXWordIndex = 0 ;	
	}
	struct _ITEM ( UINT nPos, UINT nIndex ) {
		nMatchPos = nPos ;
		nLXWordIndex = nIndex ;
	}
} ITEM, *PITEM ;

// 联想字典原始数据
typedef struct _LX_ITEM {
	WCHAR	CHWord[MAX_WORD_LEN] ;	// 词汇
	UINT	nFreq ;					// 词频
} LX_ITEM, *PLX_ITEM ;

// 临时缓冲区，保存联想字列表
typedef struct _TEMP_ARRAY {
	UINT	nIndex ;
	__int64	nFreq ;
} TEMP_ARRAY, *PTEMP_ARRAY ;

class CLXWord
{
public:
	CLXWord(void);
public:
	~CLXWord(void);

private:
	PLX_ITEM	pLXItem ;		// 保存联想字典原始数据

public:
	// 加载联想字典
	BOOL	LX_LoadDic ( LPSTR lpDicName ) ;
	// 搜索联想字
	BOOL	LX_SearchWord ( LPWSTR lpWord, LPWSTR lpLXWord, UINT &nWordNum ) ;
	// 排序函数（从大到小排序）
	friend int SortCmp ( const void* a, const void* b ) ;

private:
	// 字符串转换
	inline BOOL		LX_BTW ( LPSTR lpSrc, LPWSTR lpDst ) ;
	inline BOOL		LX_WTB ( LPWSTR lpSrc, LPSTR lpDst ) ;
};
