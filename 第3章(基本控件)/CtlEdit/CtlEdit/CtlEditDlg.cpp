// CtlEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CtlEdit.h"
#include "CtlEditDlg.h"

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


// CCtlEditDlg 对话框




CCtlEditDlg::CCtlEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCtlEditDlg::IDD, pParent)
	, szMsg1(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCtlEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, szMsg1);
	DDX_Text(pDX, IDC_EDIT2, szMsg2);
	DDX_Text(pDX, IDC_EDIT3, szMsg3);
	DDX_Text(pDX, IDC_EDIT4, szMsg4);
	DDX_Text(pDX, IDC_EDIT5, szMsg5);
	DDX_Text(pDX, IDC_EDIT6, szMsg6);
	DDX_Text(pDX, IDC_EDIT7, szMsg7);
	DDX_Text(pDX, IDC_EDIT8, szMsg8);
	DDX_Text(pDX, IDC_EDIT9, szMsg9);
	DDX_Text(pDX, IDC_EDIT10, szMsg10);
	DDX_Text(pDX, IDC_EDIT11, szMsg11);
	DDX_Text(pDX, IDC_EDIT12, szMsg12);
}

BEGIN_MESSAGE_MAP(CCtlEditDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCtlEditDlg 消息处理程序

BOOL CCtlEditDlg::OnInitDialog()
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

	this->szMsg1 = L"左对齐" ;
	this->szMsg2 = L"居中对齐" ;
	this->szMsg3 = L"右对齐" ;
	this->szMsg4 = L"横向滚动条" ;
	this->szMsg5 = L"纵向滚动条" ;
	this->szMsg6 = L"UpperCase" ;
	this->szMsg7 = L"LowerCase" ;
	this->szMsg8 = L"123456" ;
	this->szMsg9 = L"随意字符[*—）~32]" ;
	this->szMsg10 = L"正常显示" ;
	this->szMsg11 = L"Password" ;
	this->szMsg12 = L"只读" ;
	this->UpdateData ( FALSE ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCtlEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCtlEditDlg::OnPaint()
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
HCURSOR CCtlEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

