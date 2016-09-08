#pragma once

#include "crc32.h"

#include <fstream>
using namespace std ;

#define MAX_TITLE_LEN	256

// 每一行/每一列图标数目
#define DEF_ICON_NUM	8

// 图标大小
#define DEF_ICON_SIZE	48	

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

class CDDP  
{
public:
	CDDP();
	virtual ~CDDP();

private:
	POINT	StartButtonPos ;
	HWND	hWnd ;										// DDP窗口句柄
	CHAR	szWindowTitle[MAX_TITLE_LEN+1] ;			// DDP窗口标题
	DWORD	dwStartOffsetX, dwStartOffsetY ;			// DDP图标起点与客户区域的偏移坐标
	DWORD	dwIconType[DEF_ICON_NUM][DEF_ICON_NUM] ;	// 保存图表类型值
	DWORD	dwClientPosX, dwClientPosY ;				// 游戏窗口客户区域起点
	CRC32	crc ;										// 用于hash处理
	CBitmap CurBmp ;									// 屏幕位图
	CDC		ScreenDC, MemoryDC ;						// 屏幕DC和内存DC

public:
	BOOL	DDP_Find () ;						// 消去操作

private:
	VOID	DDP_SwapPair ( LPPAIR lpPair ) ;	// 交换图标，模拟鼠标操作
	BOOL	DDP_CalcBmpValue ( int i, int j ) ;	// 计算指定坐标图标的hash值
	BOOL	DDP_IsWaiting () ;					// 检测是否处于等待状态
};