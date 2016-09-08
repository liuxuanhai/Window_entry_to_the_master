// MemSearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MemSearch.h"
#include "MemSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SelProcess.h"

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


// CMemSearchDlg 对话框




CMemSearchDlg::CMemSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemSearchDlg::IDD, pParent)
	, dwTagValue(0)
	, dwTagAddr(0)
	, dwNewValue(0)
	, szPathInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR_LIST, AddrList);
	DDX_Text(pDX, IDC_TAG_VALUE, dwTagValue);
	DDX_Text(pDX, IDC_TAG_ADDR, dwTagAddr);
	DDX_Text(pDX, IDC_NEW_VALUE, dwNewValue);
	DDX_Text(pDX, IDC_PROCESS_PATH, szPathInfo);
}

BEGIN_MESSAGE_MAP(CMemSearchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SELECT, &CMemSearchDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_FIRST_SEARCH, &CMemSearchDlg::OnBnClickedFirstSearch)
	ON_BN_CLICKED(IDC_RE_SEARCH, &CMemSearchDlg::OnBnClickedReSearch)
	ON_BN_CLICKED(IDC_MODIFY, &CMemSearchDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_EXIT, &CMemSearchDlg::OnBnClickedExit)
	ON_LBN_SELCHANGE(IDC_ADDR_LIST, &CMemSearchDlg::OnLbnSelchangeAddrList)
	ON_EN_CHANGE(IDC_TAG_VALUE, &CMemSearchDlg::OnEnChangeTagValue)
	ON_EN_CHANGE(IDC_NEW_VALUE, &CMemSearchDlg::OnEnChangeNewValue)
END_MESSAGE_MAP()


// CMemSearchDlg 消息处理程序

BOOL CMemSearchDlg::OnInitDialog()
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
	TOKEN_PRIVILEGES tkp; 
	HANDLE hToken; 
	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken)) //打开当前进程失败 
		return FALSE; 
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid); //查看当前权限
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); //调整权限，如上设置

	this->VMInfo.VM_GetSysInfo () ;

	RegisterHotKey ( this->m_hWnd, 1001, MOD_CONTROL, VK_F5 ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMemSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMemSearchDlg::OnPaint()
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
HCURSOR CMemSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMemSearchDlg::OnBnClickedSelect()
{
	CSelProcess dlg ;
	if ( dlg.DoModal () == IDOK )
	{
		this->dwSelectedProcessId = dlg.GetSelProcessId () ;
		this->szSelectedProcessName = dlg.GetSelProcessName () ;
		this->szPathInfo.Format ( L"[%d]%s", this->dwSelectedProcessId, this->szSelectedProcessName ) ;

		this->AddrList.ResetContent () ;
		this->dwTagAddr = 0 ;
		this->UpdateData ( FALSE ) ;	

		this->VMInfo.VM_GetMemInfoList ( this->dwSelectedProcessId ) ;
	}
}

void CMemSearchDlg::OnBnClickedFirstSearch()
{
	this->VMInfo.VM_FirstSearch ( this->dwSelectedProcessId, this->dwTagValue ) ;
	this->UpdateAddrListInfo () ;
}

void CMemSearchDlg::OnBnClickedReSearch()
{
	this->VMInfo.VM_ReSearch ( this->dwSelectedProcessId, this->dwTagValue ) ;
	this->UpdateAddrListInfo () ;
}

void CMemSearchDlg::OnBnClickedModify()
{
	this->VMInfo.VM_Modify ( this->dwSelectedProcessId, this->dwTagAddr, this->dwNewValue ) ;
}

void CMemSearchDlg::OnBnClickedExit()
{
	CDialog::OnCancel () ;
}

void CMemSearchDlg::UpdateAddrListInfo ()
{
	this->AddrList.ResetContent () ;

	CString		TempStr ;
	list<DWORD>::iterator p ;
	for ( p =this->VMInfo.AddrList.begin(); p != this->VMInfo.AddrList.end(); p++ )
	{
		TempStr.Format ( L"0x%08X", *p ) ;
		this->AddrList.AddString ( TempStr ) ;
	}
}

void CMemSearchDlg::OnLbnSelchangeAddrList()
{
	int nCurSel = this->AddrList.GetCurSel () ;
	if ( nCurSel != -1 )
	{
		WCHAR szValue[32] ;
		this->AddrList.GetText ( nCurSel, szValue ) ;
		swscanf ( szValue, L"0x%08X", &this->dwTagAddr ) ;
		this->UpdateData ( FALSE ) ;
	}
}

void CMemSearchDlg::OnEnChangeTagValue()
{
	this->UpdateData () ;
}

void CMemSearchDlg::OnEnChangeNewValue()
{
	this->UpdateData () ;
}

BOOL CMemSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_HOTKEY )
	{
		switch ( pMsg->wParam )
		{
		case 1001:
			this->SetActiveWindow () ;
			break ;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}