// DDPToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DDPTool.h"
#include "DDPToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_FIND_ONCE	1001
#define HOTKEY_BEGIN_FIND	1002
#define HOTKEY_STOP_FIND	1003


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


// CDDPToolDlg 对话框




CDDPToolDlg::CDDPToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDDPToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDDPToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDDPToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDDPToolDlg 消息处理程序

BOOL CDDPToolDlg::OnInitDialog()
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
	::RegisterHotKey ( this->m_hWnd, HOTKEY_FIND_ONCE, 0, VK_F5 ) ;
	::RegisterHotKey ( this->m_hWnd, HOTKEY_BEGIN_FIND, 0, VK_F6 ) ;
	::RegisterHotKey ( this->m_hWnd, HOTKEY_STOP_FIND, 0, VK_F7 ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDDPToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDDPToolDlg::OnPaint()
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
HCURSOR CDDPToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT NewThread ( LPVOID lpParam )
{
	CDDPToolDlg* pDlg = (CDDPToolDlg*)lpParam ;

	while ( pDlg->bStatus )
	{
		pDlg->ddp.DDP_Find() ;
		Sleep ( 100 ) ;
	}
	return 0 ;
}


BOOL CDDPToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_HOTKEY )
	{
		switch ( pMsg->wParam )
		{
		case HOTKEY_FIND_ONCE:
			if ( this->bStatus == FALSE )
				this->ddp.DDP_Find() ;	
			break ;
		case HOTKEY_BEGIN_FIND:	
			this->bStatus = TRUE ;
			AfxBeginThread ( NewThread, this, THREAD_PRIORITY_HIGHEST ) ;
			break ;
		case HOTKEY_STOP_FIND:
			this->bStatus = FALSE ;
			break ;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
