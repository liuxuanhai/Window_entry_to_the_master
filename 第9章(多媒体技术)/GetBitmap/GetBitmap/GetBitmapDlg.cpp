// GetBitmapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GetBitmap.h"
#include "GetBitmapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_BUF_SIZE		1024

#define START_BITMAP_MOD	33333
#define STOP_BITMAP_MOD		44444
#define STOP_SOFTWARE		55555


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGetBitmapDlg 对话框




CGetBitmapDlg::CGetBitmapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetBitmapDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->StartPt.x = this->StartPt.y = 0 ;
	this->StopPt.x = this->StopPt.y = 0 ;
	memset ( &this->SelRect, 0, sizeof(RECT) ) ;

	RectBrush.CreateSolidBrush ( RGB(255,0,0) ) ;
}

void CGetBitmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetBitmapDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CGetBitmapDlg 消息处理程序

BOOL CGetBitmapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//最大化窗口 （ SW_SHOWMAXIMIZED ）
	this->GetScreenBmp () ;
	WINDOWPLACEMENT WndPlace ;
	WndPlace.showCmd			= SW_MAXIMIZE;
	WndPlace.ptMaxPosition.x	= 0 ;
	WndPlace.ptMaxPosition.y    = 0 ;
	this->SetWindowPlacement ( &WndPlace ) ;
	::RegisterHotKey ( this->m_hWnd, START_BITMAP_MOD, MOD_ALT, VK_F1 ) ;
	::RegisterHotKey ( this->m_hWnd, STOP_BITMAP_MOD, MOD_ALT, VK_F2 ) ;
	::RegisterHotKey ( this->m_hWnd, STOP_SOFTWARE, MOD_ALT, VK_F3 ) ;
	this->bStatus = TRUE ;
	this->UpdateView () ;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGetBitmapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGetBitmapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGetBitmapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CGetBitmapDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_HOTKEY )
	{
		switch ( pMsg->wParam )
		{
		case START_BITMAP_MOD:
			{
				if ( LOWORD(pMsg->lParam)==MOD_ALT && HIWORD(pMsg->lParam)==VK_F1 )
				{
					if( this->bStatus )
						break ;

					this->GetScreenBmp () ;
					this->ShowWindow ( SW_SHOW ) ;	
					this->bStatus == TRUE ;			
					this->StartPt.x = this->StartPt.y = this->StopPt.x = this->StopPt.y = 0 ;
					this->UpdateView () ;
				}
			}
			break ;
		case STOP_BITMAP_MOD:
			{
				if ( LOWORD(pMsg->lParam)==MOD_ALT && HIWORD(pMsg->lParam)==VK_F2 )
				{
					this->bStatus = FALSE ;
					this->ShowWindow ( SW_HIDE ) ;
				}
			}
			break ;
		case STOP_SOFTWARE:
			{
				if ( LOWORD(pMsg->lParam)==MOD_ALT && HIWORD(pMsg->lParam)==VK_F3 )
				{
					if ( this->MessageBox ( L"是否确认退出！", 0, MB_YESNO ) == IDYES )
						this->EndDialog ( 0 ) ;
				}
			}
			break ;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGetBitmapDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CGetBitmapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->StartPt.x = this->StopPt.x = point.x ;
	this->StartPt.y = this->StopPt.y = point.y ;

	CDialog::OnLButtonDown(nFlags, point);
}

void CGetBitmapDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	this->StopPt.x = point.x ;
	this->StopPt.y = point.y ;

	CDialog::OnLButtonUp(nFlags, point);
}

void CGetBitmapDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// 检测区域有效性
	if ( this->ConvertTwoPtToRect() == FALSE )
		return ;

	// 把选定的位图拷贝到剪贴版
	if ( this->SaveSelBmpToClipboard () == FALSE )
		this->MessageBox ( L"无法保存位图到剪贴版!" ) ;

	CDialog::OnRButtonUp(nFlags, point);
}

void CGetBitmapDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	//char temp[DEF_BUF_SIZE] = {0} ;
	//sprintf ( temp, "%d %d %d %d", this->StartPt.x, this->StartPt.y, this->StopPt.x, this->StopPt.y ) ;
	//::MessageBoxA ( 0, temp, 0, 0 ) ;

	// 检测区域有效性
	if ( this->ConvertTwoPtToRect() == FALSE )
		return ;

	// 把位图保存到文件
	if ( this->SaveSelBmpToFile () == FALSE )
		this->MessageBox ( L"保存位图到文件失败!" ) ;

	CDialog::OnRButtonDblClk(nFlags, point);
}

void CGetBitmapDlg::GetScreenBmp ()
{
	UINT uDeskopPixelX = ::GetSystemMetrics ( SM_CXSCREEN ) ;
	UINT uDeskopPixelY = ::GetSystemMetrics ( SM_CYSCREEN ) ;

	CDC		dc ;
	dc.CreateDC ( L"DISPLAY", NULL, NULL, NULL ) ;

	this->ScreenDc.DeleteDC () ;
	this->ScreenDc.CreateCompatibleDC ( &dc ) ;
	this->ScreenBmp.DeleteObject () ;
	this->ScreenBmp.CreateCompatibleBitmap ( &dc, uDeskopPixelX, uDeskopPixelY ) ;
	this->ScreenDc.SelectObject ( &ScreenBmp ) ;

	this->ScreenDc.BitBlt ( 0, 0, uDeskopPixelX, uDeskopPixelY, &dc, 0, 0, SRCCOPY ) ;
}

BOOL CGetBitmapDlg::ConvertTwoPtToRect ()
{
	if ( this->StartPt.x == this->StopPt.x || this->StartPt.y == this->StopPt.y )
	{
		return FALSE ;
	}

	if ( this->StartPt.x > this->StopPt.x )
	{
		this->SelRect.left	= this->StopPt.x ;
		this->SelRect.right	= this->StartPt.x ;
	}
	else
	{
		this->SelRect.left	= this->StartPt.x ;
		this->SelRect.right	= this->StopPt.x ;
	}

	if ( this->StartPt.y > this->StopPt.y )
	{
		this->SelRect.top	= this->StopPt.y ;
		this->SelRect.bottom= this->StartPt.y ;
	}
	else
	{
		this->SelRect.top	= this->StartPt.y ;
		this->SelRect.bottom= this->StopPt.y ;
	}
	return TRUE ;
}

VOID CGetBitmapDlg::UpdateView ()
{
	CDC		MemDc ;
	CBitmap bmp ;
	CClientDC cdc ( this ) ;

	//取得屏幕像素
	UINT uDeskopPixelX = ::GetSystemMetrics ( SM_CXSCREEN ) ;
	UINT uDeskopPixelY = ::GetSystemMetrics ( SM_CYSCREEN ) ;

	MemDc.CreateCompatibleDC ( &this->ScreenDc ) ;
	bmp.CreateCompatibleBitmap ( &this->ScreenDc, uDeskopPixelX, uDeskopPixelY ) ;
	MemDc.SelectObject ( &bmp ) ;
	MemDc.BitBlt ( 0, 0, uDeskopPixelX, uDeskopPixelY, &this->ScreenDc, 0, 0, SRCCOPY ) ;

	if ( this->ConvertTwoPtToRect() )
	{
		MemDc.FrameRect ( &this->SelRect, &this->RectBrush ) ;
	}

	cdc.BitBlt ( 0, 0, uDeskopPixelX, uDeskopPixelY, &MemDc, 0, 0, SRCCOPY ) ;
}

void CGetBitmapDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( ( nFlags&MK_LBUTTON ) > 0 )
	{	
		this->StopPt.x = point.x ;
		this->StopPt.y = point.y ;
		this->UpdateView () ;
	}

	CDialog::OnMouseMove(nFlags, point);
}

