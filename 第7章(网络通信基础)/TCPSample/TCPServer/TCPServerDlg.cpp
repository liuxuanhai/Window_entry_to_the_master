// TCPServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPServer.h"
#include "TCPServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>
#pragma comment ( lib, "ws2_32.lib" )

#define DEF_BUF_SIZE 1024

inline VOID OutputMessage ( LPWSTR lpMessage, DWORD dwErrorCode )
{
	WCHAR szMsg[DEF_BUF_SIZE] = {0} ;
	wsprintf ( szMsg, L"%s, ErrorCode=%d", lpMessage, dwErrorCode ) ;
	::MessageBox ( 0, szMsg, 0, 0 ) ;
}

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


// CTCPServerDlg 对话框




CTCPServerDlg::CTCPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPServerDlg::IDD, pParent)
	, nDataA(0)
	, nDataB(0)
	, nDataC(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATA_A, nDataA);
	DDX_Text(pDX, IDC_DATA_B, nDataB);
	DDX_Text(pDX, IDC_DATA_C, nDataC);
}

BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序

BOOL CTCPServerDlg::OnInitDialog()
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
	this->bStatus = TRUE ;
	this->hThread = CreateThread ( NULL, 0, ServerThread, this, NULL, NULL ) ;
	if ( this->hThread == NULL )
	{
		this->MessageBox ( L"创建服务线程失败！" ) ;
		this->bStatus = FALSE ;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPServerDlg::OnPaint()
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
HCURSOR CTCPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI ServerThread ( LPVOID lParam )
{
	// 窗体对象指针
	CTCPServerDlg* pDlg = (CTCPServerDlg*)lParam ;

	// 初始化WinSock
	DWORD dwErrorCode = 0 ;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		OutputMessage ( L"初始化失败！", GetLastError() ) ;
		return 0 ;
	}

	// 创建套接字
	SOCKET LocalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (LocalSocket == INVALID_SOCKET )
	{
		OutputMessage ( L"创建socket失败！", GetLastError() ) ;
		return 0;
	}

	// 取得本地地址信息
	char szHostName[DEF_BUF_SIZE] = {0} ;
	gethostname ( szHostName, DEF_BUF_SIZE ) ;
	hostent* pHost = gethostbyname ( szHostName ) ;
	if ( pHost == NULL )
		return FALSE ;

	sockaddr_in SrvAddr;
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_port = htons(12345);
	memcpy ( &(SrvAddr.sin_addr.s_addr), pHost->h_addr_list[0], pHost->h_length ) ;

	// 绑定地址信息到套接字
	if ( bind ( LocalSocket, (SOCKADDR*)&SrvAddr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	{
		OutputMessage ( L"绑定地址信息失败!", GetLastError() ) ;
		return 0;
	}

	// 监听
	if ( listen(LocalSocket,SOMAXCONN) == SOCKET_ERROR )
	{
		OutputMessage ( L"监听失败！", GetLastError() ) ;
		return 0;
	}

	while ( TRUE )
	{
		int len = sizeof(SOCKADDR) ;
		sockaddr_in ConnAddr ;
		// 接受连接
		SOCKET ConnSock = accept ( LocalSocket, (SOCKADDR*)&ConnAddr, &len ) ;
		if ( ConnSock == INVALID_SOCKET )
		{
			OutputMessage ( L"接受连接失败！", GetLastError() ) ;
			continue ;
		}

		// 接收数据
		char szData[DEF_BUF_SIZE] = {0} ;
		int nRecvSize = recv ( ConnSock, szData, DEF_BUF_SIZE, 0 ) ;
		if ( nRecvSize <= 0 )
		{
			OutputMessage ( L"接收数据错误！", GetLastError() ) ;
			continue ;
		}

		// 服务器所提供的服务，这里是简单的加法运算
		sscanf ( szData, "%d %d", &pDlg->nDataA, &pDlg->nDataB ) ;
		pDlg->nDataC = pDlg->nDataA + pDlg->nDataB ;
		pDlg->SetDlgItemInt ( IDC_DATA_A, pDlg->nDataA ) ;
		pDlg->SetDlgItemInt ( IDC_DATA_B, pDlg->nDataB ) ;
		pDlg->SetDlgItemInt ( IDC_DATA_C, pDlg->nDataC ) ;

		// 发送数据
		int nSendSize = send ( ConnSock, (char*)&pDlg->nDataC, sizeof(UINT), 0 ) ;
		if ( nSendSize <= 0 )
		{
			OutputMessage ( L"发送数据错误！", GetLastError() ) ;
			continue ;
		}

		closesocket ( ConnSock ) ;
	}

	closesocket ( LocalSocket ) ;
	WSACleanup();

	return 0 ;
}