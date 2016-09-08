#include "StdAfx.h"
#include "LLK.h"

CLLK::CLLK()
{

}

CLLK::~CLLK()
{
	this->ScreenDC.DeleteDC () ;
}

BOOL CLLK::LLK_Initial ()
{
	// 设置LLK窗口名称
	strcpy ( this->szWindowTitle, "QQ游戏 - 连连看角色版" ) ;

	// 寻找LLK窗口
	this->hWnd = FindWindowA ( NULL, szWindowTitle ) ;
	if ( hWnd == NULL )
		return FALSE;

	// 设置客户区域坐标
	RECT WindowRect ;
	GetWindowRect ( this->hWnd, &WindowRect ) ;
	this->dwClientPosX = WindowRect.left ;
	this->dwClientPosY = WindowRect.top ;

	// 设置LLK图标起点坐标偏移
	this->dwStartOffsetX = 14 ;
	this->dwStartOffsetY = 181 ;

	// 设置LLK图标大小
	this->dwIconSizeX = 31 ;
	this->dwIconSizeY = 35 ;

	// 设置LLK横向和纵向图标数目
	this->dwIconNumX = 19 ;
	this->dwIconNumY = 11 ;

	// 创建屏幕DC
	this->ScreenDC.DeleteDC () ;
	this->ScreenDC.CreateDC ( L"DISPLAY", NULL, NULL, NULL ) ;

	int i, j ;
	this->dwIconNum = 0 ;
	this->dwColorNum = 0 ;
	memset ( this->bValidIcon, 0, sizeof(bValidIcon) ) ;
	memset ( this->dwColorType, 0, sizeof(this->dwColorType) ) ;
	for ( i = 0; i < MAX_ICON_NUM*MAX_ICON_NUM; i++ )
		this->IconList[i].clear() ;

	// 建立内部数据结构
	POINT pt ;
	for ( i = 0; i < this->dwIconNumY; i++ )
	{
		for ( j = 0; j < this->dwIconNumX; j++ )
		{
			pt.x = j ;	pt.y = i ;
			// 检测指定逻辑坐标位置的图标类型
			DWORD dwIconType = this->LLK_GetSpecifyIconType ( &pt ) ;			
			if ( dwIconType == 0xFFFFFFFF )
				continue ;

			this->dwIconNum++ ;
			this->bValidIcon[i][j] = TRUE ;
			this->IconList[dwIconType].push_back ( pt ) ;
		}
	}

	return TRUE ;
}

BOOL CLLK::LLK_DelSpecifyPairs ( UINT nCount )
{
	int i ;
	for ( i = 0; i < nCount; i++ )
	{
		if ( this->LLK_FindPAIR () == FALSE )
			return FALSE ;
	}
	return TRUE ;
}

BOOL CLLK::LLK_FindPAIR ()
{
	if ( this->hWnd == NULL )
		return FALSE ;

	if ( this->dwIconNum == 0 )
		return TRUE ;
	
	// 枚举邻接链表
	for ( int i = 0; i < this->dwColorNum; i++ )
	{
		if ( this->IconList[i].empty() == TRUE )
			continue ;

		vector<POINT>::iterator p, q ;
		// 双重循环，每举所有配对，检测是否可消区配对
		for ( p = this->IconList[i].begin(); p != this->IconList[i].end(); p++ )
		{
			for ( q = p; q != this->IconList[i].end(); q++ )
			{
				if ( q == p )
					continue ;
				PAIR pair ( *p, *q ) ;
				if ( this->LLK_IsValidPair ( &pair ) )
				{
					// 把消去的两个图表位置标为空
					this->bValidIcon[p->y][p->x] = FALSE ;
					this->bValidIcon[q->y][q->x] = FALSE ;
					// 消去图标，会模拟鼠标操作
					this->LLK_DeletePair ( &pair ) ;
					// 在邻接链表中删除结点
					this->IconList[i].erase ( q ) ;
					this->IconList[i].erase ( p ) ;
					// 数量减2
					this->dwIconNum -= 2 ;
					return TRUE ;
				}
			}
		}
	}

	return ( this->dwIconNum == 0 ) ? TRUE : FALSE ;
}

VOID CLLK::LLK_DeletePair ( LPPAIR lpPair )
{
	DWORD x, y ;
	x = this->dwClientPosX + this->dwStartOffsetX + lpPair->a.x * this->dwIconSizeX ;
	y = this->dwClientPosY + this->dwStartOffsetY + lpPair->a.y * this->dwIconSizeY ;
	SetCursorPos ( x, y ) ;
	mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;

	x = this->dwClientPosX + this->dwStartOffsetX + lpPair->b.x * this->dwIconSizeX ;
	y = this->dwClientPosY + this->dwStartOffsetY + lpPair->b.y * this->dwIconSizeY ;
	SetCursorPos ( x, y ) ;
	mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;
}

