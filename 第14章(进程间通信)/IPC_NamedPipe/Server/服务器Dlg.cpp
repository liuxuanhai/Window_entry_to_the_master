// 服务器Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "服务器.h"
#include "服务器Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFFER_SIZE 1024
CMyDlg* pMyDlg ;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
	nResValue = 0;
	nFirst = 0;
	nSecond = 0;
	nMaxConn = 3;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset ( PipeInst, 0, sizeof(PipeInst) ) ;
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
	DDX_Text(pDX, IDC_RESULT, nResValue);
	DDX_Text(pDX, IDC_FIRST, nFirst);
	DDX_Text(pDX, IDC_SECOND, nSecond);
	DDX_Text(pDX, IDC_MAXCONN, nMaxConn);
	DDV_MinMaxUInt(pDX, nMaxConn, 1, 100);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MAXCONN, OnChangeMaxconn)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	pMyDlg = this ;

	HANDLE hMutex = OpenMutex ( MUTEX_ALL_ACCESS, true, "NamedPipeServer" ) ;
	if ( hMutex == NULL )
		CreateMutex ( NULL, true, "NamedPipeServer" ) ;
	else
	{
		this->MessageBox ( "服务器只能运行一个实例" ) ;
		TerminateProcess ( GetCurrentProcess(), 0 ) ;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnChangeMaxconn() 
{
	this->UpdateData() ;
}

// 线程服务函数
// 工作线程，每个管道实例对应一个工作线程
UINT ServerThread ( LPVOID lpParameter )
{
	DWORD  nReadByte = 0, nWriteByte = 0, dwByte = 0 ;  
	char  szBuf[MAX_BUFFER_SIZE] = {0} ;
	PIPE_INSTRUCT  CurPipeInst = *(PIPE_INSTRUCT*)lpParameter ;
	OVERLAPPED OverLapStruct = { 0, 0, 0, 0, CurPipeInst.hEvent } ;
	while ( true )
	{
		memset ( szBuf, 0, sizeof(szBuf) ) ;  
		// 命名管道的连接函数，等待客户端的连接（只针对NT）
		ConnectNamedPipe ( CurPipeInst.hPipe, &OverLapStruct ) ;
		// 实现重叠I/0，等待OVERLAPPED结构的事件对象
		WaitForSingleObject ( CurPipeInst.hEvent, INFINITE ) ;
		// 检测I/0是否已经完成，如果未完成，意味着该事件对象是人工设置，即服务需要停止
		if ( !GetOverlappedResult ( CurPipeInst.hPipe, &OverLapStruct, &dwByte, true ) )
			break ;
		// 从管道中读取客户端的请求信息
		if ( !ReadFile ( CurPipeInst.hPipe, szBuf, MAX_BUFFER_SIZE, &nReadByte, NULL ) )
		{
			MessageBox ( 0, "读取管道错误！", 0, 0 ) ;
			break ;
		}

		int a, b ;
		sscanf ( szBuf, "%d %d", &a, &b ) ;
		pMyDlg->nFirst    = a ;
		pMyDlg->nSecond    = b ;
		pMyDlg->nResValue  = a + b ;
		memset ( szBuf, 0, sizeof(szBuf) ) ;
		sprintf ( szBuf, "%d", pMyDlg->nResValue ) ;
		// 把反馈信息写入管道
		WriteFile ( CurPipeInst.hPipe, szBuf, strlen(szBuf), &nWriteByte, NULL ) ;
		pMyDlg->SetDlgItemInt ( IDC_FIRST, a, true ) ;
		pMyDlg->SetDlgItemInt ( IDC_SECOND, b, true ) ;
		pMyDlg->SetDlgItemInt ( IDC_RESULT, pMyDlg->nResValue, true ) ;
		// 断开客户端的连接，以便等待下一客户的到来
		DisconnectNamedPipe ( CurPipeInst.hPipe ) ;
	}

	return 0 ;
}

// 启动服务
void CMyDlg::OnStart() 
{
	CString lpPipeName = "\\\\.\\Pipe\\NamedPipe" ;
	for ( UINT i = 0; i < nMaxConn; i++ )
	{
		// 创建管道实例
		PipeInst[i].hPipe =  CreateNamedPipe ( lpPipeName, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, \
			PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, nMaxConn, 0, 0, 1000, NULL ) ;
		if ( PipeInst[i].hPipe == INVALID_HANDLE_VALUE )
		{
			DWORD dwErrorCode = GetLastError () ;
			this->MessageBox ( "创建管道错误！" ) ;
			return ;
		}
		// 为每个管道实例创建一个事件对象，用于实现重叠IO
		PipeInst[i].hEvent  =  CreateEvent ( NULL, false, false, false ) ;
		// 为每个管道实例分配一个线程，用于响应客户端的请求
		PipeInst[i].hTread = AfxBeginThread ( ServerThread, &PipeInst[i], THREAD_PRIORITY_NORMAL ) ;
	}

	this->SetWindowText ( "[第14章]命名管道—服务器(运行)" ) ;
	this->MessageBox ( "服务启动成功" ) ;
}
// 停止服务
void CMyDlg::OnStop() 
{
	DWORD dwNewMode = PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_NOWAIT ;
	for ( UINT i = 0; i < nMaxConn; i++ )
	{
		// 设置重叠I/O的事件，使得工作线程安全结束
		SetEvent ( PipeInst[i].hEvent ) ;
		CloseHandle ( PipeInst[i].hTread ) ;
		CloseHandle ( PipeInst[i].hPipe ) ;
	}

	this->SetWindowText ( "[第14章]命名管道—服务器" ) ;
	this->MessageBox ( "停止启动成功" ) ;
}
