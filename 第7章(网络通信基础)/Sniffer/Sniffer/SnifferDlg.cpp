// SnifferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sniffer.h"
#include "SnifferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>
#pragma comment ( lib, "ws2_32.lib" ) 

#define   SIO_RCVALL   _WSAIOW(IOC_VENDOR,1) 

#define DEF_BUF_SIZE 2048

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


// CSnifferDlg 对话框




CSnifferDlg::CSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnifferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->isMonitor = FALSE ;
}

void CSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACK_LIST, m_PackList);
}

BEGIN_MESSAGE_MAP(CSnifferDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START, &CSnifferDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CSnifferDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_EMPTY, &CSnifferDlg::OnBnClickedEmpty)
END_MESSAGE_MAP()


// CSnifferDlg 消息处理程序

BOOL CSnifferDlg::OnInitDialog()
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
	this->m_PackList.InsertColumn ( 0, L"ID",		LVCFMT_CENTER, 30 ) ;
	this->m_PackList.InsertColumn ( 1, L"长度",		LVCFMT_CENTER, 60 ) ;
	this->m_PackList.InsertColumn ( 2, L"类型",		LVCFMT_CENTER, 40 ) ;
	this->m_PackList.InsertColumn ( 3, L"源IP",		LVCFMT_CENTER, 120 ) ;
	this->m_PackList.InsertColumn ( 4, L"源端口",	LVCFMT_CENTER, 60 ) ;
	this->m_PackList.InsertColumn ( 5, L"目的IP",	LVCFMT_CENTER, 120 ) ;
	this->m_PackList.InsertColumn ( 6, L"目的端口", LVCFMT_CENTER, 60 ) ;
	this->m_PackList.SetExtendedStyle ( this->m_PackList.GetExtendedStyle()|LVS_EX_FULLROWSELECT ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSnifferDlg::OnPaint()
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
HCURSOR CSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnifferDlg::OnBnClickedStart()
{
	if ( this->isMonitor )
	{
		this->MessageBox ( L"已经在工作！" ) ;
		return ;
	}

	HANDLE hThread = CreateThread ( NULL, 0, MonitorThread, this, 0, NULL ) ;
	if ( hThread == NULL )
	{
		this->MessageBox ( L"创建监视器失败!" ) ;
	}
	else
		this->isMonitor = TRUE ;
}

void CSnifferDlg::OnBnClickedStop()
{
	this->isMonitor = FALSE ;
}

void CSnifferDlg::OnBnClickedEmpty()
{
	this->m_PackList.DeleteAllItems () ;
}

// 添加数据包信息
void CSnifferDlg::AddPackInfo ( LPPACK_INFO lpPackInfo )
{
	UINT nCount = this->m_PackList.GetItemCount () ;

	CString TempStr ;
	TempStr.Format ( L"%d", nCount+1 ) ;
	this->m_PackList.InsertItem ( nCount, TempStr ) ;

	TempStr.Format ( L"%d", lpPackInfo->nLength ) ;
	this->m_PackList.SetItemText ( nCount, 1, TempStr ) ;

	if ( lpPackInfo->nProtocol == IPPROTO_TCP )
		this->m_PackList.SetItemText ( nCount, 2, L"TCP" ) ;
	else if ( lpPackInfo->nProtocol == IPPROTO_UDP )
		this->m_PackList.SetItemText ( nCount, 2, L"UDP" ) ;

	in_addr addr ;
	WCHAR	szTempBufW[DEF_BUF_SIZE] = {0} ;

	addr.S_un.S_addr = lpPackInfo->nSourIp ;
	MultiByteToWideChar (CP_ACP, 0, inet_ntoa(addr), -1, szTempBufW, sizeof(szTempBufW) );
	this->m_PackList.SetItemText ( nCount, 3, szTempBufW ) ;

	TempStr.Format ( L"%d", htons(lpPackInfo->nSourPort) ) ;
	this->m_PackList.SetItemText ( nCount, 4, TempStr ) ;

	addr.S_un.S_addr = lpPackInfo->nDestIp ;
	MultiByteToWideChar (CP_ACP, 0, inet_ntoa(addr), -1, szTempBufW, sizeof(szTempBufW) );
	this->m_PackList.SetItemText ( nCount, 5, szTempBufW ) ;

	TempStr.Format ( L"%d", htons(lpPackInfo->nDestPort) ) ;
	this->m_PackList.SetItemText ( nCount, 6, TempStr ) ;

	this->m_PackList.EnsureVisible( nCount, FALSE ) ;
	this->m_PackList.SetItemState ( nCount, LVIS_SELECTED,LVIS_SELECTED) ;
	if ( nCount > 0 )
		this->m_PackList.SetItemState ( nCount-1, 0, LVIS_SELECTED) ;
	this->m_PackList.SetFocus ( ) ;
}

// 监视线程
DWORD WINAPI MonitorThread ( LPVOID lParam )
{
	PACK_INFO	PackInfo = {0} ;
	int nRecvSize = 0 ;
	char szPackBuf[DEF_BUF_SIZE] = {0} ;

	// 取得窗体指针
	CSnifferDlg* pDlg = (CSnifferDlg*)lParam ;

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		return 0 ;

	// 获取本地地址信息
	sockaddr_in LocalAddr ;
	char szLocalName[DEF_BUF_SIZE] = {0} ;
	gethostname ( szLocalName, DEF_BUF_SIZE ) ;
	hostent* pHost = gethostbyname ( szLocalName ) ;
	if ( pHost != NULL )
	{
		LocalAddr.sin_family = AF_INET ;
		LocalAddr.sin_port = htons(0) ;
		memcpy ( &(LocalAddr.sin_addr.s_addr), pHost->h_addr_list[0], pHost->h_length ) ;
	}
	else
		return 0 ;

	// 创建监听套接字
	SOCKET MonSock = socket ( AF_INET, SOCK_RAW, IPPROTO_IP ) ;
	if ( MonSock == INVALID_SOCKET )
		return 0 ;

	// 绑定地址信息到套接字
	if ( bind ( MonSock, (sockaddr*)&LocalAddr, sizeof(sockaddr) ) == SOCKET_ERROR )
		return 0 ;

	// 设置为混杂模式，收所有IP包
	DWORD dwValue = 1 ;
	if ( ioctlsocket ( MonSock, SIO_RCVALL, &dwValue ) != 0 )
		return 0 ;

	// 检测控制标志，是否继续监视
	while ( pDlg->isMonitor )
	{
		// 取得数据包
		nRecvSize = recv ( MonSock, szPackBuf, DEF_BUF_SIZE, 0 ) ;
		if ( nRecvSize > 0 )
		{
			// 解析IP包头
			PIP_HEADER pIpHeader = (PIP_HEADER)szPackBuf ;
			PackInfo.nLength	= nRecvSize ;
			PackInfo.nProtocol	= (USHORT)pIpHeader->bProtocol ;
			PackInfo.nSourIp	= pIpHeader->nSourIp ;
			PackInfo.nDestIp	= pIpHeader->nDestIp ;
			UINT nIpHeadLength = ( pIpHeader->bVerAndHLen & 0x0F ) * sizeof(UINT) ;			// IP数据包头长度

			// 只检测TCP和UDP包
			switch ( pIpHeader->bProtocol )
			{
			case IPPROTO_TCP:
				{
					// 取得TCP数据包端口号
					PTCP_HEADER pTcpHeader = (PTCP_HEADER)&szPackBuf[nIpHeadLength] ;
					PackInfo.nSourPort = pTcpHeader->nSourPort ;
					PackInfo.nDestPort = pTcpHeader->nDestPort ;
					pDlg->AddPackInfo ( &PackInfo ) ;	// 输出到界面
				}
				break ;
			case IPPROTO_UDP:
				{
					// 取得UDP数据包端口号
					PUDP_HEADER pUdpHeader = (PUDP_HEADER)&szPackBuf[nIpHeadLength] ;
					PackInfo.nSourPort = pUdpHeader->nSourPort ;
					PackInfo.nDestPort = pUdpHeader->nDestPort ;
					pDlg->AddPackInfo ( &PackInfo ) ;	// 输出到界面
				}
				break ;
			}
		}
		Sleep ( 100 ) ;
	}
}