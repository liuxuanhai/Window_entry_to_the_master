// IPC_ClipboardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IPC_Clipboard.h"
#include "IPC_ClipboardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct _PERSON_INFO {
	WCHAR	szName[128] ;
	UINT	uAge ;
} PERSON_INFO, *PPERSON_INFO ;


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


// CIPC_ClipboardDlg 对话框




CIPC_ClipboardDlg::CIPC_ClipboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPC_ClipboardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPC_ClipboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST, MsgList);
}

BEGIN_MESSAGE_MAP(CIPC_ClipboardDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TEXT, &CIPC_ClipboardDlg::OnBnClickedText)
	ON_BN_CLICKED(IDC_BITMAP, &CIPC_ClipboardDlg::OnBnClickedBitmap)
	ON_BN_CLICKED(IDC_MULT_DATA, &CIPC_ClipboardDlg::OnBnClickedMultData)
	ON_BN_CLICKED(IDC_DELAY_SUBMIT, &CIPC_ClipboardDlg::OnBnClickedDelaySubmit)
	ON_BN_CLICKED(IDC_SELF_DEFINE, &CIPC_ClipboardDlg::OnBnClickedSelfDefine)
	ON_WM_RENDERALLFORMATS()
	ON_WM_RENDERFORMAT()
	ON_WM_DESTROYCLIPBOARD()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CIPC_ClipboardDlg 消息处理程序

BOOL CIPC_ClipboardDlg::OnInitDialog()
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
	this->uDeskopPixelX = ::GetSystemMetrics ( SM_CXSCREEN ) ;
	this->uDeskopPixelY = ::GetSystemMetrics ( SM_CYSCREEN ) ;

	this->uNewFormat = RegisterClipboardFormat ( L"MY_TEST_FORMAT" ) ;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIPC_ClipboardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPC_ClipboardDlg::OnPaint()
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
HCURSOR CIPC_ClipboardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID ShowErrorMsg ( DWORD dwErrorCode )
{
	WCHAR szMsg[128] = {0} ;
	wsprintf ( szMsg, L"设置剪贴板数据失败！(ErrorCode=%d)", dwErrorCode ) ;
	MessageBox ( 0, szMsg, 0, 0 ) ;
}

// 写入文本数据
void CIPC_ClipboardDlg::OnBnClickedText()
{	
	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 首先清空剪贴板
	::EmptyClipboard();
	
	// 设置剪贴板文本数据
	this->WriteIntoText () ;

	// 关闭剪贴板
	::CloseClipboard();
}

// 写入位图数据
void CIPC_ClipboardDlg::OnBnClickedBitmap()
{
	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 首先清空剪贴板
	::EmptyClipboard();

	// 设置剪贴板位图数据
	this->WriteIntoBitmap ( ) ;

	// 关闭剪贴板
	::CloseClipboard();
}

void CIPC_ClipboardDlg::OnBnClickedSelfDefine()
{
	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 首先清空剪贴板
	::EmptyClipboard();

	// 写入自定义数据
	this->WriteIntoSelfDefineData () ;

	// 关闭剪贴板
	::CloseClipboard();
}

// 延迟提交
void CIPC_ClipboardDlg::OnBnClickedDelaySubmit()
{
	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 首先清空剪贴板
	::EmptyClipboard();

	// 设置为延迟提交
	::SetClipboardData ( CF_BITMAP, NULL ) ;

	// 关闭剪贴板
	::CloseClipboard();
}

// 提交多项数据
void CIPC_ClipboardDlg::OnBnClickedMultData()
{
	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 首先清空剪贴板
	::EmptyClipboard();

	// 写入3种数据
	this->WriteIntoText () ;
	this->WriteIntoBitmap () ;
	this->WriteIntoSelfDefineData () ;

	// 关闭剪贴板
	::CloseClipboard();
}

void CIPC_ClipboardDlg::OnRenderAllFormats()
{
	CDialog::OnRenderAllFormats();

	this->MsgList.AddString ( L"WM_RENDERALLFORMAT" ) ;
	this->MsgList.SetCurSel ( this->MsgList.GetCount()-1 ) ;
}

void CIPC_ClipboardDlg::OnRenderFormat(UINT nFormat)
{
	this->MsgList.AddString ( L"WM_RENDERFORMAT" ) ;
	this->MsgList.SetCurSel ( this->MsgList.GetCount()-1 ) ;

	this->WriteIntoBitmap () ;

	CDialog::OnRenderFormat(nFormat);
}

void CIPC_ClipboardDlg::OnDestroyClipboard()
{
	CDialog::OnDestroyClipboard();

	this->MsgList.AddString ( L"WM_DESTORYCLIPBOARD" ) ;
	this->MsgList.SetCurSel ( this->MsgList.GetCount()-1 ) ;
}

void CIPC_ClipboardDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

// 写入文本数据
VOID CIPC_ClipboardDlg::WriteIntoText ()
{
	// 初始化文本信息
	const char szText[] = "文本剪贴板使用示例！" ;
	DWORD dwTextLen = strlen ( szText ) ;
	HANDLE hMem = GlobalAlloc ( GHND, dwTextLen + 1 ) ;
	if ( hMem )
	{
		LPBYTE lpData = (LPBYTE)GlobalLock ( hMem ) ;
		memcpy ( lpData, szText, dwTextLen ) ;
		lpData[dwTextLen] = 0 ;
		GlobalUnlock ( hMem ) ;

		// 设置剪贴板文本数据
		if ( ::SetClipboardData ( CF_TEXT, hMem ) == NULL )
			ShowErrorMsg ( GetLastError () ) ;
	}
	else
		this->MessageBox ( L"[ERROR]GlobalAlloc!" ) ;
}

// 写入位图数据
VOID CIPC_ClipboardDlg::WriteIntoBitmap ()
{	
	// 取得当前屏幕位图
	CDC			dc, ScreenDc ;
	CBitmap		ScreenBmp ;
	dc.CreateDC ( L"DISPLAY", NULL, NULL, NULL ) ;
	ScreenDc.CreateCompatibleDC ( &dc ) ;
	ScreenBmp.CreateCompatibleBitmap ( &dc, uDeskopPixelX, uDeskopPixelY ) ;
	ScreenDc.SelectObject ( &ScreenBmp ) ;
	ScreenDc.BitBlt ( 0, 0, uDeskopPixelX, uDeskopPixelY, &dc, 0, 0, SRCCOPY ) ;

	// 设置剪贴板文本数据
	if ( ::SetClipboardData ( CF_BITMAP, ScreenBmp.GetSafeHandle() ) == NULL )
		ShowErrorMsg ( GetLastError () ) ;

	ScreenBmp.DeleteObject () ;
	ScreenDc.DeleteDC () ;
}

// 写入自定义数据
VOID CIPC_ClipboardDlg::WriteIntoSelfDefineData ()
{	
	PERSON_INFO PersonInfo = { L"zhang", 23 } ;

	// 初始化自定义格式
	DWORD dwLen = sizeof(PERSON_INFO) ;
	HANDLE hMem = GlobalAlloc ( GHND, dwLen + 1 ) ;
	if ( hMem )
	{
		LPBYTE lpData = (LPBYTE)GlobalLock ( hMem ) ;
		memcpy ( lpData, &PersonInfo, dwLen ) ;
		lpData[dwLen] = 0 ;
		GlobalUnlock ( hMem ) ;

		::SetClipboardData ( this->uNewFormat, hMem ) ;
	}
}