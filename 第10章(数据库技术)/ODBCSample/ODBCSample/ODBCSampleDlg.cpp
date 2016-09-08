// ODBCSampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ODBCSample.h"
#include "ODBCSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MyDlg.h"

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


// CODBCSampleDlg 对话框




CODBCSampleDlg::CODBCSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CODBCSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CODBCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, BookList);
}

BEGIN_MESSAGE_MAP(CODBCSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CODBCSampleDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CODBCSampleDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DEL, &CODBCSampleDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_FLUSH, &CODBCSampleDlg::OnBnClickedFlush)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CODBCSampleDlg 消息处理程序

BOOL CODBCSampleDlg::OnInitDialog()
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
	this->BookList.InsertColumn ( 0, L" ID",LVCFMT_CENTER, 30 ) ;
	this->BookList.InsertColumn ( 1, L" 书   名",LVCFMT_CENTER, 160 ) ;
	this->BookList.InsertColumn ( 2, L"作者",	LVCFMT_CENTER, 100 ) ;
	this->BookList.InsertColumn ( 3, L"出版社",	LVCFMT_CENTER, 100 ) ;
	this->BookList.InsertColumn ( 4, L"价格",	LVCFMT_CENTER, 60 ) ;
	this->BookList.SetExtendedStyle ( this->BookList.GetExtendedStyle()|LVS_EX_FULLROWSELECT) ;

	Record.Open ( CRecordset::dynaset, L"SELECT * FROM BOOK" ) ;
	this->OnBnClickedFlush () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CODBCSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CODBCSampleDlg::OnPaint()
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
HCURSOR CODBCSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CODBCSampleDlg::OnBnClickedAdd()
{
	// 定义对话框对象，并设置为“添加”模式
	CMyDlg BookInfoDlg ;
	BookInfoDlg.SetMode ( TRUE ) ;

	if ( BookInfoDlg.DoModal () == IDOK )
	{
		// 使用CRecordset::AddNew函数表明要开始创建新记录
		Record.AddNew () ;
		// 设置新记录成员，由CBook类负责与变量与字段之间的关联
		Record.m_BookName = BookInfoDlg.GetBookName () ;
		Record.m_Author = BookInfoDlg.GetAuthor () ;
		Record.m_Publish = BookInfoDlg.GetPublish () ;
		Record.m_Price = BookInfoDlg.GetPrice () ;
		// 只有在执行CRecordset::Update函数后才会把记录写入到数据库
		Record.Update () ;

		// 刷新界面列表控件的数据显示
		this->OnBnClickedFlush() ;
	}
}

void CODBCSampleDlg::OnBnClickedModify()
{
	// 检测是否选择某一书目
	int nCurSel = this->BookList.GetNextItem(-1,LVNI_SELECTED) ;
	if ( nCurSel == -1 )
	{
		this->MessageBox ( L"未选择书目！" ) ;
		return ;
	}

	// 定义对话框对象，并设置为“修改”模式
	CMyDlg BookInfoDlg ;
	BookInfoDlg.SetMode ( FALSE ) ;

	// 设置到绝对位置
	Record.SetAbsolutePosition ( nCurSel + 1 ) ;
	// 把需要修改的当前记录设置到“修改书籍信息对话框”
	BookInfoDlg.SetBookName ( Record.m_BookName ) ;
	BookInfoDlg.SetAuthor ( Record.m_Author ) ;
	BookInfoDlg.SetPublish ( Record.m_Publish ) ;
	BookInfoDlg.SetPrice ( Record.m_Price ) ;

	if ( BookInfoDlg.DoModal () == IDOK )
	{
		// 使用CRecordset::Edit函数表明要开始修改当前记录
		Record.Edit () ;
		// 修改当前记录成员，由CBook类负责与变量与字段之间的关联
		Record.m_BookName = BookInfoDlg.GetBookName () ;
		Record.m_Author = BookInfoDlg.GetAuthor () ;
		Record.m_Publish = BookInfoDlg.GetPublish () ;
		Record.m_Price = BookInfoDlg.GetPrice () ;
		// 只有在执行CRecordset::Update函数后才会修改数据库记录
		Record.Update () ;

		// 刷新界面列表控件的数据显示
		this->OnBnClickedFlush () ;
	}
}

void CODBCSampleDlg::OnBnClickedDel()
{
	// 检测是否选择某一书目
	int nCurSel = this->BookList.GetNextItem(-1,LVNI_SELECTED) ;
	if ( nCurSel == -1 )
	{
		this->MessageBox ( L"未选择书目！" ) ;
		return ;
	}
	
	// 设置到记录集的绝对位置
	Record.SetAbsolutePosition ( nCurSel + 1 ) ;
	// 删除当前记录
	Record.Delete () ;
	// 刷新界面列表控件的数据显示
	this->OnBnClickedFlush() ;
}

void CODBCSampleDlg::OnBnClickedFlush()
{
	// 删除列表控件的所有记录
	this->BookList.DeleteAllItems () ;

	UINT nIndex = 0 ;
	CString TempStr ;

	// 移动指针对第一条记录
	Record.MoveFirst () ;

	// 检测是否已经到达记录集尾部
	while ( !Record.IsEOF() )
	{
		// 向列表控件添加记录
		TempStr.Format ( L"%d", nIndex+1 ) ;
		this->BookList.InsertItem ( nIndex, TempStr ) ;

		// 设置列表控件当前记录内容
		this->BookList.SetItemText ( nIndex, 1, Record.m_BookName ) ;
		this->BookList.SetItemText ( nIndex, 2, Record.m_Author ) ;
		this->BookList.SetItemText ( nIndex, 3, Record.m_Publish ) ;

		TempStr.Format ( L"%d", Record.m_Price ) ;
		this->BookList.SetItemText ( nIndex, 4, TempStr ) ;

		// 移到下一个记录
		Record.MoveNext () ;
		nIndex++ ;
	}
}


void CODBCSampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	this->Record.Close () ;
}
