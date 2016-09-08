#pragma once

#ifndef _WINDOWS_H
#include <windows.h>
#endif

#include <iomanip>
#include <fstream>
using namespace std ;

#include "crc32.h"

#include <vector>
using namespace std ;

#define MAX_TITLE_LEN	256
#define MAX_ICON_NUM	64
#define DEL_ALL_PAIRS	999

typedef struct _PAIR {
	POINT a, b ;
	struct _PAIR () {}
	struct _PAIR ( POINT x, POINT y ) {
		a.x = x.x ;
		a.y = x.y ;
		b.x = y.x ;
		b.y = y.y ;
	}
} PAIR, *LPPAIR ;

class CLLK  
{
public:
	CLLK();
	virtual ~CLLK();

private:
	BOOL	bActive ;							// LLK是否存在处于激活状态
	HWND	hWnd ;								// LLK窗口句柄
	CHAR	szWindowTitle[MAX_TITLE_LEN+1] ;	// LLK窗口标题
	DWORD	dwClientPosX, dwClientPosY ;		// LLK客户区域坐标
	DWORD	dwStartOffsetX, dwStartOffsetY ;	// LLK图标起点与客户区域的偏移坐标
	DWORD	dwIconSizeX, dwIconSizeY ;			// LLK图标大小
	DWORD	dwIconNumX, dwIconNumY ;			// LLK横向与纵向图标数

private:
	DWORD	dwIconNum ;										// 图标数量
	BOOL	bValidIcon[MAX_ICON_NUM+1][MAX_ICON_NUM+1] ;	// 标识指定位置的图标是否存在
	DWORD	dwColorNum ;									// 图标类型数
	DWORD	dwColorType[MAX_ICON_NUM*MAX_ICON_NUM] ;		// 保存每种图表的hash值
	vector<POINT>	IconList[MAX_ICON_NUM*MAX_ICON_NUM] ;	// 邻接链表

private:
	CRC32	crc ;		// 使用CRC32作为hash算法
	CDC		ScreenDC ;	// 屏幕DC

public:
	// 初始化，取屏并分析
	BOOL	LLK_Initial () ;

	// 消去指定数目的配对
	BOOL	LLK_DelSpecifyPairs ( UINT nCount = DEL_ALL_PAIRS ) ;


private:
	// 检测图标对是否合法
	BOOL	LLK_IsValidPair ( LPPAIR lpPair ) ;

	// 取得指定图标类型
	DWORD	LLK_GetSpecifyIconType ( LPPOINT pt ) ;

	// 返回TRUE：两点在同一直线且两点之间无障碍
	BOOL	LLK_IsPairInlineValid ( LPPAIR lpPair ) ;

	// 取得指顶点横向有效区域
	VOID	LLK_GetValidRangeH ( POINT pt, int &min, int &max ) ;

	// 取得指顶点纵向有效区域
	VOID	LLK_GetValidRangeV ( POINT pt, int &min, int &max ) ;

	// 保存指定图标
	VOID	LLK_SaveSpecifyBmp ( CBitmap* pBmp, CString szPathName ) ;

	// 检测指定BMP是否为空图标
	BOOL	LLK_IsValidIcon ( CBitmap* pBmp ) ;

	// 寻找配对
	BOOL	LLK_FindPAIR () ;

	// 消去配对
	VOID	LLK_DeletePair ( LPPAIR lpPair ) ;

private:
	int		LLK_GetMin ( int a, int b ) ;
	int		LLK_GetMax ( int a, int b ) ;
};
