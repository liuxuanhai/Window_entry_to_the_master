#include "StdAfx.h"
#include "resource.h"
#include "EightPuzzleGame.h"

#include <stdlib.h>
#include <time.h>

#include "Hash.h"

CEightPuzzleGame::CEightPuzzleGame(void)
{
	this->DigitBmp.LoadBitmapW ( IDB_DIGIT ) ;

	this->bEndMetrix[0][0] = 1 ;
	this->bEndMetrix[0][1] = 2 ;
	this->bEndMetrix[0][2] = 3 ;
	this->bEndMetrix[1][0] = 8 ;
	this->bEndMetrix[1][1] = 0 ;
	this->bEndMetrix[1][2] = 4 ;
	this->bEndMetrix[2][0] = 7 ;
	this->bEndMetrix[2][1] = 6 ;
	this->bEndMetrix[2][2] = 5 ;

	this->RandomBegStatus () ;
}

CEightPuzzleGame::~CEightPuzzleGame(void)
{
}

VOID CEightPuzzleGame::PaintBegStatus ( CClientDC* pdc )
{
	CDC		MemoryDC, TempDC ;
	MemoryDC.CreateCompatibleDC ( pdc ) ;
	TempDC.CreateCompatibleDC ( pdc ) ;
	TempDC.SelectObject ( &this->DigitBmp ) ;

	CBitmap BegBmp ;
	BegBmp.CreateCompatibleBitmap ( pdc, DEF_BMP_SIZE * 3, DEF_BMP_SIZE * 3 ) ;

	// 绘制初始状态图
	MemoryDC.SelectObject ( &BegBmp ) ;	
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			MemoryDC.StretchBlt ( DEF_BMP_SIZE*j, DEF_BMP_SIZE*i, DEF_BMP_SIZE, DEF_BMP_SIZE, \
				&TempDC, DEF_BMP_SIZE*this->bBegMetrix[i][j], 0, DEF_BMP_SIZE, DEF_BMP_SIZE, SRCCOPY ) ;
		}
	}
	pdc->BitBlt ( 240, 25, DEF_BMP_SIZE*3, DEF_BMP_SIZE*3, &MemoryDC, 0, 0, SRCCOPY ) ;

	TempDC.DeleteDC () ;
	MemoryDC.DeleteDC() ;
	BegBmp.DeleteObject() ;
}


VOID CEightPuzzleGame::PaintCurStatus ( CClientDC* pdc )
{
	CDC		MemoryDC, TempDC ;
	MemoryDC.CreateCompatibleDC ( pdc ) ;
	TempDC.CreateCompatibleDC ( pdc ) ;
	TempDC.SelectObject ( &this->DigitBmp ) ;

	CBitmap CurBmp ;
	CurBmp.CreateCompatibleBitmap ( pdc, DEF_BMP_SIZE * 6, DEF_BMP_SIZE * 6 ) ;

	// 绘制当前状态图
	MemoryDC.SelectObject ( &CurBmp ) ;
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			MemoryDC.StretchBlt ( DEF_BMP_SIZE*2*j, DEF_BMP_SIZE*2*i, DEF_BMP_SIZE*2, DEF_BMP_SIZE*2, \
				&TempDC, DEF_BMP_SIZE*this->bCurMetrix[i][j], 0, DEF_BMP_SIZE, DEF_BMP_SIZE, SRCCOPY ) ;
		}
	}
	pdc->BitBlt ( 20, 25, DEF_BMP_SIZE*6, DEF_BMP_SIZE*6, &MemoryDC, 0, 0, SRCCOPY ) ;

	TempDC.DeleteDC () ;
	MemoryDC.DeleteDC() ;
	CurBmp.DeleteObject() ;
}


