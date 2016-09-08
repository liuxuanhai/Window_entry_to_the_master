#include "StdAfx.h"
#include "DDP.h"


CDDP::CDDP()
{
	strcpy ( this->szWindowTitle, "¶Ô¶ÔÅö½ÇÉ«°æ" ) ;

	this->dwStartOffsetX = 269 ;
	this->dwStartOffsetY = 95 ;

	this->StartButtonPos.x = 354 ;
	this->StartButtonPos.y = 373 ;
}

CDDP::~CDDP()
{

}

BOOL CDDP::DDP_IsWaiting ()
{	
	DWORD dwLenX = 63, dwLenY = 28 ;
	DWORD dwTempX = this->dwClientPosX + this->StartButtonPos.x ;
	DWORD dwTempY = this->dwClientPosY + this->StartButtonPos.y ;
	
	CBitmap TempBmp, *pOldBmp ;
	TempBmp.CreateCompatibleBitmap ( &this->ScreenDC, dwLenX, dwLenY ) ;
	pOldBmp = this->MemoryDC.SelectObject ( &TempBmp ) ;
	this->MemoryDC.BitBlt ( 0, 0, dwLenX, dwLenY, \
		&this->ScreenDC, dwTempX, dwTempY, SRCCOPY ) ;
	this->MemoryDC.SelectObject ( pOldBmp ) ;
	
	// Í¼±ê»º³åÇø£¨16-16-8-8£©
	const DWORD	dwBufSize = dwLenX * dwLenY * 4 ;
	LPBYTE	pBuf = new BYTE[dwBufSize] ;
	int nBytes = TempBmp.GetBitmapBits ( dwBufSize, pBuf ) ;
	DWORD dwValue = this->crc.CRC32_Calc ( pBuf, nBytes ) ;
	delete pBuf ;

	return dwValue == 0xee4cd0df ? TRUE : FALSE ;
}

