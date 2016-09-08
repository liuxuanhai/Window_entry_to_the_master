// ProgressCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProgressCtrl.h"
#include "ProgressCtrlDlg.h"

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


// CProgressCtrlDlg 对话框




CProgressCtrlDlg::CProgressCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressCtrlDlg::IDD, pParent)
	, nStart(0)
	, nEnd(0)
	, nStep(0)
	, szInfo(_T(""))
	, szInfo2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProgressCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BEGIN, nStart);
	DDV_MinMaxUInt(pDX, nStart, 0, 10000);
	DDX_Text(pDX, IDC_END, nEnd);
	DDV_MinMaxUInt(pDX, nEnd, 0, 100000000);
	DDX_Control(pDX, IDC_PROGRESS, ProgressCtrl);
	DDX_Text(pDX, IDC_INFO, szInfo);
	DDX_Control(pDX, IDC_PROGRESS2, ProgressCtrl2);
	DDX_Text(pDX, IDC_INFO2, szInfo2);
}

BEGIN_MESSAGE_MAP(CProgressCtrlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_BEGIN, &CProgressCtrlDlg::OnEnChangeBegin)
	ON_EN_CHANGE(IDC_END, &CProgressCtrlDlg::OnEnChangeEnd)
	ON_BN_CLICKED(IDC_SHOW, &CProgressCtrlDlg::OnBnClickedShow)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressCtrlDlg 消息处理程序

BOOL CProgressCtrlDlg::OnInitDialog()
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
	this->bStatus = FALSE ;
	this->nStart = 0 ;
	this->nEnd = 100 ;
	this->szInfo = L"当前进度" ;
	this->UpdateData ( FALSE ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CProgressCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProgressCtrlDlg::OnPaint()
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
HCURSOR CProgressCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CProgressCtrlDlg::OnEnChangeBegin()
{
	// 只有在没有工作时才能接受输入
	if ( this->bStatus == FALSE )
		this->UpdateData () ;
}

void CProgressCtrlDlg::OnEnChangeEnd()
{
	// 只有在没有工作时才能接受输入
	if ( this->bStatus == FALSE )
		this->UpdateData () ;
}

void CProgressCtrlDlg::OnBnClickedShow()
{
	if ( bStatus == TRUE )
	{
		this->MessageBox ( L"正在运行！", L"Error" ) ;
		return ;
	}

	// 检测输入是否合法
	if ( this->nEnd <= this->nStart )
	{
		this->MessageBox ( L"输入不合法！", L"Error" ) ;
		return ;
	}
	
	this->bStatus = TRUE ;

	// 设置两个进度条的范围
	this->ProgressCtrl.SetRange32 ( this->nStart, this->nEnd ) ;
	this->ProgressCtrl2.SetRange32 ( this->nStart, this->nEnd ) ;

	// 设置进度为起始值
	this->ProgressCtrl.SetPos ( this->nStart ) ;
	this->ProgressCtrl2.SetPos ( this->nStart ) ;

	// 设置步长，固定设置为100步（步长不为0）
	this->nStep = ( this->nEnd - this->nStart ) / 100 ;
	if ( this->nStep == 0 )
		this->nStep = 1 ;
	this->ProgressCtrl2.SetStep ( this->nStep ) ;

	// 设置记时器ID为1，每100ms触发一次
	this->SetTimer ( 1, 100, NULL ) ;
}

void CProgressCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 判断是否指定的记时器
	if ( nIDEvent == 1 )
	{
		// 取得当前进度
		int nCurPos1 = this->ProgressCtrl.GetPos () ;
		int nCurPos2 = this->ProgressCtrl2.GetPos () ;

		// 如果两个进度条都执行结束，就关闭记时器
		if ( nCurPos1 >= this->nEnd && nCurPos2 >= this->nEnd )
		{
			this->KillTimer ( 1 ) ;
			this->bStatus = FALSE ;
		}

		// 定位方式：每次进度增加1
		if ( nCurPos1 < this->nEnd )
			this->ProgressCtrl.SetPos ( nCurPos1 + 1 ) ;

		// 逐步方式：每次进度增加指定的步长
		if ( nCurPos2 < this->nEnd )
			this->ProgressCtrl2.StepIt () ;

		// 第一个进度条的当前信息
		this->szInfo.Format ( L"[定位方式]当前进度（CurPos=%d,Step=1）", nCurPos1 ) ;
		// 第二个进度条的当前信息
		this->szInfo2.Format ( L"[逐步方式]当前进度（CurPos=%d,Step=%d）", nCurPos2, this->nStep ) ;
		// 更新显示信息
		this->UpdateData ( FALSE ) ;
	}

	CDialog::OnTimer(nIDEvent);
}