VOID CEightPuzzleGame::PaintEndStatus ( CClientDC* pdc )
{
	CDC		MemoryDC, TempDC ;
	MemoryDC.CreateCompatibleDC ( pdc ) ;
	TempDC.CreateCompatibleDC ( pdc ) ;
	TempDC.SelectObject ( &this->DigitBmp ) ;

	CBitmap	EndBmp ;
	EndBmp.CreateCompatibleBitmap ( pdc, DEF_BMP_SIZE * 3, DEF_BMP_SIZE * 3 ) ;

	// 绘制目标状态图
	MemoryDC.SelectObject ( &EndBmp ) ;
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			MemoryDC.StretchBlt ( DEF_BMP_SIZE*j, DEF_BMP_SIZE*i, DEF_BMP_SIZE, DEF_BMP_SIZE, \
				&TempDC, DEF_BMP_SIZE*this->bEndMetrix[i][j], 0, DEF_BMP_SIZE, DEF_BMP_SIZE, SRCCOPY ) ;
		}
	}
	pdc->BitBlt ( 363, 25, DEF_BMP_SIZE*3, DEF_BMP_SIZE*3, &MemoryDC, 0, 0, SRCCOPY ) ;

	TempDC.DeleteDC () ;
	MemoryDC.DeleteDC() ;
	EndBmp.DeleteObject() ;
}

VOID CEightPuzzleGame::RandomBegStatus ()
{
	srand ( time (NULL) ) ;
	memcpy ( this->bCurMetrix, this->bEndMetrix, sizeof(BYTE) * 9 ) ;

	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( this->bCurMetrix[i][j] == 0 )
			{
				this->nBlankPosX = i ;
				this->nBlankPosY = j ;
			}
		}
	}

	int step = ( rand() % 500 ) + 50 ;
	for ( int k = 0; k < step; k++ )
	{
		while ( this->SingleMove ( rand() % 4 ) == FALSE ) ;
	}

	memcpy ( this->bBegMetrix, this->bCurMetrix, sizeof(BYTE)*9 ) ;
}


VOID CEightPuzzleGame::Restart ()
{
	memcpy ( this->bCurMetrix, this->bBegMetrix, sizeof(BYTE) * 9 ) ;

	for ( int i = 0; i < 3; i ++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( this->bCurMetrix[i][j] == 0 )
			{
				this->nBlankPosX = i ;
				this->nBlankPosY = j ;
				return ;
			}
		}
	}
}

// 0: 上
// 1: 左
// 2: 下
// 3: 右
BOOL CEightPuzzleGame::SingleMove ( int nDir )
{
	switch ( nDir )
	{
	case 0:	
		{
			if ( this->nBlankPosX == 0 )
				return FALSE ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY] = this->bCurMetrix[this->nBlankPosX-1][this->nBlankPosY] ;
			this->bCurMetrix[this->nBlankPosX-1][this->nBlankPosY] = 0 ;
			this->nBlankPosX -- ;
			return TRUE ;
		}
	case 1:
		{
			if ( this->nBlankPosY == 0 )
				return FALSE ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY] = this->bCurMetrix[this->nBlankPosX][this->nBlankPosY-1] ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY-1] = 0 ;
			this->nBlankPosY -- ;
			return TRUE ;
		}
	case 2:
		{
			if ( this->nBlankPosX == 2 )
				return FALSE ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY] = this->bCurMetrix[this->nBlankPosX+1][this->nBlankPosY] ;
			this->bCurMetrix[this->nBlankPosX+1][this->nBlankPosY] = 0 ;
			this->nBlankPosX ++ ;
			return TRUE ;
		}
	case 3:
		{
			if ( this->nBlankPosY == 2 )
				return FALSE ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY] = this->bCurMetrix[this->nBlankPosX][this->nBlankPosY+1] ;
			this->bCurMetrix[this->nBlankPosX][this->nBlankPosY+1] = 0 ;
			this->nBlankPosY ++ ;
			return TRUE ;
		}
	default:	return FALSE ;
	}
}

