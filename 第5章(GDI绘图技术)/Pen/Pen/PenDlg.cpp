// PenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Pen.h"
#include "PenDlg.h"

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


// CPenDlg 对话框




CPenDlg::CPenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 定义图形的宽度、高度、间距
	const UINT nWidth = 100, nHeight = 100, nDist=20 ;

	// 设置前4个图形的绘制区域
	CPoint StartPt ( 20, 20 ) ;
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

	// 创建普通画笔
	this->MyEllipse[0].pen.CreatePen ( PS_SOLID, 5, RGB(0,0,255) ) ;
	this->MyEllipse[1].pen.CreatePen ( PS_DASH, 1, RGB(0,0,255) ) ;
	this->MyEllipse[2].pen.CreatePen ( PS_DOT, 1, RGB(0,0,255) ) ;
	this->MyEllipse[3].pen.CreatePen ( PS_DASHDOT, 1, RGB(0,0,255) ) ;
	this->MyEllipse[4].pen.CreatePen ( PS_DASHDOTDOT, 1, RGB(0,0,255) ) ;
	this->MyEllipse[5].pen.CreatePen ( PS_NULL, 1, RGB(0,0,255) ) ;
	this->MyEllipse[6].pen.CreatePen ( PS_INSIDEFRAME, 5, RGB(0,0,255) ) ;

	// 创建集合画笔
	LOGBRUSH	LogBrush ;
	LogBrush.lbStyle = BS_HATCHED ;
	LogBrush.lbColor = RGB ( 0, 0, 255 ) ;
	LogBrush.lbHatch = HS_DIAGCROSS ;
	this->MyEllipse[7].pen.CreatePen ( PS_GEOMETRIC, 10, &LogBrush ) ;
}

void CPenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPenDlg 消息处理程序

BOOL CPenDlg::OnInitDialog()
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

void CPenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPenDlg::OnPaint()
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
		CPen* pOldPen = NULL ;
		for ( int i = 0; i < 8; i++ )
		{
			// 把当前的画笔选入DC
			pOldPen = cdc.SelectObject ( &this->MyEllipse[i].pen ) ;
			// 在指定区域绘制椭圆，区域为正方形，因而实际为画圆
			cdc.Ellipse ( this->MyEllipse[i].rect ) ;
			// 还原DC
			cdc.SelectObject ( pOldPen ) ;
		}
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CPenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