BOOL CLLK::LLK_IsValidPair ( LPPAIR lpPair )
{
	// 两点是否重合
	if ( lpPair->a.x == lpPair->b.x && lpPair->a.y == lpPair->b.y )
		return FALSE ;

	//
	// 0折，两点处于同一直线
	//
	BOOL isSameLine = FALSE ;
	if ( lpPair->a.x == lpPair->b.x || lpPair->a.y == lpPair->b.y )
	{
		isSameLine = TRUE ;
		if ( this->LLK_IsPairInlineValid ( lpPair ) == TRUE )
			return TRUE ;
	}

	//
	// 1折，两点不在同一直线
	//
	POINT a, b ;		// 两个折点
	a.x = lpPair->a.x ;	a.y = lpPair->b.y ;
	b.x = lpPair->b.x ;	b.y = lpPair->a.y ;
	if ( this->bValidIcon[a.y][a.x] == FALSE )
	{
		PAIR FirPair ( a, lpPair->a ) ;
		PAIR SecPair ( a, lpPair->b ) ;
		if ( this->LLK_IsPairInlineValid( &FirPair ) && \
			 this->LLK_IsPairInlineValid( &SecPair ) )
			 return TRUE ;
	}
	if ( this->bValidIcon[b.y][b.x] == FALSE )
	{
		PAIR FirPair ( b, lpPair->a ) ;
		PAIR SecPair ( b, lpPair->b ) ;
		if ( this->LLK_IsPairInlineValid( &FirPair ) && \
			 this->LLK_IsPairInlineValid( &SecPair ) )
			 return TRUE ;
	}

	//
	// 2折，横向
	//
	int i, j, nMinA, nMaxA, nMinB, nMaxB, min, max ;
	this->LLK_GetValidRangeH ( lpPair->a, nMinA, nMaxA ) ;
	this->LLK_GetValidRangeH ( lpPair->b, nMinB, nMaxB ) ;
	min = nMinA > nMinB ? nMinA : nMinB ;
	max = nMaxA < nMaxB ? nMaxA : nMaxB ;
	if ( min <= max )
	{
		for ( j = min; j <= max; j++ )
		{
			if ( j == lpPair->a.x || j == lpPair->b.x )
				continue ;

			// 两个折点（a与PairA同行，b与PairB同行）
			POINT a, b ;
			a.x = j ;	a.y = lpPair->a.y ;
			b.x = j ;	b.y = lpPair->b.y ;

			PAIR PairA ( lpPair->a, a ) ;
			PAIR PairB ( lpPair->b, b ) ;
			PAIR PairC ( a, b ) ;
			if ( this->bValidIcon[a.y][a.x] == FALSE && \
				 this->bValidIcon[b.y][b.x] == FALSE && \
				 this->LLK_IsPairInlineValid ( &PairA ) && \
				 this->LLK_IsPairInlineValid ( &PairB ) && \
				 this->LLK_IsPairInlineValid ( &PairC ) )
				 return TRUE ;
		}
	}

	// 2折，纵向
	this->LLK_GetValidRangeV ( lpPair->a, nMinA, nMaxA ) ;
	this->LLK_GetValidRangeV ( lpPair->b, nMinB, nMaxB ) ;
	min = nMinA > nMinB ? nMinA : nMinB ;
	max = nMaxA < nMaxB ? nMaxA : nMaxB ;
	if ( min <= max )
	{
		for ( i = min; i <= max; i++ )
		{
			if ( i == lpPair->a.y || i == lpPair->b.y )
				continue ;

			// 两个折点（a与PairA同列，b与PairB同列）
			POINT a, b ;
			a.x = lpPair->a.x ;		a.y = i ;
			b.x = lpPair->b.x ;		b.y = i ;

			PAIR PairA ( lpPair->a, a ) ;
			PAIR PairB ( lpPair->b, b ) ;
			PAIR PairC ( a, b ) ;
			if ( this->bValidIcon[a.y][a.x] == FALSE && \
				 this->bValidIcon[b.y][b.x] == FALSE && \
				 this->LLK_IsPairInlineValid ( &PairA ) && \
				 this->LLK_IsPairInlineValid ( &PairB ) && \
				 this->LLK_IsPairInlineValid ( &PairC ) )
				 return TRUE ;
		}
	}

	return FALSE ;
}

