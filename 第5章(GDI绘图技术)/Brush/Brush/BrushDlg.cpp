// BrushDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Brush.h"
#include "BrushDlg.h"

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


// CBrushDlg 对话框




CBrushDlg::CBrushDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrushDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	const UINT nWidth = 100, nHeight = 100, nDist = 30 ;

	// 设置前4个图形的绘制区域
	CPoint StartPt ( 20, 30 ) ;
	for ( int i = 0; i < 4; i++ )
	{
		this->MyEllipse[i].rect.left = StartPt.x ;
		this->MyEllipse[i].rect.top = StartPt.y ;
		this->MyEllipse[i].rect.right = this->MyEllipse[i].rect.left + nWidth ;
		this->MyEllipse[i].rect.bottom = this->MyEllipse[i].rect.top + nHeight ;
		StartPt.x += nWidth + nDist ;
	}

	// 设置后4个图形的绘制区域
	for ( int i = 4; i < 8; i++ )
	{
		this->MyEllipse[i].rect.left = this->MyEllipse[i-4].rect.left ;
		this->MyEllipse[i].rect.top = this->MyEllipse[i-4].rect.top + nHeight + nDist ;
		this->MyEllipse[i].rect.right = this->MyEllipse[i].rect.left + nWidth ;
		this->MyEllipse[i].rect.bottom = this->MyEllipse[i].rect.top + nHeight ;
	}

	// 创建单色画刷
	this->MyEllipse[0].brush.CreateSolidBrush ( RGB(0,0,255) ) ;

	// 创建阴影画刷
	this->MyEllipse[1].brush.CreateHatchBrush ( HS_BDIAGONAL, RGB(0,0,255) ) ;
	this->MyEllipse[2].brush.CreateHatchBrush ( HS_CROSS, RGB(0,0,255) ) ;
	this->MyEllipse[3].brush.CreateHatchBrush ( HS_DIAGCROSS, RGB(0,0,255) ) ;
	this->MyEllipse[4].brush.CreateHatchBrush ( HS_FDIAGONAL, RGB(0,0,255) ) ;
	this->MyEllipse[5].brush.CreateHatchBrush ( HS_HORIZONTAL, RGB(0,0,255) ) ;
	this->MyEllipse[6].brush.CreateHatchBrush ( HS_VERTICAL, RGB(0,0,255) ) ;

	// 创建几何画刷
	CBitmap bmp ;
	bmp.LoadBitmapW ( IDB_BRUSH ) ;
	this->MyEllipse[7].brush.CreatePatternBrush ( &bmp ) ;
}

void CBrushDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBrushDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CBrushDlg 消息处理程序

BOOL CBrushDlg::OnInitDialog()
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

void CBrushDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBrushDlg::OnPaint()
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
		// 定义客户区域DC
		CClientDC cdc ( this ) ;
		CBrush* pOldBrush = NULL ;
		for ( int i = 0; i < 8; i++ )
		{
			// 把当前的画笔选入DC
			pOldBrush = cdc.SelectObject ( &this->MyEllipse[i].brush ) ;
			// 在指定区域绘制椭圆，区域为正方形，因而实际为画圆
			cdc.Ellipse ( this->MyEllipse[i].rect ) ;
			// 还原DC
			cdc.SelectObject ( pOldBrush ) ;
		}

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CBrushDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

