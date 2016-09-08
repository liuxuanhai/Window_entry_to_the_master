// MouseMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MouseMsg.h"
#include "MouseMsgDlg.h"

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


// CMouseMsgDlg 对话框




CMouseMsgDlg::CMouseMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMouseMsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMouseMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMouseMsgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CMouseMsgDlg 消息处理程序

BOOL CMouseMsgDlg::OnInitDialog()
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
	this->BkBrush.CreateSolidBrush( RGB(255,255,255) ) ;
	this->pen.CreatePen ( PS_SOLID, 5, RGB(0,0,255) ) ;

	this->pdc = new CClientDC (this) ;
	this->pdc->SelectObject ( &this->pen ) ;

	this->GetWindowRect ( &this->WndRect ) ;
	this->OldPt.x = this->OldPt.y = 0 ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMouseMsgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMouseMsgDlg::OnPaint()
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
HCURSOR CMouseMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMouseMsgDlg::OnEraseBkgnd(CDC* pDC)
{
	CBrush* pOldBrush = pDC->SelectObject ( &this->BkBrush ) ;
	pDC->Rectangle ( &this->WndRect ) ;
	pDC->SelectObject ( pOldBrush ) ;
	return TRUE ;
}

// 响应鼠标移动消息
// 参数nFlags：鼠标移动时的状态信息
// 参数point：当前鼠标的位置
void CMouseMsgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 检测鼠标状态，即检测鼠标移动过程左键是否处于按下的状态
	if ( nFlags & MK_LBUTTON )
	{
		// pdc是预先保存的对话框客户区域DC
		// 使用MoveTo和LineTo绘制线条
		this->pdc->MoveTo ( this->OldPt.x, this->OldPt.y ) ;
		this->pdc->LineTo ( point.x, point.y ) ;
	}
	// 保存当前点
	this->OldPt = point ;	
	CDialog::OnMouseMove(nFlags, point);
}

void CMouseMsgDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	this->pdc->DeleteDC () ;
}

// 响应鼠标左键双击消息
void CMouseMsgDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC cdc ( this ) ;
	// BkBrush是预先创建的白色画刷
	CBrush* pOldBrush = cdc.SelectObject ( &this->BkBrush ) ;
	// 使用白色画刷绘制整个客户区域，达到清除的效果
	cdc.Rectangle ( &this->WndRect ) ;
	cdc.SelectObject ( pOldBrush ) ;
	CDialog::OnLButtonDblClk(nFlags, point);
}
