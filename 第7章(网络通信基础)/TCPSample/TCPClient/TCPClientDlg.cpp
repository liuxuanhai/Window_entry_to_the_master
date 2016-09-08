// TCPClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientDlg.h"

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


// CTCPClientDlg 对话框




CTCPClientDlg::CTCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPClientDlg::IDD, pParent)
	, nDataA(0)
	, nDataB(0)
	, nDataC(0)
	, szSrvIp(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATA_A, nDataA);
	DDV_MinMaxUInt(pDX, nDataA, 0, 1000000000);
	DDX_Text(pDX, IDC_DATA_B, nDataB);
	DDV_MinMaxUInt(pDX, nDataB, 0, 1000000000);
	DDX_Text(pDX, IDC_DATA_C, nDataC);
	DDX_Text(pDX, IDC_IP, szSrvIp);
	DDV_MaxChars(pDX, szSrvIp, 14);
}

BEGIN_MESSAGE_MAP(CTCPClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_DATA_A, &CTCPClientDlg::OnEnChangeDataA)
	ON_EN_CHANGE(IDC_DATA_B, &CTCPClientDlg::OnEnChangeDataB)
	ON_BN_CLICKED(IDC_SEND, &CTCPClientDlg::OnBnClickedSend)
	ON_EN_CHANGE(IDC_IP, &CTCPClientDlg::OnEnChangeIp)
END_MESSAGE_MAP()


// CTCPClientDlg 消息处理程序

BOOL CTCPClientDlg::OnInitDialog()
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
	// 取得原始服务地址
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData)  ;

	in_addr addr ;
	char szHostName[DEF_BUF_SIZE] = {0} ;
	gethostname ( szHostName, DEF_BUF_SIZE ) ;
	hostent* pHost = gethostbyname ( szHostName ) ;
	if ( pHost )
	{
		memcpy ( &addr, pHost->h_addr_list[0], pHost->h_length ) ;
		::SetDlgItemTextA ( this->m_hWnd, IDC_IP, inet_ntoa ( addr ) ) ;

	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPClientDlg::OnPaint()
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
HCURSOR CTCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTCPClientDlg::OnEnChangeDataA()
{
	this->UpdateData () ;
}

void CTCPClientDlg::OnEnChangeDataB()
{
	this->UpdateData () ;
}

void CTCPClientDlg::OnBnClickedSend()
{
	// 初始化WinSock
	DWORD dwErrorCode = 0 ;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
	{
		OutputMessage ( L"初始化失败!", GetLastError() ) ;
		return ;
	}

	// 连接原始服务器
	SOCKET LocalSock = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ;
	if ( LocalSock == INVALID_SOCKET )
	{
		OutputMessage ( L"创建socket失败!", GetLastError() ) ;
		return ;
	}
	
	// 设置超时信息
	int nTimeOut = 3000 ;
	::setsockopt(LocalSock,SOL_SOCKET,SO_RCVTIMEO,(char *)&nTimeOut,sizeof(int) ) ;

	// 取得原始服务地址
	char szSrvIp[DEF_BUF_SIZE] = {0} ;
	::GetDlgItemTextA ( this->m_hWnd, IDC_IP, szSrvIp, DEF_BUF_SIZE ) ;
	sockaddr_in RemoteAddr;
	RemoteAddr.sin_family = AF_INET;
	RemoteAddr.sin_port = htons(12345);
	RemoteAddr.sin_addr.s_addr = inet_addr ( szSrvIp ) ;

	// 连接服务器
	if ( connect ( LocalSock, (SOCKADDR*)&RemoteAddr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	{
		OutputMessage ( L"连接失败！", GetLastError() ) ;
		return ;
	}
	
	// 发送请求（数据A和数据B）
	char szData[DEF_BUF_SIZE] = {0} ;
	sprintf ( szData, "%d %d", this->nDataA, this->nDataB ) ;
	int nSendSize = send ( LocalSock, szData, DEF_BUF_SIZE, 0 ) ;
	if ( nSendSize <= 0 )
	{
		OutputMessage ( L"发送数据错误！", GetLastError() ) ;
		return ;
	}

	// 接收服务器处理结果（数据C）
	int nRecvSize = recv ( LocalSock, (char*)&this->nDataC, sizeof(UINT), 0 ) ;
	if ( nRecvSize <= 0 )
	{
		OutputMessage ( L"接受数据错误！", GetLastError() ) ;
		return ;
	}
	
	// 更新界面信息
	this->UpdateData ( FALSE ) ;
	// 关闭套接字句柄
	closesocket ( LocalSock ) ;
	WSACleanup();
}

void CTCPClientDlg::OnEnChangeIp()
{
	this->UpdateData () ;
}