DWORD CLLK::LLK_GetSpecifyIconType ( LPPOINT pt )
{
	// 把逻辑坐标转化为屏幕相素坐标
	DWORD x = this->dwClientPosX + this->dwStartOffsetX + pt->x * this->dwIconSizeX ;
	DWORD y = this->dwClientPosY + this->dwStartOffsetY + pt->y * this->dwIconSizeY ;

	// 从屏幕DC中提取图标位图
	CBitmap CurBmp ;
	CDC		MemoryDC ;
	CurBmp.CreateCompatibleBitmap ( &this->ScreenDC, this->dwIconSizeX, this->dwIconSizeY ) ;
	MemoryDC.CreateCompatibleDC ( &this->ScreenDC ) ;
	MemoryDC.SelectObject ( &CurBmp ) ;
	MemoryDC.BitBlt ( 0, 0, this->dwIconSizeX, this->dwIconSizeY, &this->ScreenDC, x, y, SRCCOPY ) ;

	/*
	// DEBUG:保存图标
	CString szPathName ;
	szPathName.Format ( "%d-%d.bmp", pt->y, pt->x ) ;
	this->LLK_SaveSpecifyBmp ( &CurBmp, szPathName ) ;
	*/

	// 检测是否有效的图表
	if ( this->LLK_IsValidIcon ( &CurBmp ) == FALSE )
		return 0xFFFFFFFF ;

	// 分配临时存储区域，提取位图数据
	LPVOID lpBmpOffset = VirtualAlloc ( NULL, \
		this->dwIconSizeX*this->dwIconSizeY, MEM_COMMIT, PAGE_READWRITE ) ;
	if ( lpBmpOffset != NULL )
		CurBmp.GetBitmapBits ( this->dwIconSizeX * this->dwIconSizeY, lpBmpOffset ) ;

	// 使用CRC32对数据进行hash
	DWORD dwCrcValue = this->crc.CRC32_Calc ( \
		(LPBYTE)lpBmpOffset, this->dwIconSizeX * this->dwIconSizeY ) ;

	// 释放空间
	VirtualFree ( lpBmpOffset, this->dwIconSizeX*this->dwIconSizeY, MEM_RELEASE ) ;

	// 检测当前图标类型是否已经存在
	for ( int i = 0; i < this->dwColorNum; i++ )
	{
		if ( this->dwColorType[i] == dwCrcValue )
			return i ;
	}
	// 如果不存在，则添加新类型
	this->dwColorType[this->dwColorNum++] = dwCrcValue ;
	return (this->dwColorNum-1) ;
}

BOOL CLLK::LLK_IsPairInlineValid ( LPPAIR lpPair )
{
	// 两点是否重合
	if ( lpPair->a.x == lpPair->b.x && lpPair->a.y == lpPair->b.y )
		return FALSE ;

	// 两点不在同一直线
	if ( lpPair->a.x != lpPair->b.x && lpPair->a.y != lpPair->b.y )
		return FALSE ;

	// 两点处于同一直线
	int i, j ;
	if ( lpPair->a.x == lpPair->b.x )
	{
		int	min = this->LLK_GetMin ( lpPair->a.y, lpPair->b.y ) + 1;
		int max = this->LLK_GetMax ( lpPair->a.y, lpPair->b.y ) - 1;
		if ( min > max )
			return TRUE ;

		for ( i = min; i <= max; i++ )
		{
			if ( this->bValidIcon[i][lpPair->a.x] == TRUE )
				return FALSE ;
		}
		return TRUE ;
	}
	else if ( lpPair->a.y == lpPair->b.y )
	{
		int	min = this->LLK_GetMin ( lpPair->a.x, lpPair->b.x ) + 1;
		int max = this->LLK_GetMax ( lpPair->a.x, lpPair->b.x ) - 1;
		if ( min > max )
			return TRUE ;

		for ( j = min; j <= max; j++ )
		{
			if ( this->bValidIcon[lpPair->a.y][j] == TRUE )
				return FALSE ;
		}
		return TRUE ;
	}
	return FALSE ;
}

BOOL CLLK::LLK_IsValidIcon ( CBitmap* pBmp )
{
	DWORD dwSize = this->dwIconSizeX * this->dwIconSizeY * 4 ;
	LPBYTE lpBits = new BYTE [ dwSize ] ;
	pBmp->GetBitmapBits ( dwSize, lpBits ) ;

	int i, j ;
	int r, g, b ;
	BOOL flag = TRUE ;
	for ( i = this->dwIconSizeY - 20; i < this->dwIconSizeY; i++ )
	{
		for ( j = 0; j < 20; j++ )
		{
			DWORD dwOffset = ( i * this->dwIconSizeX + j ) * 4 ;
			if ( flag )
			{
				flag = FALSE ;
				r = lpBits[dwOffset] ;
				g = lpBits[dwOffset+1] ;
				b = lpBits[dwOffset+2] ;
			}
			else
			{
				if ( lpBits[dwOffset] != r || \
					lpBits[dwOffset+1] != g || lpBits[dwOffset+2] != b )
				{
					delete lpBits ;
					return TRUE ;
				}
			}
		}
	}
	delete lpBits ;
	return FALSE ;
}

