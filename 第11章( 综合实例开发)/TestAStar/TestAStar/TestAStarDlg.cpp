// TestAStarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestAStar.h"
#include "TestAStarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "AStar.h"


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


// CTestAStarDlg 对话框




CTestAStarDlg::CTestAStarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAStarDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestAStarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUTPUT, OutputList);
}

BEGIN_MESSAGE_MAP(CTestAStarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_GEN_MAP, &CTestAStarDlg::OnBnClickedGenMap)
	ON_BN_CLICKED(IDC_SEARCH_PATH, &CTestAStarDlg::OnBnClickedSearchPath)
	ON_EN_CHANGE(IDC_SIZE, &CTestAStarDlg::OnEnChangeSize)
	ON_EN_CHANGE(IDC_RATE, &CTestAStarDlg::OnEnChangeRate)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestAStarDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestAStarDlg 消息处理程序

BOOL CTestAStarDlg::OnInitDialog()
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
	this->SetDlgItemInt ( IDC_SIZE, 50, 0 ) ;
	this->SetDlgItemInt ( IDC_RATE, 60, 0 ) ;
	AStar_SetMapParam ( 50, 60 ) ;

	this->OutputList.ResetContent() ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestAStarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestAStarDlg::OnPaint()
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
HCURSOR CTestAStarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestAStarDlg::OnBnClickedGenMap()
{
	// TODO: 在此添加控件通知处理程序代码
	int size = this->GetDlgItemInt ( IDC_SIZE ) ;
	if ( size < 10 || size > 1000 )
	{
		this->OutputList.AddString ( L"参数错误 10 <= size <= 1000" ) ;
		return ;
	}

	int rate = this->GetDlgItemInt ( IDC_RATE ) ;
	if ( rate < 0 || rate > 100 )
	{
		this->OutputList.AddString ( L"参数错误 0 <= rate <= 100" ) ;
		return ;
	}

	AStar_SetMapParam ( size, rate ) ;
	if ( AStar_GenerateMap () )
		this->OutputList.AddString ( L"新建地图成功!" ) ;
}

void CTestAStarDlg::OnBnClickedSearchPath()
{
	this->OutputList.AddString ( L"正在加载地图文件……" ) ;
	int num = AStar_InitialMap () ;
	if ( num == 0 )
	{
		this->OutputList.AddString ( L"初始化失败！(确认地图map.in存在)" ) ;
		return ;
	}
	
	CString TempStr ;
	TempStr.Format ( L"地图加载成功! ( Size = %d )", num ) ;
	this->OutputList.AddString ( TempStr ) ;

	this->OutputList.AddString ( L"正在寻找路径……" ) ;
	int a = clock () ;
	bool ret = AStar_GetShortestPath () ;
	int b = clock () ;

	if ( ret == true )
	{
		TempStr.Format ( L"寻路成功！time=%dms", b-a ) ;
		this->OutputList.AddString ( TempStr ) ;
		this->OutputList.AddString ( L"正在输出到文件……" ) ;
		AStar_Output () ;
		this->OutputList.AddString ( L"输出成功!" ) ;
	}
	else
		this->OutputList.AddString ( L"该地图没有路径!" ) ;
}

void CTestAStarDlg::OnEnChangeSize()
{
	this->UpdateData () ;
}

void CTestAStarDlg::OnEnChangeRate()
{
	this->UpdateData () ;
}

void CTestAStarDlg::OnBnClickedButton1()
{
	this->OutputList.ResetContent () ;
}
