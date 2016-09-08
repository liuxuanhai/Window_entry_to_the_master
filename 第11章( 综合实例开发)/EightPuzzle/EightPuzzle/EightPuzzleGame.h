#pragma once


#include <vector>
#include <list>
using namespace std ;

#define DEF_BMP_SIZE 32

typedef struct _ITEM {
	int nIndex ;
	int nValue ;
	struct _ITEM (){}
	struct _ITEM ( int index, int value ) {
		nIndex = index ;
		nValue = value ;
	}
} ITEM ;

class CEightPuzzleGame
{
public:
	CEightPuzzleGame(void);
	~CEightPuzzleGame(void);

private:
	BYTE	bBegMetrix[3][3] ;			// 起始状态矩阵
	BYTE	bEndMetrix[3][3] ;			// 终止状态矩阵
	BYTE	bCurMetrix[3][3] ;			// 当前状态矩阵

	list<int>	PathList ;				// 路径链表
	CBitmap		DigitBmp ;				// 位图对象
	UINT		nBlankPosX, nBlankPosY ;// 当前空格（即“0”）所在位置

public:
	// 随机产生起始状态
	VOID	RandomBegStatus () ;
	// 虫置
	VOID	Restart () ;
	// 移动，nDir表示方向（0:上；1:左；2:下；3:右）
	BOOL	SingleMove ( int nDir ) ;
	// 实现搜索算法，并保存路径，用于演示
	UINT	GetSlove () ;
	// 演示过程
	VOID	DemoShow ( CClientDC* pdc ) ;
	// 检测是否到达目标状态
	BOOL	IsFinish ( ) ;

private:
	// 把状态矩阵转化为整数
	int		ConvertMetrixToInt ( BYTE pMetrix[3][3] ) ;
	// 把整数状态为状态矩阵
	VOID	ConvertIntToCurMetrix ( int nValue ) ;

public:
	// 绘制起始状态图
	VOID	PaintBegStatus ( CClientDC* pdc ) ;
	// 绘制当前状态图
	VOID	PaintCurStatus ( CClientDC* pdc ) ;
	// 绘制终止状态图
	VOID	PaintEndStatus ( CClientDC* pdc ) ;
};
