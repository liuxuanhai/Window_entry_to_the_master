// BlowfishCaluDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BlowfishCalu.h"
#include "BlowfishCaluDlg.h"

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


// CBlowfishCaluDlg 对话框




CBlowfishCaluDlg::CBlowfishCaluDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlowfishCaluDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset ( this->szKey, 0, MAX_KEY_SIZE ) ;
	memset ( this->szMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->ECB_szEnMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->ECB_szDeMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->CBC_szEnMsg, 0, DEF_BUF_SIZE ) ;
	memset ( this->CBC_szEnMsg, 0, DEF_BUF_SIZE ) ;
}

void CBlowfishCaluDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBlowfishCaluDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_KEY, &CBlowfishCaluDlg::OnEnChangeKey)
	ON_BN_CLICKED(IDC_CODING, &CBlowfishCaluDlg::OnBnClickedCoding)
END_MESSAGE_MAP()


// CBlowfishCaluDlg 消息处理程序

BOOL CBlowfishCaluDlg::OnInitDialog()
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
	::SetDlgItemTextA ( this->m_hWnd, IDC_KEY, "Blowfish算法加密解密演示软件" ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBlowfishCaluDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlowfishCaluDlg::OnPaint()
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
HCURSOR CBlowfishCaluDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBlowfishCaluDlg::OnEnChangeKey()
{
}

void CBlowfishCaluDlg::OnBnClickedCoding()
{
	int nKeySize = ::GetDlgItemTextA ( this->m_hWnd, IDC_KEY, (char*)this->szKey, MAX_KEY_SIZE ) ;
	if ( nKeySize < MIN_KEY_SIZE )
	{
		this->MessageBox ( L"密钥长度不能小于4！" ) ;
		return ;
	}

	int nMsgSize = ::GetDlgItemTextA ( this->m_hWnd, IDC_MSG, (char*)this->szMsg, DEF_BUF_SIZE ) ;
	if ( nMsgSize == 0 )
	{
		this->MessageBox ( L"请输入消息字符串！" ) ;
		return ;
	}

	this->Blowfish.BF_SetKey ( this->szKey, nKeySize ) ;

	UINT nRealSize = DEF_BUF_SIZE ;
	this->Blowfish.BF_Encrypt ( this->szMsg, nMsgSize, this->ECB_szEnMsg, &nRealSize ) ;
	this->Blowfish.BF_Decrypt ( this->ECB_szEnMsg, nRealSize, this->ECB_szDeMsg, DEF_BUF_SIZE ) ;

	nRealSize = DEF_BUF_SIZE ;
	this->Blowfish.BF_SetMode ( 1 )  ;
	this->Blowfish.BF_Encrypt ( this->szMsg, nMsgSize, this->CBC_szEnMsg, &nRealSize ) ;
	this->Blowfish.BF_Decrypt ( this->CBC_szEnMsg, nRealSize, this->CBC_szDeMsg, DEF_BUF_SIZE ) ;

	::SetDlgItemTextA ( this->m_hWnd, IDC_ECB_EN_MSG, (char*)this->ECB_szEnMsg ) ;
	::SetDlgItemTextA ( this->m_hWnd, IDC_ECB_DE_MSG, (char*)this->ECB_szDeMsg ) ;
	::SetDlgItemTextA ( this->m_hWnd, IDC_CBC_EN_MSG, (char*)this->CBC_szEnMsg ) ;
	::SetDlgItemTextA ( this->m_hWnd, IDC_CBC_DE_MSG, (char*)this->CBC_szDeMsg ) ;
}