VOID CLLK::LLK_SaveSpecifyBmp ( CBitmap* pBmp, CString szPathName )
{
	CDC		MemoryDC ;
	MemoryDC.CreateCompatibleDC ( &this->ScreenDC ) ;
	MemoryDC.SelectObject ( &pBmp ) ;

	//取得位图对象的基本信息到bmp
	BITMAP bmp ;
	pBmp->GetBitmap ( &bmp ) ;
	
	//设置位图信息头
	BITMAPINFOHEADER BmpInformHeader = {0} ;
	BmpInformHeader.biSize			= sizeof(BITMAPINFOHEADER); 
	BmpInformHeader.biWidth			= bmp.bmWidth ;
	BmpInformHeader.biHeight		= bmp.bmHeight ;
	BmpInformHeader.biPlanes		= 1 ;
    BmpInformHeader.biBitCount		= bmp.bmBitsPixel ;
    BmpInformHeader.biCompression	= BI_RGB ;
	//BmpInformHeader.biSizeImage		= bmp.bmWidthBytes * bmp.bmHeight ;

	//设置位图文件头
	BITMAPFILEHEADER BmpFileHeader = {0} ;
	BmpFileHeader.bfType	= (WORD)0x4d42 ;
	BmpFileHeader.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ;
	BmpFileHeader.bfSize	= BmpFileHeader.bfOffBits + bmp.bmWidthBytes*bmp.bmHeight ;

	//分配临时存储区域
	LPVOID lpBmpOffset = VirtualAlloc ( NULL, \
		bmp.bmWidthBytes*bmp.bmHeight, MEM_COMMIT, PAGE_READWRITE ) ;
	if ( GetLastError() != 0 )
	{
		(::AfxGetApp()->GetMainWnd())->MessageBox ( L"Memory allocate failed!" ) ;
		return ;
	}
	else
	{
		//把位图数据拷贝到临时拷贝
		GetDIBits(MemoryDC.m_hDC, (HBITMAP) pBmp->m_hObject, 0, bmp.bmHeight, \
			lpBmpOffset, (LPBITMAPINFO)&BmpInformHeader, DIB_RGB_COLORS);
	}

	//创建文件
	FILE *fp = fopen ( (LPSTR)szPathName.GetBuffer(szPathName.GetLength()), "wb" ) ;
	DWORD dwErrorCode = GetLastError() ;
	if ( fp == NULL )
	{
		(::AfxGetApp()->GetMainWnd())->MessageBox ( L"Create file error!" ) ;
		VirtualFree ( lpBmpOffset, bmp.bmWidthBytes*bmp.bmHeight, MEM_RELEASE ) ;
		return ;
	}

	//把数据写入到文件
	if ( !( fwrite(&BmpFileHeader, 1, sizeof(BITMAPFILEHEADER), fp) &&
			fwrite(&BmpInformHeader, 1, sizeof(BITMAPINFOHEADER), fp) &&
			fwrite(lpBmpOffset, 1, bmp.bmWidthBytes*bmp.bmHeight, fp) ) )
	{
		(::AfxGetApp()->GetMainWnd())->MessageBox ( L"Write file error!" ) ;
		VirtualFree ( lpBmpOffset, bmp.bmHeight*bmp.bmWidth, MEM_RELEASE ) ;
		fclose ( fp ) ;
		return ;
	}
	
	VirtualFree ( lpBmpOffset, bmp.bmHeight*bmp.bmWidth, MEM_RELEASE ) ;
	fclose(fp) ;
}

VOID CLLK::LLK_GetValidRangeH ( POINT pt, int &min, int &max )
{
	min = max = pt.x ;

	int j ;
	for ( j = pt.x - 1; j >= 0; j-- )
	{
		if ( j < 0 || this->bValidIcon[pt.y][j] == TRUE )
			break ;
		min = j ;
	}
	for ( j = pt.x + 1; j < this->dwIconNumX; j ++ )
	{
		if ( j >= this->dwIconNumX || this->bValidIcon[pt.y][j] == TRUE )
			break ;
		max = j ;
	}
}

VOID CLLK::LLK_GetValidRangeV ( POINT pt, int &min, int &max )
{
	min = max = pt.y ;
	
	int i ;
	for ( i = pt.y - 1; i >= 0; i-- )
	{
		if ( i < 0 || this->bValidIcon[i][pt.x] == TRUE )
			break ;
		min = i ;
	}
	for ( i = pt.y + 1; i < this->dwIconNumY; i ++ )
	{
		if ( i >= this->dwIconNumY || this->bValidIcon[i][pt.x] == TRUE )
			break ;
		max = i ;
	}
}

int CLLK::LLK_GetMin ( int a, int b )
{
	return a <= b ? a : b ;
}

int CLLK::LLK_GetMax ( int a, int b )
{
	return a >= b ? a : b ;
}
