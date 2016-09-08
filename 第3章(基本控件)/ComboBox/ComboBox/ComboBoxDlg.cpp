// ComboBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComboBox.h"
#include "ComboBoxDlg.h"

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


// CComboBoxDlg 对话框




CComboBoxDlg::CComboBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComboBoxDlg::IDD, pParent)
	, szFruitName(_T(""))
	, szCurName(_T(""))
	, szCurName2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CComboBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, MyComboBox);
	DDX_Text(pDX, IDC_EDIT2, szFruitName);
	DDX_Text(pDX, IDC_EDIT1, szCurName);
	DDX_Control(pDX, IDC_COMBO2, MyComboBox2);
	DDX_Text(pDX, IDC_EDIT3, szCurName2);
}

BEGIN_MESSAGE_MAP(CComboBoxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT2, &CComboBoxDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_ADD, &CComboBoxDlg::OnBnClickedAdd)
	ON_CBN_SELCHANGE(IDC_COMBO, &CComboBoxDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CComboBoxDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CComboBoxDlg 消息处理程序

BOOL CComboBoxDlg::OnInitDialog()
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
	this->MyComboBox.AddString ( L"苹果" ) ;
	this->MyComboBox.AddString ( L"香蕉" ) ;
	this->MyComboBox.AddString ( L"西瓜" ) ;
	this->MyComboBox.SetCurSel ( 0 ) ;
	this->OnCbnSelchangeCombo () ;

	this->MyComboBox2.AddString ( L"苹果" ) ;
	this->MyComboBox2.AddString ( L"香蕉" ) ;
	this->MyComboBox2.AddString ( L"西瓜" ) ;
	this->MyComboBox2.SetCurSel ( 0 ) ;
	this->OnCbnSelchangeCombo2 () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CComboBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CComboBoxDlg::OnPaint()
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
HCURSOR CComboBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComboBoxDlg::OnEnChangeEdit2()
{
	this->UpdateData() ;
}

void CComboBoxDlg::OnBnClickedAdd()
{
	if ( this->szFruitName.IsEmpty() == TRUE )
		this->MessageBox ( L"请输入水果名称", L"Error" ) ;
	else
	{
		this->MyComboBox.AddString ( this->szFruitName ) ;
		this->MyComboBox2.AddString ( this->szFruitName ) ;
	}
}

void CComboBoxDlg::OnCbnSelchangeCombo()
{
	int nCurSel = this->MyComboBox.GetCurSel () ;
	if ( nCurSel != LB_ERR )
	{
		this->MyComboBox.GetLBText(nCurSel, this->szCurName) ;
		this->UpdateData ( FALSE ) ;
	}
}

void CComboBoxDlg::OnCbnSelchangeCombo2()
{
	int nCurSel = this->MyComboBox2.GetCurSel () ;
	if ( nCurSel != LB_ERR )
	{
		this->MyComboBox2.GetLBText(nCurSel, this->szCurName2) ;
		this->UpdateData ( FALSE ) ;
	}
}
