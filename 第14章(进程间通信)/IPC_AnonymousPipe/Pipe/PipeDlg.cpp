// PipeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Pipe.h"
#include "PipeDlg.h"

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


// CPipeDlg 对话框




CPipeDlg::CPipeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeDlg::IDD, pParent)
	, szParam(_T(""))
	, szOutput(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARAM, szParam);
	DDV_MaxChars(pDX, szParam, 256);
	DDX_Text(pDX, IDC_OUTPUT_INFO, szOutput);
}

BEGIN_MESSAGE_MAP(CPipeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_PARAM, &CPipeDlg::OnEnChangeParam)
	ON_BN_CLICKED(IDC_EXECUTE, &CPipeDlg::OnBnClickedExecute)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPipeDlg 消息处理程序

BOOL CPipeDlg::OnInitDialog()
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

void CPipeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPipeDlg::OnPaint()
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
HCURSOR CPipeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPipeDlg::OnEnChangeParam()
{
	this->UpdateData () ;
}

// 工作线程
DWORD WINAPI NewThread ( LPVOID lParam )
{
	CPipeDlg* pDlg = (CPipeDlg*)lParam ;
	WCHAR szSysDir[1024] ;
	CString szCommand ;
	// 取得系统目录
	GetSystemDirectory ( szSysDir, sizeof(szSysDir) ) ;
	// 格式化命令行参数
	szCommand.Format ( L"%s\\ping.exe ", szSysDir ) ;
	szCommand += pDlg->szParam ;

	HANDLE	hRead, hWrite ;
	SECURITY_ATTRIBUTES saAttr;			// 定义安全描述符
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	// 创建管道，用于重定向ping程序的输出信息
	if ( !CreatePipe(&hRead, &hWrite, &saAttr, 0) ) 
	{
		OutputDebugStringA ( "[ERROR]CreatePipe failed!" ) ;
		return 0 ;
	}
	
	// 设置创建子进程的参数
	PROCESS_INFORMATION pi ;
	STARTUPINFO si = { sizeof(STARTUPINFO) } ;
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW ;
	si.hStdOutput = hWrite ;
	si.hStdError = hWrite ;
	si.wShowWindow = SW_HIDE ;
	// 创建子进程
	LPWSTR lpCommand = szCommand.GetBuffer(szCommand.GetLength()) ;
	if ( CreateProcess ( NULL, lpCommand, NULL, NULL, TRUE, 0, NULL, szSysDir, &si, &pi ) )
	{
		// 管道的写入端已经传递给子进程，可以关闭在本进程的句柄
		CloseHandle ( hWrite ) ;

		// 从管道读取数据
		// 此时子进程使用写入端向管道写入数据，本进程使用读取端从管道读取数据
		DWORD dwTotalBytes = 0, dwReadBytes ;
		char  pBuf[1024] ={0} ;
		pDlg->szOutput += "=======================================================\n" ;
		while ( TRUE )
		{
			memset ( pBuf, 0, sizeof(pBuf) ) ;
			if ( !ReadFile ( hRead, pBuf, sizeof(pBuf)-1, &dwReadBytes, NULL ) )
				break ;
			if ( dwReadBytes > 0 ) 
			{
				pDlg->szOutput += pBuf ;
				pDlg->UpdateData ( FALSE ) ;
			}
		}
	}

	CloseHandle ( hRead ) ;
	return 0;
}

void CPipeDlg::OnBnClickedExecute()
{
	CreateThread ( NULL, 0, NewThread, this, 0, NULL ) ;
}

void CPipeDlg::OnDestroy()
{
	CDialog::OnDestroy();
}
