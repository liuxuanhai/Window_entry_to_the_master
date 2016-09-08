// IDEACaluDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IDEACalu.h"
#include "IDEACaluDlg.h"

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


// CIDEACaluDlg 对话框




CIDEACaluDlg::CIDEACaluDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIDEACaluDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset ( this->bMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->bEnMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->bDeMsg, 0, DEF_BUF_SIZE ) ;
}

void CIDEACaluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIDEACaluDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EN_DE, &CIDEACaluDlg::OnBnClickedEnDe)
	ON_BN_CLICKED(IDC_SET_KEY, &CIDEACaluDlg::OnBnClickedSetKey)
END_MESSAGE_MAP()


// CIDEACaluDlg 消息处理程序

BOOL CIDEACaluDlg::OnInitDialog()
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
	::SetDlgItemTextA ( this->m_hWnd, IDC_KEY, "TEST_IDEA_SAMPLE" ) ;
	this->OnBnClickedSetKey (); 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIDEACaluDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIDEACaluDlg::OnPaint()
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
HCURSOR CIDEACaluDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CIDEACaluDlg::OnBnClickedEnDe()
{
	int nTextLen = ::GetDlgItemTextA ( this->m_hWnd, IDC_TEXT, (char*)this->bMsg, DEF_BUF_SIZE ) ;
	if ( nTextLen == 0 )
	{
		this->MessageBox ( L"请输入字符串!" ) ;
		return ;
	}

	this->nEnDataSize = DEF_BUF_SIZE ;
	memset ( (char*)this->bEnMsg, 0, DEF_BUF_SIZE ) ;
	this->idea.EncryptData ( this->bMsg, nTextLen, this->bEnMsg, &this->nEnDataSize ) ;
	::SetDlgItemTextA ( this->m_hWnd, IDC_EN_TEXT, (char*)this->bEnMsg ) ;

	UINT nBufSize = DEF_BUF_SIZE ;
	memset ( (char*)this->bDeMsg, 0, DEF_BUF_SIZE ) ;
	this->idea.DecryptData ( this->bEnMsg, this->nEnDataSize, this->bDeMsg, &nBufSize ) ;
	::SetDlgItemTextA ( this->m_hWnd, IDC_DE_TEXT, (char*)this->bDeMsg ) ;
}

void CIDEACaluDlg::OnBnClickedSetKey()
{
	BYTE bKey[DEF_BUF_SIZE] ;
	int nSize = ::GetDlgItemTextA ( this->m_hWnd, IDC_KEY, (char*)bKey, DEF_BUF_SIZE ) ;
	this->idea.SetKey ( bKey, nSize ) ;
}
