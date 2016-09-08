// ListCtrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrl.h"
#include "ListCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "AddInfoDlg.h"


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


// CListCtrlDlg 对话框




CListCtrlDlg::CListCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, list);
}

BEGIN_MESSAGE_MAP(CListCtrlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CListCtrlDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEL, &CListCtrlDlg::OnBnClickedDel)
END_MESSAGE_MAP()


// CListCtrlDlg 消息处理程序

BOOL CListCtrlDlg::OnInitDialog()
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

	this->InitListCtrl () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CListCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CListCtrlDlg::OnPaint()
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
HCURSOR CListCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CListCtrlDlg::OnBnClickedAdd()
{
	CAddInfoDlg dlg ;
	if ( dlg.DoModal () != IDCANCEL )
	{
		// 取得列表控件中条目数量
		int nItemNum = this->list.GetItemCount () ;
		// 添加人员信息到列表控件
		this->list.InsertItem ( nItemNum, dlg.NEW_GetName() ) ;
		this->list.SetItemText ( nItemNum, 1, dlg.NEW_GetId() ) ;
		this->list.SetItemText ( nItemNum, 2, dlg.NEW_GetGender() ) ;
		this->list.SetItemText ( nItemNum, 3, dlg.NEW_GetAddr () ) ;
	}
}

void CListCtrlDlg::OnBnClickedDel()
{
	// 取得当前所选择的条目索引
	int nCurSel = this->list.GetNextItem ( -1, LVNI_SELECTED ) ;
	if ( nCurSel != -1 )
		this->list.DeleteItem ( nCurSel ) ;
}

VOID CListCtrlDlg::InitListCtrl(void)
{
	this->list.InsertColumn ( 0, L"  姓名",	LVCFMT_CENTER, 80 ) ;
	this->list.InsertColumn ( 1, L"身份证",	LVCFMT_CENTER, 200 ) ;
	this->list.InsertColumn ( 2, L"性别",	LVCFMT_CENTER, 60 ) ;
	this->list.InsertColumn ( 3, L"籍贯",	LVCFMT_CENTER, 80 ) ;
	this->list.SetExtendedStyle ( this->list.GetExtendedStyle()|LVS_EX_FULLROWSELECT) ;

	this->list.InsertItem ( 0, L"张三丰" ) ;
	this->list.SetItemText ( 0, 1, L"1234356789123456789" ) ;
	this->list.SetItemText ( 0, 2, L"男" ) ;
	this->list.SetItemText ( 0, 3, L"武当" ) ;

	this->list.InsertItem ( 1, L"灭绝师太" ) ;
	this->list.SetItemText ( 1, 1, L"123456789987654321" ) ;
	this->list.SetItemText ( 1, 2, L"女" ) ;
	this->list.SetItemText ( 1, 3, L"峨眉" ) ;

	this->list.InsertItem ( 2, L"阳顶天" ) ;
	this->list.SetItemText ( 2, 1, L"431354345657657686" ) ;
	this->list.SetItemText ( 2, 2, L"男" ) ;
	this->list.SetItemText ( 2, 3, L"明教" ) ;

	this->list.InsertItem ( 3, L"乔峰" ) ;
	this->list.SetItemText ( 3, 1, L"123123132132321321" ) ;
	this->list.SetItemText ( 3, 2, L"男" ) ;
	this->list.SetItemText ( 3, 3, L"丐帮" ) ;

	return VOID();
}
