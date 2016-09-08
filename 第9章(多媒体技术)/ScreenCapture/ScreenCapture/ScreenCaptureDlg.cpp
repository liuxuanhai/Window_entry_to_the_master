// ScreenCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "ScreenCaptureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "vfw.h"
#pragma comment ( lib, "vfw32.lib" )

#define HOTKEY_START	1001
#define HOTKEY_STOP		1002
#define DEF_BUF_SIZE	2048

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


// CScreenCaptureDlg 对话框




CScreenCaptureDlg::CScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCaptureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->bStatus = FALSE ;
}

void CScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenCaptureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CScreenCaptureDlg 消息处理程序

BOOL CScreenCaptureDlg::OnInitDialog()
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

	RegisterHotKey ( this->m_hWnd, HOTKEY_START, MOD_CONTROL, VK_F5 ) ;
	RegisterHotKey ( this->m_hWnd, HOTKEY_STOP, MOD_CONTROL, VK_F6 ) ;

	CRect DesktopWnd ;
	GetDesktopWindow()->GetWindowRect ( &DesktopWnd ) ;

	this->nWidth = DesktopWnd.Width() ;
	this->nHeight = DesktopWnd.Height() ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CScreenCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScreenCaptureDlg::OnPaint()
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
HCURSOR CScreenCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI CaptureThread ( LPVOID lParam )
{
	CScreenCaptureDlg* pDlg = (CScreenCaptureDlg*)lParam ;

	// 取得当前目录路径
	WCHAR szFilePath[DEF_BUF_SIZE] = {0} ;
	GetCurrentDirectoryW( DEF_BUF_SIZE, szFilePath ) ;

	// 取得时间
	SYSTEMTIME	LocalTime ;
	GetLocalTime ( &LocalTime ) ;

	// 根据创建时间生成目标AVI文件名
	int nPathLen = wcslen ( szFilePath ) ;
	wsprintf ( &szFilePath[nPathLen], L"\\[%04d-%02d-%02d] %02d-%02d-%02d .avi", LocalTime.wYear, \
		LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond ) ;

	// 初始化
	AVIFileInit () ;

	// 创建AVI文件
	PAVIFILE	pAviFile ;
	AVIFileOpen ( &pAviFile, szFilePath, OF_CREATE|OF_WRITE, NULL ) ;
	if ( pAviFile == NULL )
		return 0;

	//设置位图信息头
	BITMAPINFOHEADER BmpInfoHeader = {0} ;
	BmpInfoHeader.biSize		= sizeof(BITMAPINFOHEADER); 
	BmpInfoHeader.biWidth		= pDlg->nWidth ;
	BmpInfoHeader.biHeight		= pDlg->nHeight ;
	BmpInfoHeader.biPlanes		= 1 ;
	BmpInfoHeader.biBitCount	= 32 ;
	BmpInfoHeader.biCompression	= BI_RGB ;
	BmpInfoHeader.biSizeImage	= pDlg->nWidth * pDlg->nHeight *4 ;

	// 设置AVI流信息
	AVISTREAMINFO	AviStreamInfo = {0} ;
	AviStreamInfo.fccType = streamtypeVIDEO;
	AviStreamInfo.dwScale = 1 ;
	AviStreamInfo.dwRate = 15 ;
	AviStreamInfo.dwSuggestedBufferSize = BmpInfoHeader.biSizeImage ;
	SetRect ( &AviStreamInfo.rcFrame, 0, 0, pDlg->nWidth, pDlg->nHeight ) ;

	// 创建AVI流
	PAVISTREAM	pAviStream ;
	AVIFileCreateStream ( pAviFile, &pAviStream, &AviStreamInfo ) ;
	if ( pAviStream == NULL )
		return 0 ;

	//分配临时存储区域
	LPVOID lpBmpBuf = VirtualAlloc ( NULL, BmpInfoHeader.biSizeImage, MEM_COMMIT, PAGE_READWRITE ) ;
	if ( lpBmpBuf == NULL )
		return 0;

	// 取得屏幕位图的指针
	CDC sdc ;
	sdc.CreateDC (  L"DISPLAY", NULL, NULL, NULL ) ;
	CBitmap* pCurBmp = sdc.GetCurrentBitmap() ;

	UINT nFrameIndex = 0 ;
	while ( pDlg->bStatus )
	{
		//把位图数据拷贝到临时缓冲区
		GetDIBits ( sdc.m_hDC, (HBITMAP)pCurBmp->m_hObject, 0, pDlg->nHeight, lpBmpBuf, (LPBITMAPINFO)&BmpInfoHeader, DIB_RGB_COLORS);
		// 设置流格式
		AVIStreamSetFormat ( pAviStream, nFrameIndex, &BmpInfoHeader, sizeof(BITMAPINFOHEADER) ) ;
		// 写入数据流
		AVIStreamWrite ( pAviStream, nFrameIndex, 1, lpBmpBuf, BmpInfoHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL ) ;
		// 帧数加1
		nFrameIndex ++ ;
	}

	sdc.DeleteDC () ;
	VirtualFree ( lpBmpBuf, BmpInfoHeader.biSizeImage, MEM_RELEASE ) ;

	if ( pAviStream )
		AVIStreamClose ( pAviStream ) ;
	if ( pAviFile )
		AVIFileRelease ( pAviFile ) ;
	AVIFileExit () ;

	CString msg ;
	msg.Format ( L"%s", szFilePath ) ;
	MessageBox ( 0, msg, L"保存到路径：", 0 ) ;

	return 0 ;
}

BOOL CScreenCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_HOTKEY )
	{
		switch ( pMsg->wParam )
		{
		case HOTKEY_START:	
			if ( this->bStatus == FALSE )
			{
				this->bStatus = TRUE ;
				this->CloseWindow () ;
				if ( CreateThread ( NULL, 0, CaptureThread, this, 0, NULL ) == NULL )
				{
					this->MessageBox ( L"无法创建工作线程！" ) ;
					this->bStatus = FALSE ;
				}
			}
			break ;
		case HOTKEY_STOP:
			{
				this->bStatus = FALSE ;
			}
			break ;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
