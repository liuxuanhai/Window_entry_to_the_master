// DAOSampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DAOSample.h"
#include "DAOSampleDlg.h"

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


// CDAOSampleDlg 对话框




CDAOSampleDlg::CDAOSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDAOSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDAOSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BOOK_LIST, BookList);
}

BEGIN_MESSAGE_MAP(CDAOSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDAOSampleDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CDAOSampleDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DEL, &CDAOSampleDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_FLUSH, &CDAOSampleDlg::OnBnClickedFlush)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDAOSampleDlg 消息处理程序

BOOL CDAOSampleDlg::OnInitDialog()
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
	
	// 连接数据库
	this->db.Open ( L"book.mdb", FALSE, FALSE, NULL ) ;
	this->OnBnClickedFlush () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDAOSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDAOSampleDlg::OnPaint()
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
HCURSOR CDAOSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDAOSampleDlg::OnBnClickedAdd()
{
	// 检测数据库是否已连接
	if ( this->db.IsOpen () == FALSE )
	{
		this->MessageBox ( L"未连接数据库！" ) ;
		return ;
	}

	// 定义记录集对象，打开记录集
	CDaoRecordset Record ( &this->db ) ;
	Record.Open ( dbOpenDynaset, L"SELECT * FROM BOOK" ) ;

	// 定义对话框对象，并设置为“添加”模式
	CMyDlg AddBookDlg ;
	AddBookDlg.SetMode ( TRUE ) ;

	if ( AddBookDlg.DoModal() == IDOK )
	{
		// 使用CDaoRecordset::AddNew函数表明要开始添加新记录
		Record.AddNew () ;
		// 设置新记录
		Record.SetFieldValue ( 1, AddBookDlg.GetBookName().GetBuffer() ) ;
		Record.SetFieldValue ( 2, AddBookDlg.GetAuthor().GetBuffer() ) ;
		Record.SetFieldValue ( 3, AddBookDlg.GetPublish ().GetBuffer() ) ;
		CString szTempStr ;
		szTempStr.Format ( L"%d", AddBookDlg.GetPrice() ) ;
		Record.SetFieldValue ( 4, szTempStr.GetBuffer() ) ;
		// 只有在执行CDaoRecordset::Update函数后才会写入到数据库
		Record.Update () ;
		// 关闭记录集
		Record.Close () ;
		// 刷新界面列表控件数据显示
		this->OnBnClickedFlush () ;
	}
}

void CDAOSampleDlg::OnBnClickedModify()
{
	// 检测数据库是否已连接
	if ( this->db.IsOpen () == FALSE )
	{
		this->MessageBox ( L"未连接数据库！" ) ;
		return ;
	}

	// 检测是否选择书目
	int nCurSel = this->BookList.GetNextItem ( -1,LVNI_SELECTED) ;
	if ( nCurSel == -1 )
	{
		this->MessageBox ( L"没有选择书目！" ) ;
		return ;
	}

	// 定义记录集对象，并移动指针到指定位置
	CDaoRecordset Record ( &this->db ) ;
	Record.Open ( dbOpenDynaset, L"SELECT * FROM BOOK" ) ;
	Record.Move ( nCurSel ) ;

	// 定义对话框对象，并设置为“修改”模式
	CMyDlg ModBookDlg ;
	ModBookDlg.SetMode ( FALSE ) ;

	// 把当前记录信息设置到对话框
	COleVariant OleVariant ;
	Record.GetFieldValue ( 1, OleVariant ) ;
	ModBookDlg.SetBookName ( OleVariant.bstrVal ) ;
	Record.GetFieldValue ( 2, OleVariant ) ;
	ModBookDlg.SetAuthor ( OleVariant.bstrVal ) ;
	Record.GetFieldValue ( 3, OleVariant ) ;
	ModBookDlg.SetPublish ( OleVariant.bstrVal ) ;
	Record.GetFieldValue ( 4, OleVariant ) ;
	ModBookDlg.SetPrice ( OleVariant.uintVal ) ;

	if ( ModBookDlg.DoModal () == IDOK )
	{
		// 使用CDaoRecordset::Edit函数表明要开始修改当前记录
		Record.Edit () ;
		// 修改当前记录成员
		Record.SetFieldValue ( 1, ModBookDlg.GetBookName().GetBuffer() ) ;
		Record.SetFieldValue ( 2, ModBookDlg.GetAuthor().GetBuffer() ) ;
		Record.SetFieldValue ( 3, ModBookDlg.GetPublish ().GetBuffer() ) ;
		CString szTempStr ;
		szTempStr.Format ( L"%d", ModBookDlg.GetPrice() ) ;
		Record.SetFieldValue ( 4, szTempStr.GetBuffer() ) ;
		// 只有在执行CDaoRecordset::Update函数后才会修改数据库记录
		Record.Update () ;
		// 关于记录集
		Record.Close () ;
		// 刷新界面列表控件数据显示
		this->OnBnClickedFlush () ;
	}
}

void CDAOSampleDlg::OnBnClickedDel()
{
	// 检测数据库是否已连接
	if ( this->db.IsOpen () == FALSE )
	{
		this->MessageBox ( L"未连接数据库！" ) ;
		return ;
	}

	// 检测是否已选择书目
	int nCurSel = this->BookList.GetNextItem ( -1,LVNI_SELECTED) ;
	if ( nCurSel == -1 )
	{
		this->MessageBox ( L"没有选择书目！" ) ;
		return ;
	}

	// 定义记录集对象，并移动指针到指定位置
	CDaoRecordset Record ( &this->db ) ;
	Record.Open ( dbOpenDynaset, L"SELECT * FROM BOOK" ) ;
	Record.Move ( nCurSel ) ;
	// 删除当前记录
	Record.Delete () ;
	// 关闭记录集
	Record.Close () ;
	// 刷新界面列表控件数据显示
	this->OnBnClickedFlush () ;
}

void CDAOSampleDlg::OnBnClickedFlush()
{
	// 检测数据库是否已连接
	if ( this->db.IsOpen () == FALSE )
	{
		this->MessageBox ( L"未连接数据库！" ) ;
		return ;
	}

	// 删除列表控件所有记录
	this->BookList.DeleteAllItems () ;

	// 定义记录集对象，并打开记录集
	CDaoRecordset	Record ( &this->db ) ;
	Record.Open ( dbOpenDynaset, L"SELECT * FROM BOOK" ) ;

	UINT		nIndex = 0 ;
	CString		TempStr ;
	COleVariant	OleVariant ;

	// 移动到第一条记录
	Record.MoveFirst () ;
	while( !Record.IsEOF() )
	{
		// 在列表控件添加记录
		TempStr.Format ( L"%d", nIndex+1 ) ;
		this->BookList.InsertItem ( nIndex, TempStr ) ;

		for ( int i = 1; i <= 3; i++ )
		{
			Record.GetFieldValue ( i, OleVariant ) ;
			this->BookList.SetItemText ( nIndex, i, OleVariant.bstrVal ) ;
		}

		Record.GetFieldValue ( 4, OleVariant ) ;
		TempStr.Format ( L"%d", OleVariant.uintVal ) ;
		this->BookList.SetItemText ( nIndex, 4, TempStr ) ;

		// 移到下一条记录
		Record.MoveNext () ;
		nIndex++ ;
	}

	// 关闭记录集
	Record.Close () ;
}

void CDAOSampleDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	this->db.Close () ;
}
