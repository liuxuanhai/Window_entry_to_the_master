// CBTHookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CBTHook.h"
#include "CBTHookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////
HHOOK	hCBT = NULL ;	// 保存CBT钩子句柄

// 键盘钩子消息处理过程
LRESULT CALLBACK CBTProc ( int nCode, WPARAM wParam, LPARAM lParam )
{
	switch ( nCode )
	{
	case HCBT_ACTIVATE:		OutputDebugStringA ( "HCBT_ACTIVE" ) ;		break ;
	case HCBT_CREATEWND:	OutputDebugStringA ( "HCBT_CREATEWND" ) ;	break ;
	case HCBT_DESTROYWND:	OutputDebugStringA ( "HCBT_DESTORYWND" ) ;	break ;
	case HCBT_MINMAX:		OutputDebugStringA ( "HCBT_MINMAX" ) ;		break ;
	case HCBT_MOVESIZE:		OutputDebugStringA ( "HCBT_MOVESIZE" ) ;	break ;
	case HCBT_SETFOCUS:		OutputDebugStringA ( "HCBT_SETFOCUS" ) ;	break ;
	case HCBT_SYSCOMMAND:	OutputDebugStringA ( "HCBT_SYSCOMMAND" ) ;	break ;
	}
	// 继续传递消息
	return CallNextHookEx ( hCBT, nCode, wParam, lParam ) ;
}

BOOL WINAPI SetHook ( BOOL isInstall ) 
{
	// 需要安装，且钩子不存在
	if ( isInstall && !hCBT )
	{
		// 设置全局钩子
		hCBT = SetWindowsHookEx ( WH_CBT, (HOOKPROC)CBTProc, 0, GetCurrentThreadId() ) ;
		if ( hCBT == NULL )
			return FALSE ;
	}

	// 需要卸载，且钩子存在
	if ( !isInstall && hCBT )
	{
		// 卸载钩子
		BOOL ret = UnhookWindowsHookEx ( hCBT ) ;
		hCBT	= NULL ;
		return ret ;
	}

	return TRUE ;
}
//////////////////////////////////////////////////////////////////////////////////

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


// CCBTHookDlg 对话框




CCBTHookDlg::CCBTHookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBTHookDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBTHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCBTHookDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START, &CCBTHookDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CCBTHookDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CCBTHookDlg 消息处理程序

BOOL CCBTHookDlg::OnInitDialog()
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

void CCBTHookDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCBTHookDlg::OnPaint()
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
HCURSOR CCBTHookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCBTHookDlg::OnBnClickedStart()
{
	if ( !SetHook ( TRUE ) )
		this->MessageBox ( L"安装钩子失败！" ) ;
}

void CCBTHookDlg::OnBnClickedStop()
{
	if ( !SetHook ( FALSE ) )
		this->MessageBox ( L"卸载钩子失败！" ) ;
}
