// VirtualMemoryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VirtualMemory.h"
#include "VirtualMemoryDlg.h"

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


// CVirtualMemoryDlg 对话框




CVirtualMemoryDlg::CVirtualMemoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualMemoryDlg::IDD, pParent)
	, szProcess(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VM_LIST, VMList);
	DDX_Text(pDX, IDC_TAG_PROCESS, szProcess);
	DDX_Control(pDX, IDC_CHECK1, CommitCheck);
	DDX_Control(pDX, IDC_CHECK2, ReserveCheck);
	DDX_Control(pDX, IDC_CHECK3, FreeCheck);
}

BEGIN_MESSAGE_MAP(CVirtualMemoryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FLUSH, &CVirtualMemoryDlg::OnBnClickedFlush)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SELECT, &CVirtualMemoryDlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_CHECK1, &CVirtualMemoryDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CVirtualMemoryDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CVirtualMemoryDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CVirtualMemoryDlg 消息处理程序

BOOL CVirtualMemoryDlg::OnInitDialog()
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
	this->VMInfo.VM_GetSysInfo () ;
	this->VMInfo.VM_GetMemStatus () ;

	this->VMList.InsertColumn ( 0, L"ID", LVCFMT_LEFT, 30 ) ;
	this->VMList.InsertColumn ( 1, L"地址", LVCFMT_LEFT, 60 ) ;
	this->VMList.InsertColumn ( 2, L"大小", LVCFMT_CENTER, 60 ) ;
	this->VMList.InsertColumn ( 3, L"状态", LVCFMT_CENTER, 65 ) ;
	this->VMList.InsertColumn ( 4, L"类型", LVCFMT_CENTER, 65 ) ;
	this->VMList.InsertColumn ( 5, L"初始保护", LVCFMT_CENTER, 70 ) ;
	this->VMList.InsertColumn ( 6, L"访问保护", LVCFMT_CENTER, 70 ) ;
	this->VMList.SetExtendedStyle( this->VMList.GetExtendedStyle()|LVS_EX_FULLROWSELECT ) ;

	this->CommitCheck.SetCheck ( BST_CHECKED ) ;
	this->ReserveCheck.SetCheck ( BST_CHECKED ) ;
	this->FreeCheck.SetCheck ( BST_UNCHECKED ) ;

	this->SetTimer ( 1, 1000, NULL ) ;
	this->dwSelProcessId = GetCurrentProcessId () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVirtualMemoryDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVirtualMemoryDlg::OnPaint()
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
HCURSOR CVirtualMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVirtualMemoryDlg::OnBnClickedFlush()
{
	this->VMList.DeleteAllItems () ;
	
	if ( this->VMInfo.VM_GetMemInfoList ( this->dwSelProcessId ) == FALSE )
	{
		this->MessageBox ( L"获取信息失败！" ) ;
		return ;
	}

	CString		TempStr ;
	UINT nIndex = 0 ;
	list<MEMORY_BASIC_INFORMATION>::iterator p ;
	for ( p = this->VMInfo.MemList.begin(); p != this->VMInfo.MemList.end(); p++ ) 
	{
		if ( ( p->State == MEM_COMMIT && this->CommitCheck.GetCheck()==BST_UNCHECKED ) || \
			 ( p->State == MEM_RESERVE && this->ReserveCheck.GetCheck()==BST_UNCHECKED ) || \
			 ( p->State == MEM_FREE && this->FreeCheck.GetCheck()==BST_UNCHECKED ) )
			 continue ;

		TempStr.Format ( L"%d", nIndex+1 ) ;
		this->VMList.InsertItem ( nIndex, TempStr ) ;

		// [1]起始地址
		TempStr.Format ( L"%08X", p->BaseAddress ) ;
		this->VMList.SetItemText ( nIndex, 1, TempStr ) ;

		// [2]大小
		TempStr.Format ( L"%08X", p->RegionSize ) ;
		this->VMList.SetItemText ( nIndex, 2, TempStr ) ;

		// [3]状态
		switch ( p->State )
		{
		case MEM_COMMIT:	TempStr = L"提交" ;	break ;
		case MEM_RESERVE:	TempStr = L"保留" ;	break ;
		case MEM_FREE:		TempStr = L"空闲" ;	break ;
		}
		this->VMList.SetItemText ( nIndex, 3, TempStr ) ;

		// [4]类型
		switch ( p->Type )
		{
		case MEM_IMAGE:		TempStr = L"映像";	break ;
		case MEM_PRIVATE:	TempStr = L"私有" ;	break ;
		case MEM_MAPPED:	TempStr = L"映射" ;	break ;
		}
		this->VMList.SetItemText ( nIndex, 4, TempStr ) ;

		// [5]初始保护
		switch ( p->AllocationProtect )
		{
		case PAGE_READONLY:				TempStr = L"-R--";	break;
		case PAGE_READWRITE:			TempStr = L"-RW-";	break;
		case PAGE_WRITECOPY:			TempStr = L"-RWC";	break;
		case PAGE_EXECUTE:				TempStr = L"E---";	break;
		case PAGE_EXECUTE_READ:			TempStr = L"ER--";	break;
		case PAGE_EXECUTE_READWRITE:	TempStr = L"ERW-";	break;
		case PAGE_EXECUTE_WRITECOPY:	TempStr = L"ERWC";	break;
		case PAGE_NOACCESS:				TempStr = L"----";	break;
		default:						TempStr = L"----";	break;
		}
		this->VMList.SetItemText ( nIndex, 5, TempStr ) ;


		// [6]访问保护
		switch ( p->Protect )
		{
		case PAGE_READONLY:				TempStr = L"-R--";	break;
		case PAGE_READWRITE:			TempStr = L"-RW-";	break;
		case PAGE_WRITECOPY:			TempStr = L"-RWC";	break;
		case PAGE_EXECUTE:				TempStr = L"E---";	break;
		case PAGE_EXECUTE_READ:			TempStr = L"ER--";	break;
		case PAGE_EXECUTE_READWRITE:	TempStr = L"ERW-";	break;
		case PAGE_EXECUTE_WRITECOPY:	TempStr = L"ERWC";	break;
		case PAGE_NOACCESS:				TempStr = L"----";	break;
		default:						TempStr = L"----";	break;
		}
		this->VMList.SetItemText ( nIndex, 6, TempStr ) ;

		nIndex ++ ;
	}
}

void CVirtualMemoryDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( nIDEvent == 1 )
	{
		this->VMInfo.VM_GetSysInfo () ;
		this->VMInfo.VM_GetMemStatus () ;

		CString TempStr ;
		TempStr.Format ( L"%08X", this->VMInfo.SysInfo.dwPageSize ) ;
		this->SetDlgItemText ( IDC_PAGE_SIZE, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.SysInfo.lpMinimumApplicationAddress ) ;
		this->SetDlgItemText ( IDC_MIN_ADDR, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.SysInfo.lpMaximumApplicationAddress ) ;
		this->SetDlgItemText ( IDC_MAX_ADDR, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwTotalPhys ) ;
		this->SetDlgItemText ( IDC_TOTAL_PHYSICAL, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwAvailPhys ) ;
		this->SetDlgItemText ( IDC_VALID_PHYSIC, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwTotalPageFile ) ;
		this->SetDlgItemText ( IDC_TOTAL_PAGE, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwAvailPageFile ) ;
		this->SetDlgItemText ( IDC_VALID_PAGE, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwTotalVirtual ) ;
		this->SetDlgItemText ( IDC_TOTAL_VM, TempStr ) ;
		TempStr.Format ( L"%08X", this->VMInfo.MemStatus.dwAvailVirtual ) ;
		this->SetDlgItemText ( IDC_VALID_VM, TempStr ) ;
	}

	CDialog::OnTimer(nIDEvent);
}

void CVirtualMemoryDlg::OnBnClickedSelect()
{
	CSelProcess dlg ;
	if ( dlg.DoModal() == IDOK )
	{
		this->dwSelProcessId = dlg.GetSelProcessId () ;
		this->szProcess.Format ( L"[%04d]%s", dlg.GetSelProcessId(), dlg.GetSelProcessName () ) ;
		this->UpdateData ( FALSE ) ;
	}
}

void CVirtualMemoryDlg::OnBnClickedCheck1()
{
	this->OnBnClickedFlush () ;
}

void CVirtualMemoryDlg::OnBnClickedCheck2()
{
	this->OnBnClickedFlush () ;
}

void CVirtualMemoryDlg::OnBnClickedCheck3()
{
	this->OnBnClickedFlush () ;
}
