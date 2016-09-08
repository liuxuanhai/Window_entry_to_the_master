// SockMonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SockMon.h"
#include "SockMonDlg.h"

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


// CSockMonDlg 对话框




CSockMonDlg::CSockMonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSockMonDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSockMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOCK_TREE, SockTree);
}

BEGIN_MESSAGE_MAP(CSockMonDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_INSTALL, &CSockMonDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDC_UNINSTALL, &CSockMonDlg::OnBnClickedUninstall)
	ON_WM_DESTROY()
	ON_MESSAGE(NOTIFY_MESSAGE,OnNotifyMessage)
	ON_BN_CLICKED(IDC_EMPTY, &CSockMonDlg::OnBnClickedEmpty)
	ON_BN_CLICKED(IDC_FLUSH, &CSockMonDlg::OnBnClickedFlush)
END_MESSAGE_MAP()


// CSockMonDlg 消息处理程序

BOOL CSockMonDlg::OnInitDialog()
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

	this->SockData.SetTreeCtrl ( &this->SockTree ) ;

	this->OnBnClickedUninstall () ;
	this->OnBnClickedInstall () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSockMonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSockMonDlg::OnPaint()
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
HCURSOR CSockMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSockMonDlg::OnBnClickedInstall()
{
	if ( this->bStatus )
	{
		this->MessageBox ( L"已经安装！" ) ;
		return ;
	}
	else
	{
		this->bStatus = TRUE ;
		
		WCHAR szPath [ MAX_BUF_SIZE ] = {0} ;
		GetCurrentDirectory ( MAX_BUF_SIZE, szPath ) ;
		wcscat ( szPath, L"\\LSP.dll" ) ;
		if ( this->LSP.InstallProvider ( szPath ) != 0 )
			this->MessageBox ( L"安装失败！" ) ;
	}
}

void CSockMonDlg::OnBnClickedUninstall()
{
	this->LSP.RemoveProvider (); 
	this->bStatus = FALSE ;
}

void CSockMonDlg::OnDestroy()
{
	this->OnBnClickedUninstall () ;
	CDialog::OnDestroy();
}

LRESULT CSockMonDlg::OnNotifyMessage ( WPARAM wParam, LPARAM lParam )
{
	this->SockData.UpdateInfo () ;
	if ( this->SockData.IsFull() )
	{
		if ( this->MessageBox ( L"存储空间已满，是否清空数据？" ) == IDOK )
			this->SockData.ResetSockData () ;
	}
	return 0 ;
}

void CSockMonDlg::OnBnClickedEmpty()
{
	this->SockData.ResetSockData () ;
}

void CSockMonDlg::OnBnClickedFlush()
{
	this->SockData.FlushSockData () ;
}
