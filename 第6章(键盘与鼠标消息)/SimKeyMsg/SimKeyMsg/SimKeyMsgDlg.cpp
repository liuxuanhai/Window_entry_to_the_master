// SimKeyMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SimKeyMsg.h"
#include "SimKeyMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_POSTMESSAGE	1001
#define HOTKEY_KEYBDEVENT	1002


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


// CSimKeyMsgDlg 对话框




CSimKeyMsgDlg::CSimKeyMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimKeyMsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimKeyMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimKeyMsgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UP, &CSimKeyMsgDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CSimKeyMsgDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_LEFT, &CSimKeyMsgDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CSimKeyMsgDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_WIN_R, &CSimKeyMsgDlg::OnBnClickedWinR)
	ON_BN_CLICKED(IDC_WIN_U, &CSimKeyMsgDlg::OnBnClickedWinU)
	ON_BN_CLICKED(IDC_CTRL_SHIFT, &CSimKeyMsgDlg::OnBnClickedCtrlShift)
	ON_BN_CLICKED(IDC_ALT_TAB, &CSimKeyMsgDlg::OnBnClickedAltTab)
END_MESSAGE_MAP()


// CSimKeyMsgDlg 消息处理程序

BOOL CSimKeyMsgDlg::OnInitDialog()
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

void CSimKeyMsgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimKeyMsgDlg::OnPaint()
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
HCURSOR CSimKeyMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSimKeyMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYDOWN )
	{
		switch ( pMsg->wParam )
		{
		case VK_UP:		this->MessageBox ( L"你按下了↑键！" ) ;	break ;
		case VK_DOWN:	this->MessageBox ( L"你按下了↓键！" ) ;	break ;
		case VK_LEFT:	this->MessageBox ( L"你按下了←键！" ) ;	break ;
		case VK_RIGHT:	this->MessageBox ( L"你按下了→键！" ) ;	break ;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSimKeyMsgDlg::OnBnClickedUp()
{
	this->PostMessage ( WM_KEYDOWN, VK_UP, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedDown()
{
	this->PostMessage ( WM_KEYDOWN, VK_DOWN, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedLeft()
{
	this->PostMessage ( WM_KEYDOWN, VK_LEFT, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedRight()
{
	this->PostMessage ( WM_KEYDOWN, VK_RIGHT, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedWinR()
{
	keybd_event ( VK_LWIN, 0, 0, 0 ) ;
	keybd_event ( 'R', 0, 0, 0 ) ;
	keybd_event ( 'R', 0, KEYEVENTF_KEYUP, 0 ) ;
	keybd_event ( VK_LWIN, 0, KEYEVENTF_KEYUP, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedWinU()
{
	keybd_event ( VK_LWIN, 0, 0, 0 ) ;
	keybd_event ( 'U', 0, 0, 0 ) ;
	keybd_event ( 'U', 0, KEYEVENTF_KEYUP, 0 ) ;
	keybd_event ( VK_LWIN, 0, KEYEVENTF_KEYUP, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedCtrlShift()
{
	keybd_event ( VK_CONTROL, 0, 0, 0 ) ;
	keybd_event ( VK_SHIFT, 0, 0, 0 ) ;
	keybd_event ( VK_SHIFT, 0, KEYEVENTF_KEYUP, 0 ) ;
	keybd_event ( VK_CONTROL, 0, KEYEVENTF_KEYUP, 0 ) ;
}

void CSimKeyMsgDlg::OnBnClickedAltTab()
{
	keybd_event ( VK_MENU, 0, 0, 0 ) ;
	keybd_event ( VK_TAB, 0, 0, 0 ) ;
	Sleep ( 1000 ) ;
	keybd_event ( VK_TAB, 0, KEYEVENTF_KEYUP, 0 ) ;
	keybd_event ( VK_MENU, 0, KEYEVENTF_KEYUP, 0 ) ;
}
