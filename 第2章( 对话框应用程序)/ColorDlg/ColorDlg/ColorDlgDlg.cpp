// ColorDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorDlg.h"
#include "ColorDlgDlg.h"

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


// CColorDlgDlg 对话框




CColorDlgDlg::CColorDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlgDlg::IDD, pParent)
	, r(0)
	, g(0)
	, b(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_R, r);
	DDX_Text(pDX, IDC_G, g);
	DDX_Text(pDX, IDC_B, b);
}

BEGIN_MESSAGE_MAP(CColorDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SET_COLOR, &CColorDlgDlg::OnBnClickedSetColor)
	ON_EN_CHANGE(IDC_R, &CColorDlgDlg::OnEnChangeR)
	ON_EN_CHANGE(IDC_G, &CColorDlgDlg::OnEnChangeG)
	ON_EN_CHANGE(IDC_B, &CColorDlgDlg::OnEnChangeB)
END_MESSAGE_MAP()


// CColorDlgDlg 消息处理程序

BOOL CColorDlgDlg::OnInitDialog()
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
	this->r = this->g = this->b = 255 ;
	this->UpdateData ( FALSE ) ;

	this->ColorShowRect.left = 147 ;
	this->ColorShowRect.top = 30 ;
	this->ColorShowRect.right = 245 ;
	this->ColorShowRect.bottom = 114 ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CColorDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorDlgDlg::OnPaint()
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

		CClientDC dc (this) ;	// 定义窗口DC
		CBrush* pOldBrush ;		// 用于保存老的画刷
		CBrush brush ;			// 定义新的画刷
		// 根据指定的颜色创建画刷
		brush.CreateSolidBrush ( RGB(this->r,this->g,this->b) ) ;
		// 把新画刷选入DC
		pOldBrush = dc.SelectObject ( &brush ) ;
		// 根据指定的区域绘制矩形区域
		dc.Rectangle ( &this->ColorShowRect ) ;
		// 还原，把老画刷选入DC
		dc.SelectObject ( pOldBrush ) ;
		// 删除新画刷
		brush.DeleteObject () ;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CColorDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CColorDlgDlg::OnBnClickedSetColor()
{
	// 定义“颜色”对话框
	CColorDialog dlg ;
	if ( dlg.DoModal () == IDOK )
	{
		// 取得选取的颜色
		COLORREF color = dlg.GetColor () ;
		// 取得红色值
		this->r = GetRValue ( color ) ;
		// 取得绿色值
		this->g = GetGValue ( color ) ;
		// 取得蓝色值
		this->b = GetBValue ( color ) ;
		// 更新界面显示的颜色值
		this->UpdateData ( FALSE ) ;
		// 更新界面绘制的色彩
		this->Invalidate () ;
	}
}

void CColorDlgDlg::OnEnChangeR()
{
	this->UpdateData () ;
	this->Invalidate () ;
}

void CColorDlgDlg::OnEnChangeG()
{
	this->UpdateData () ;
	this->Invalidate () ;
}

void CColorDlgDlg::OnEnChangeB()
{
	this->UpdateData () ;
	this->Invalidate () ;
}