BOOL CDDP::DDP_Find ()
{
	HWND hWnd = FindWindowA ( NULL, this->szWindowTitle ) ;
	if ( hWnd == NULL )
		return FALSE ;

	if ( GetForegroundWindow () != hWnd )
		return FALSE ;

	RECT rect ;
	::GetWindowRect ( hWnd, &rect ) ;
	this->dwClientPosX = rect.left ;
	this->dwClientPosY = rect.top ;

	if ( this->dwClientPosX > 2048 || this->dwClientPosY > 2048 )
		return FALSE ;

	this->ScreenDC.DeleteDC () ;
	this->ScreenDC.CreateDC ( L"DISPLAY", NULL, NULL, NULL ) ;

	this->MemoryDC.DeleteDC () ;
	this->MemoryDC.CreateCompatibleDC ( &this->ScreenDC ) ;
	this->CurBmp.DeleteObject () ;
	this->CurBmp.CreateCompatibleBitmap ( &this->ScreenDC, DEF_ICON_SIZE, DEF_ICON_SIZE ) ;
	this->MemoryDC.SelectObject ( &this->CurBmp ) ;

	if ( this->DDP_IsWaiting () )
	{	
		Sleep ( 2000 ) ;
		DWORD x = this->dwClientPosX + this->StartButtonPos.x ;
		DWORD y = this->dwClientPosY + this->StartButtonPos.y ;
		SetCursorPos ( x, y ) ;
		mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
		mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;
		return TRUE;
	}

	//ofstream fout ( "value.txt" ) ;

	int i, j ;
	PAIR	CurPair ;
	memset ( this->dwIconType, 0, sizeof(this->dwIconType) ) ;
	for ( i = 0; i < DEF_ICON_NUM; i++ )
		for ( j = 0; j < DEF_ICON_NUM; j++ )
			this->DDP_CalcBmpValue ( i, j ) ;
	
	for ( i = DEF_ICON_NUM-1; i >= 0; i-- )
	{
		for ( j = 0; j < DEF_ICON_NUM; j++ )
		{
			//this->DDP_CalcBmpValue ( i, j ) ;
			//
			// ºá-Á¬
			//
			if ( this->DDP_CalcBmpValue(i,j-1) && \
				 this->dwIconType[i][j-1] == this->dwIconType[i][j] )
			{
				if ( this->DDP_CalcBmpValue(i,j+1) )
				{
					CurPair.a.x = j+1 ;
					CurPair.a.y = i ;
					if ( this->DDP_CalcBmpValue(i-1,j+1) && \
						 this->dwIconType[i-1][j+1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j+1 ;
						CurPair.b.y = i-1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i,j+2) && \
						 this->dwIconType[i][j+2] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j+2 ;
						CurPair.b.y = i ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i+1,j+1) && \
						 this->dwIconType[i+1][j+1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j+1 ;
						CurPair.b.y = i+1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
				}
				if ( this->DDP_CalcBmpValue(i,j-2) )
				{
					CurPair.a.x = j-2 ;
					CurPair.a.y = i ;
					if ( this->DDP_CalcBmpValue(i-1,j-2) && \
						 this->dwIconType[i-1][j-2] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j-2 ;
						CurPair.b.y = i-1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i,j-3) && \
						 this->dwIconType[i][j-3] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j-3 ;
						CurPair.b.y = i ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i+1,j-2) && \
						 this->dwIconType[i+1][j-2] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j-2 ;
						CurPair.b.y = i+1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
				}
			}

			//
			// Êú-Á¬
			//
			if ( this->DDP_CalcBmpValue(i-1,j) && \
				 this->dwIconType[i-1][j] == this->dwIconType[i][j] )
			{
				if ( this->DDP_CalcBmpValue(i+1,j) )
				{
					CurPair.a.x = j ;
					CurPair.a.y = i+1 ;
					if ( this->DDP_CalcBmpValue(i+1,j-1) && \
						 this->dwIconType[i+1][j-1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j-1 ;
						CurPair.b.y = i+1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i+2,j) && \
						 this->dwIconType[i+2][j] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j ;
						CurPair.b.y = i+2 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i+1,j+1) && \
						 this->dwIconType[i+1][j+1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j+1 ;
						CurPair.b.y = i+1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
				}
				if ( this->DDP_CalcBmpValue(i-2,j) )
				{
					CurPair.a.x = j ;
					CurPair.a.y = i-2 ;
					if ( this->DDP_CalcBmpValue(i-2,j-1) && \
						 this->dwIconType[i-2][j-1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j-1 ;
						CurPair.b.y = i-2 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i-3,j) && \
						 this->dwIconType[i-3][j] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j ;
						CurPair.b.y = i-3 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
					if ( this->DDP_CalcBmpValue(i-2,j+1) && \
						 this->dwIconType[i-2][j+1] == this->dwIconType[i][j] )
					{
						CurPair.b.x = j+1 ;
						CurPair.b.y = i-2 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
					}
				}
			}

			//
			// ºá-¸ô
			//
			if ( this->DDP_CalcBmpValue(i,j-2) && \
				 this->dwIconType[i][j-2] == this->dwIconType[i][j] )
			{
				CurPair.a.x = j-1 ;
				CurPair.a.y = i ;
				if ( this->DDP_CalcBmpValue(i-1,j-1) && \
					 this->dwIconType[i-1][j-1] == this->dwIconType[i][j] )
				{
					CurPair.b.x = j-1 ;
					CurPair.b.y = i-1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
				}
				if ( this->DDP_CalcBmpValue(i+1,j-1) && \
					 this->dwIconType[i+1][j-1] == this->dwIconType[i][j] )
				{
					CurPair.b.x = j-1 ;
					CurPair.b.y = i+1 ;
						this->DDP_SwapPair ( &CurPair ) ;
						return TRUE;
				}
			}

			//
			// Êú-¸ô
			//
			if ( this->DDP_CalcBmpValue(i-2,j) && \
				 this->dwIconType[i-2][j] == this->dwIconType[i][j] )
			{
				CurPair.a.x = j ;
				CurPair.a.y = i-1 ;
				if ( this->DDP_CalcBmpValue(i-1,j-1) && \
					 this->dwIconType[i-1][j-1] == this->dwIconType[i][j] )
				{
					CurPair.b.x = j-1 ;
					CurPair.b.y = i-1 ;
					this->DDP_SwapPair ( &CurPair ) ;
					return TRUE;
				}
				if ( this->DDP_CalcBmpValue(i-1,j+1) && \
					 this->dwIconType[i-1][j+1] == this->dwIconType[i][j] )
				{
					CurPair.b.x = j+1 ;
					CurPair.b.y = i-1 ;
					this->DDP_SwapPair ( &CurPair ) ;
					return TRUE;
				}
			}
		}
	}	
	

	return FALSE ;
}

BOOL CDDP::DDP_CalcBmpValue ( int i, int j )
{
	if ( i < 0 || i >= DEF_ICON_NUM )
		return FALSE ;

	if ( j < 0 || j >= DEF_ICON_NUM )
		return FALSE ;

	if ( this->dwIconType[i][j] == 0 )
	{
		DWORD dwTempX = this->dwClientPosX + this->dwStartOffsetX + j * DEF_ICON_SIZE ;
		DWORD dwTempY = this->dwClientPosY + this->dwStartOffsetY + i * DEF_ICON_SIZE ;
		
		CBitmap TempBmp, *pOldBmp ;
		TempBmp.CreateCompatibleBitmap ( &this->ScreenDC, 8, 8 ) ;
		pOldBmp = this->MemoryDC.SelectObject ( &TempBmp ) ;
		this->MemoryDC.BitBlt ( 0, 0, 8, 8, \
			&this->ScreenDC, dwTempX+16, dwTempY+16, SRCCOPY ) ;
		this->MemoryDC.SelectObject ( pOldBmp ) ;
		
		// Í¼±ê»º³åÇø£¨16-16-8-8£©
		const DWORD	dwBufSize = 8 * 8 * 4 ;
		BYTE	bBmpBuf[dwBufSize] = {0} ;
		
		int nBytes = TempBmp.GetBitmapBits ( dwBufSize, bBmpBuf ) ;
		this->dwIconType[i][j] = this->crc.CRC32_Calc ( bBmpBuf, nBytes ) ;
	}
	return TRUE ;
}

VOID CDDP::DDP_SwapPair ( LPPAIR lpPair )
{
	DWORD x, y ;
	x = this->dwClientPosX + this->dwStartOffsetX + lpPair->a.x * DEF_ICON_SIZE ;
	y = this->dwClientPosY + this->dwStartOffsetY + lpPair->a.y * DEF_ICON_SIZE ;
	SetCursorPos ( x, y ) ;
	mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;

	x = this->dwClientPosX + this->dwStartOffsetX + lpPair->b.x * DEF_ICON_SIZE ;
	y = this->dwClientPosY + this->dwStartOffsetY + lpPair->b.y * DEF_ICON_SIZE ;
	SetCursorPos ( x, y ) ;
	mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;
}
