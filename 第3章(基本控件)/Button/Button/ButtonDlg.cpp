// ButtonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Button.h"
#include "ButtonDlg.h"

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


// CButtonDlg 对话框




CButtonDlg::CButtonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CButtonDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR1, addr1);
	DDX_Control(pDX, IDC_ADDR2, addr2);
	DDX_Control(pDX, IDC_ADDR3, addr3);
	DDX_Control(pDX, IDC_ADDR4, addr4);
	DDX_Control(pDX, IDC_GAME1, game1);
	DDX_Control(pDX, IDC_GAME2, game2);
	DDX_Control(pDX, IDC_GAME3, game3);
	DDX_Control(pDX, IDC_GAME4, game4);
	DDX_Control(pDX, IDC_GAME5, game5);
	DDX_Control(pDX, IDC_GAME6, game6);
}

BEGIN_MESSAGE_MAP(CButtonDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SURE, &CButtonDlg::OnBnClickedSure)
END_MESSAGE_MAP()


// CButtonDlg 消息处理程序

BOOL CButtonDlg::OnInitDialog()
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

	this->addr1.SetCheck ( BST_CHECKED ) ;

	this->game1.SetCheck (  BST_CHECKED ) ;
	this->game2.SetCheck (  BST_CHECKED ) ;
	this->game3.SetCheck (  BST_CHECKED ) ;
	this->game4.SetCheck (  BST_CHECKED ) ;
	this->game5.SetCheck (  BST_CHECKED ) ;
	this->game6.SetCheck (  BST_CHECKED ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CButtonDlg::OnPaint()
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
HCURSOR CButtonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CButtonDlg::OnBnClickedSure()
{
	CString szMsg ;

	szMsg += L"您的所在地：" ;
	if ( this->addr1.GetCheck () == BST_CHECKED )
		szMsg += L"北京\n" ;
	else if ( this->addr2.GetCheck () == BST_CHECKED )
		szMsg += L"上海\n" ;
	else if ( this->addr3.GetCheck () == BST_CHECKED )
		szMsg += L"深圳\n" ;
	else if ( this->addr4.GetCheck () == BST_CHECKED )
		szMsg += L"杭州\n" ;

	szMsg += L"您的兴趣爱好：" ;
	if ( this->game1.GetCheck() == BST_CHECKED )
		szMsg += L"体育运动 " ;
	if ( this->game2.GetCheck() == BST_CHECKED )
		szMsg += L"影视娱乐 " ;
	if ( this->game3.GetCheck() == BST_CHECKED )
		szMsg += L"黑客技术 " ;
	if ( this->game4.GetCheck() == BST_CHECKED )
		szMsg += L"旅游观光 " ;
	if ( this->game5.GetCheck() == BST_CHECKED )
		szMsg += L"休闲棋牌 " ;
	if ( this->game6.GetCheck() == BST_CHECKED )
		szMsg += L"网络游戏 " ;

	this->MessageBox ( szMsg, L"您的选择" ) ;
}