VOID CGetBitmapDlg::GetSelRectBitmap ()
{
	int width = this->SelRect.right - this->SelRect.left ;
	int heigh = this->SelRect.bottom - this->SelRect.top ;

	this->CurMemDc.DeleteDC () ;
	this->CurMemDc.CreateCompatibleDC ( &this->ScreenDc ) ;

	this->SelBmp.DeleteObject(); 
	this->SelBmp.CreateCompatibleBitmap ( &this->ScreenDc, width, heigh ) ;
	this->CurMemDc.SelectObject ( &this->SelBmp ) ;
	this->CurMemDc.BitBlt ( 0, 0, width, heigh, &this->ScreenDc, this->SelRect.left, this->SelRect.top, SRCCOPY ) ;
}

BOOL CGetBitmapDlg::SaveSelBmpToClipboard ()
{
	//取得所选择区域的位图到 CurBmp 
	this->GetSelRectBitmap () ;

	//打开 ClipBoard
	if ( this->OpenClipboard() == FALSE )
		return FALSE;
	
	//清空 ClipBoard
	EmptyClipboard() ;

	//设置 ClipBoard 数据
	if ( SetClipboardData( CF_BITMAP, this->SelBmp.GetSafeHandle() ) == NULL )
		return FALSE ;

	//关闭 ClipBoard
	CloseClipboard();

	// 更新显示
	this->UpdateView () ;
	return TRUE ;
}

BOOL CGetBitmapDlg::SaveSelBmpToFile ()
{
	// 取得所选择区域的位图到 CurBmp 
	this->GetSelRectBitmap () ;

	// 取得当前时间，作为位图文件名
	SYSTEMTIME SysTime ;
	CHAR szFileName[DEF_BUF_SIZE] = {0} ;
	GetLocalTime ( &SysTime ) ;
	sprintf ( szFileName, "%d-%d-%d %d-%d-%d-%d.bmp", SysTime.wYear, SysTime.wMonth, \
		SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds ) ;
	
	//取得位图对象的基本信息到bmp
	BITMAP bmp ;
	this->SelBmp.GetBitmap ( &bmp ) ;
	
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
	if ( lpBmpOffset == NULL )
		return FALSE;

	//把位图数据拷贝到临时缓冲区
	GetDIBits ( this->CurMemDc.m_hDC, (HBITMAP)this->SelBmp.m_hObject, 0, bmp.bmHeight, \
			lpBmpOffset, (LPBITMAPINFO)&BmpInformHeader, DIB_RGB_COLORS);

	//创建文件
	FILE *fp = fopen ( szFileName, "wb" ) ;
	DWORD dwErrorCode = GetLastError() ;
	if ( fp == NULL )
	{
		VirtualFree ( lpBmpOffset, bmp.bmWidthBytes*bmp.bmHeight, MEM_RELEASE ) ;
		return FALSE;
	}

	//把数据写入到文件
	if ( !( fwrite(&BmpFileHeader, 1, sizeof(BITMAPFILEHEADER), fp) &&
			fwrite(&BmpInformHeader, 1, sizeof(BITMAPINFOHEADER), fp) &&
			fwrite(lpBmpOffset, 1, bmp.bmWidthBytes*bmp.bmHeight, fp) ) )
	{
		VirtualFree ( lpBmpOffset, bmp.bmHeight*bmp.bmWidth, MEM_RELEASE ) ;
		fclose ( fp ) ;
		return FALSE ;
	}
	
	// 释放临时缓冲区
	VirtualFree ( lpBmpOffset, bmp.bmHeight*bmp.bmWidth, MEM_RELEASE ) ;
	fclose(fp) ;

	::MessageBoxA ( this->m_hWnd, szFileName, "保存位图文件成功!", 0 ) ;
	return TRUE ;
}

