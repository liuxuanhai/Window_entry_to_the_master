// IPC_Clipboard_ReadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IPC_Clipboard_Read.h"
#include "IPC_Clipboard_ReadDlg.h"

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


// CIPC_Clipboard_ReadDlg 对话框




CIPC_Clipboard_ReadDlg::CIPC_Clipboard_ReadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPC_Clipboard_ReadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPC_Clipboard_ReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, InfoList);
}

BEGIN_MESSAGE_MAP(CIPC_Clipboard_ReadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GET_TEXT, &CIPC_Clipboard_ReadDlg::OnBnClickedGetText)
	ON_BN_CLICKED(IDC_GET_BITMAP, &CIPC_Clipboard_ReadDlg::OnBnClickedGetBitmap)
	ON_BN_CLICKED(IDC_GET_SELF_DEFINE, &CIPC_Clipboard_ReadDlg::OnBnClickedGetSelfDefine)
	ON_BN_CLICKED(IDC_GET_ALL, &CIPC_Clipboard_ReadDlg::OnBnClickedGetAll)
END_MESSAGE_MAP()


// CIPC_Clipboard_ReadDlg 消息处理程序

BOOL CIPC_Clipboard_ReadDlg::OnInitDialog()
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

void CIPC_Clipboard_ReadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPC_Clipboard_ReadDlg::OnPaint()
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
HCURSOR CIPC_Clipboard_ReadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIPC_Clipboard_ReadDlg::OnBnClickedGetText()
{
	CString TempStr ;
	this->InfoList.ResetContent () ;
	::OpenClipboard ( this->GetSafeHwnd() ) ;
	this->ReadOutText () ;
	::CloseClipboard () ;
}

void CIPC_Clipboard_ReadDlg::OnBnClickedGetBitmap()
{
	CString TempStr ;
	this->InfoList.ResetContent () ;
	::OpenClipboard ( this->GetSafeHwnd() ) ;
	this->ReadOutBitmap () ;
	::CloseClipboard () ;
}

void CIPC_Clipboard_ReadDlg::OnBnClickedGetSelfDefine()
{
	CString TempStr = L"[自定义]" ;
	this->InfoList.ResetContent () ;
	::OpenClipboard ( this->GetSafeHwnd() ) ;
	this->ReadOutSelfDefineData () ;
	::CloseClipboard () ;
}

void CIPC_Clipboard_ReadDlg::OnBnClickedGetAll()
{
	this->InfoList.ResetContent () ;

	// 打开剪贴板
	::OpenClipboard ( this->GetSafeHwnd() ) ;

	// 循环枚举
	UINT format = 0;	// 从第一种格式值开始枚举
	UINT uNewFormat = RegisterClipboardFormat ( L"MY_TEST_FORMAT" ) ;
	while ( format = EnumClipboardFormats ( format ) )
	{
		// 这里只处理这3种格式
		if ( format == CF_TEXT )
			this->ReadOutText () ;
		else if ( format == CF_BITMAP )
			this->ReadOutBitmap () ;
		else if ( format == uNewFormat )
			this->ReadOutSelfDefineData () ;
	}
	::CloseClipboard (); 
}


VOID CIPC_Clipboard_ReadDlg::ReadOutText ()
{
	CString TempStr ;
	HANDLE hMem = ::GetClipboardData ( CF_TEXT ) ;
	if ( hMem )
	{
		TempStr = L"[文本]" ;
		TempStr +=(LPSTR)GlobalLock ( hMem ) ;
		GlobalUnlock ( hMem ) ;
		this->InfoList.AddString ( TempStr ) ;
	}
}

VOID CIPC_Clipboard_ReadDlg::ReadOutBitmap ()
{
	CString TempStr ;
	HBITMAP hBitmap = (HBITMAP)::GetClipboardData ( CF_BITMAP ) ;
	if ( hBitmap )
	{
		BITMAP BmpInfo ;
		::GetObject ( hBitmap, sizeof(BITMAP), &BmpInfo ) ;
		TempStr.Format ( L"[位图]Size=(%d,%d)", BmpInfo.bmWidth, BmpInfo.bmHeight ) ;
		this->InfoList.AddString ( TempStr ) ;
	}
}

VOID CIPC_Clipboard_ReadDlg::ReadOutSelfDefineData ()
{
	CString TempStr ;
	UINT uNewFormat = RegisterClipboardFormat ( L"MY_TEST_FORMAT" ) ;
	HANDLE hMem = ::GetClipboardData ( uNewFormat ) ;
	if ( hMem )
	{
		PPERSON_INFO pInfo = (PPERSON_INFO)GlobalLock ( hMem ) ;
		TempStr.Format ( L"[自定义]Name=%s Age=%d", pInfo->szName, pInfo->uAge ) ;
		GlobalUnlock ( hMem ) ;
		this->InfoList.AddString ( TempStr ) ;
	}
}