int CEightPuzzleGame::ConvertMetrixToInt ( BYTE pMetrix[3][3] )
{
	return (	pMetrix[0][0]*100000000 + pMetrix[0][1]*10000000 + pMetrix[0][2]*1000000 + \
				pMetrix[1][0]*100000 + pMetrix[1][1]*10000 + pMetrix[1][2]*1000 + \
				pMetrix[2][0]*100 + pMetrix[2][1]*10 + pMetrix[2][2] ) ;
}

VOID CEightPuzzleGame::ConvertIntToCurMetrix ( int nValue ) 
{	
	this->bCurMetrix[0][0] = ( nValue / 100000000 ) % 10 ;
	this->bCurMetrix[0][1] = ( nValue / 10000000 ) % 10 ;
	this->bCurMetrix[0][2] = ( nValue / 1000000 ) % 10 ;
	this->bCurMetrix[1][0] = ( nValue / 100000 ) % 10 ;
	this->bCurMetrix[1][1] = ( nValue / 10000 ) % 10 ;
	this->bCurMetrix[1][2] = ( nValue / 1000 ) % 10 ;
	this->bCurMetrix[2][0] = ( nValue / 100 ) % 10 ;
	this->bCurMetrix[2][1] = ( nValue / 10 ) % 10 ;
	this->bCurMetrix[2][2] = nValue % 10 ;
	
	for ( int i = 0; i < 3; i ++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			if ( this->bCurMetrix[i][j] == 0 )
			{
				this->nBlankPosX = i ;
				this->nBlankPosY = j ;
				return ;
			}
		}
	}
}

UINT CEightPuzzleGame::GetSlove ()
{
	int nCurValue = this->ConvertMetrixToInt ( this->bBegMetrix ) ;
	int nEndValue = this->ConvertMetrixToInt ( this->bEndMetrix ) ;

	vector<ITEM> Queue ;				// 广搜的辅助队列
	CHash Hash ;						// Hash表，用于保存状态
	Queue.clear () ;			
	Queue.push_back ( ITEM(-1,nCurValue) ) ;
	Hash.AddValue ( nCurValue ) ;

	int nCurIndex = 0, nTotalIndex = 1 ;
	while ( nCurIndex < nTotalIndex )
	{
		if ( nCurIndex == 200000 )
			return 0 ;

		nCurValue = Queue[nCurIndex].nValue ;
		for ( int i = 0; i < 4; i++ )		// 枚举4个方向
		{
			this->ConvertIntToCurMetrix ( nCurValue ) ;
			if ( this->SingleMove ( i ) == FALSE )
				continue ;
			int nValue = this->ConvertMetrixToInt ( this->bCurMetrix ) ;
			// 如果这个状态首次出现，则添加辅助队列并保存到hash表
			if ( Hash.IsValueExist ( nValue ) == FALSE )
			{
				nTotalIndex ++ ;
				Hash.AddValue ( nValue ) ;
				Queue.push_back ( ITEM(nCurIndex,nValue) ) ;
				// 达到目标状态时，提取路径，注意路径为倒序
				if ( nValue == nEndValue )
				{
					this->PathList.clear () ;
					this->PathList.push_back ( nValue ) ;

					int nIndex = nCurIndex, nStep = 0 ;
					while ( TRUE )
					{
						nStep ++ ;
						this->PathList.push_back ( Queue[nIndex].nValue ) ;
						if ( ( nIndex = Queue[nIndex].nIndex ) == -1 )
							break ;
					}
					return nStep ;
				}
			}
		}
		nCurIndex++ ;
	}

	return 0 ;
}

VOID CEightPuzzleGame::DemoShow ( CClientDC* pdc )
{
	while ( this->PathList.empty() == FALSE )
	{
		this->ConvertIntToCurMetrix ( this->PathList.back() ) ;
		this->PaintCurStatus ( pdc ) ;
		this->PathList.pop_back() ;
		Sleep ( 1000 ) ;
	}
}

BOOL CEightPuzzleGame::IsFinish ()
{
	if ( this->ConvertMetrixToInt(this->bCurMetrix) == this->ConvertMetrixToInt(this->bEndMetrix) )
		return TRUE ;
	return FALSE ;
}