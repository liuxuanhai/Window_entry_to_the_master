// FontDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FontDlg.h"
#include "FontDlgDlg.h"

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


// CFontDlgDlg 对话框




CFontDlgDlg::CFontDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFontDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFontDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SET_FONT, &CFontDlgDlg::OnBnClickedSetFont)
END_MESSAGE_MAP()


// CFontDlgDlg 消息处理程序

BOOL CFontDlgDlg::OnInitDialog()
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
	memset ( &this->LogFont, 0, sizeof(LOGFONT)) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFontDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFontDlgDlg::OnPaint()
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

		CFont font ;			// 定义字体
		CClientDC dc ( this ) ;	// 定义当前窗口DC
		// 根据所设置的属性创建字体
		font.CreateFontIndirect ( &LogFont ) ;
		// 把新字体选入DC
		CFont* pFont = dc.SelectObject ( &font ) ;
		// 设置自绘背景模式为透明
		dc.SetBkMode ( TRANSPARENT ) ;
		// 设置字体颜色
		dc.SetTextColor ( this->color ) ;
		// 输出演示字体
		dc.TextOut ( 20, 25, L"测试字体" ) ;
		// 把老字体选入DC，还原
		dc.SelectObject ( pFont ) ;
		// 删除字体
		font.DeleteObject () ;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CFontDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFontDlgDlg::OnBnClickedSetFont()
{
	// 定义“字体”对话框
	CFontDialog dlg ;
	if ( dlg.DoModal() == IDOK )
	{
		// 取得当前设置的字体属性
		dlg.GetCurrentFont ( &LogFont ) ;
		// 取得当前设置的字体颜色
		this->color = dlg.GetColor () ;
		// 通知窗口重绘，以便执行在OnPaint函数中自定义的重绘代码
		this->Invalidate () ;
	}
}
