// BitmapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Bitmap.h"
#include "BitmapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CBitmapDlg 对话框
CBitmapDlg::CBitmapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 定义绘图区域的宽度、高度、间距
	const UINT nWidth = 80, nHeight = 60, nDist = 25 ;

	// 设置前4个图形的绘制区域
	CPoint StartPt ( 20, 20 ) ;
	for ( int i = 1; i <= 4; i++ )
	{
		this->MyBmp[i].rect.left = StartPt.x ;
		this->MyBmp[i].rect.top = StartPt.y ;
		this->MyBmp[i].rect.right = this->MyBmp[i].rect.left + nWidth ;
		this->MyBmp[i].rect.bottom = this->MyBmp[i].rect.top + nHeight ;
		StartPt.x += nWidth + nDist ;
	}

	// 设置后4个图形的绘制区域
	for ( int i = 5; i <= 8; i++ )
	{
		this->MyBmp[i].rect.left = this->MyBmp[i-4].rect.left ;
		this->MyBmp[i].rect.top = this->MyBmp[i-4].rect.top + nHeight + nDist ;
		this->MyBmp[i].rect.right = this->MyBmp[i].rect.left + nWidth ;
		this->MyBmp[i].rect.bottom = this->MyBmp[i].rect.top + nHeight ;
	}

	// 加载位图
	this->MyBmp[0].bmp.LoadBitmapW ( IDB_BKGROUND ) ;
	this->MyBmp[1].bmp.LoadBitmapW ( IDB_BITMAP1 ) ;
	this->MyBmp[2].bmp.LoadBitmapW ( IDB_BITMAP2 ) ;
	this->MyBmp[3].bmp.LoadBitmapW ( IDB_BITMAP3 ) ;
	this->MyBmp[4].bmp.LoadBitmapW ( IDB_BITMAP4 ) ;
	this->MyBmp[5].bmp.LoadBitmapW ( IDB_BITMAP5 ) ;
	this->MyBmp[6].bmp.LoadBitmapW ( IDB_BITMAP6 ) ;
	this->MyBmp[7].bmp.LoadBitmapW ( IDB_BITMAP7 ) ;
	this->MyBmp[8].bmp.LoadBitmapW ( IDB_BITMAP8 ) ;
}

void CBitmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBitmapDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CBitmapDlg 消息处理程序

BOOL CBitmapDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBitmapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBitmapDlg::OnPaint()
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
		// 取得窗口客户区域大小
		CRect WndRect ;
		this->GetWindowRect ( &WndRect ) ;
		this->ScreenToClient ( &WndRect ) ;

		CClientDC cdc(this) ;
		CDC		mdc, TempDc ;		// 内存DC
		BITMAP	BmpInfo ;
		CBitmap ClientBmp, *pOldBmp ;
		// 创建与设备DC兼容的内存DC
		mdc.CreateCompatibleDC ( &cdc ) ;
		TempDc.CreateCompatibleDC ( &cdc ) ;
		// 创建与设备DC兼容的位图对象
		ClientBmp.CreateCompatibleBitmap ( &cdc, WndRect.right, WndRect.bottom ) ;
		mdc.SelectObject ( &ClientBmp ) ;

		// 依次把位图贴到内存DC
		for ( int i = 0; i <= 8; i++ )
		{
			TempDc.SelectObject ( &this->MyBmp[i].bmp ) ;
			this->MyBmp[i].bmp.GetBitmap ( &BmpInfo ) ;

			if ( i == 0 )
				mdc.BitBlt (0, 0, WndRect.Width(), WndRect.Height(), &TempDc, 0, 0, SRCCOPY ) ;
			else
				mdc.TransparentBlt ( this->MyBmp[i].rect.left, \
					this->MyBmp[i].rect.top, BmpInfo.bmWidth, BmpInfo.bmHeight, \
					&TempDc, 0, 0, BmpInfo.bmWidth, BmpInfo.bmHeight, RGB(255,255,255) ) ;
		}

		// 把内存DC贴到设备DC上
		cdc.BitBlt ( 0, 0, WndRect.right, WndRect.bottom, &mdc, 0, 0, SRCCOPY ) ;

		// 环境清理
		ClientBmp.DeleteObject () ;
		mdc.DeleteDC () ;
		
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CBitmapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CBitmapDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;

	return CDialog::OnEraseBkgnd(pDC);
}
