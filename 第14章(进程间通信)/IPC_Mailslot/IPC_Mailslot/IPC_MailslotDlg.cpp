// IPC_MailslotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IPC_Mailslot.h"
#include "IPC_MailslotDlg.h"

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


// CIPC_MailslotDlg 对话框




CIPC_MailslotDlg::CIPC_MailslotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPC_MailslotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPC_MailslotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFOLIST, InfoList);
}

BEGIN_MESSAGE_MAP(CIPC_MailslotDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FLUSH, &CIPC_MailslotDlg::OnBnClickedFlush)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CIPC_MailslotDlg 消息处理程序

BOOL CIPC_MailslotDlg::OnInitDialog()
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
	this->StartServer() ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIPC_MailslotDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPC_MailslotDlg::OnPaint()
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
HCURSOR CIPC_MailslotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIPC_MailslotDlg::OnBnClickedFlush()
{
	DWORD dwNextSize = 0, dwMsgCount = 0, dwReadBytes = 0;
	if ( GetMailslotInfo ( this->hMailslot, NULL, &dwNextSize, &dwMsgCount, NULL ) )
	{
		for ( int i = 0; i < dwMsgCount; i++ )
		{
			LPBYTE lpBuf = new BYTE[dwNextSize+2] ;
			ReadFile( this->hMailslot, lpBuf, dwNextSize, &dwReadBytes,NULL);	
			this->InfoList.AddString( (LPWSTR)lpBuf ) ;
		}
	}
}

VOID CIPC_MailslotDlg::StartServer()
{
	this->hMailslot = CreateMailslot( MAILSLOT_NAME, 0, MAILSLOT_WAIT_FOREVER,NULL);
	if ( this->hMailslot == INVALID_HANDLE_VALUE )
		this->MessageBox ( L"无法创建邮槽!" ) ;
}

VOID CIPC_MailslotDlg::StopServer()
{
	if ( this->hMailslot )
		CloseHandle( this->hMailslot ) ;
}
void CIPC_MailslotDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	this->StopServer() ;
}
