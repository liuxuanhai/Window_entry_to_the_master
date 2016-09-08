// SimMouseMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SimMouseMsg.h"
#include "SimMouseMsgDlg.h"

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


// CSimMouseMsgDlg 对话框




CSimMouseMsgDlg::CSimMouseMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimMouseMsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimMouseMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimMouseMsgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LEFT_DOWN, &CSimMouseMsgDlg::OnBnClickedLeftDown)
	ON_BN_CLICKED(IDC_RIGHT_DOWN, &CSimMouseMsgDlg::OnBnClickedRightDown)
	ON_BN_CLICKED(IDC_MOUSE_MOVE, &CSimMouseMsgDlg::OnBnClickedMouseMove)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSimMouseMsgDlg 消息处理程序

BOOL CSimMouseMsgDlg::OnInitDialog()
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

	// 初始时没有模拟消息，设置为0
	this->nType = 0 ;
	// 设置绘图区域
	this->PaintRect.left = 15 ;
	this->PaintRect.top = 80 ;
	this->PaintRect.right = 310 ;
	this->PaintRect.bottom = 130 ;
	// 创建画笔
	this->FramePen.CreatePen ( PS_SOLID, 3, RGB(0,0,0) ) ;
	// 创建画刷
	this->RedBrush.CreateSolidBrush ( RGB(255,0,0) ) ;
	this->GreenBrush.CreateSolidBrush ( RGB(0,255,0) ) ;
	this->BlueBrush.CreateSolidBrush ( RGB(0,0,255) ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSimMouseMsgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimMouseMsgDlg::OnPaint()
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
		CClientDC cdc(this) ;
		CPen* pOldPen = cdc.SelectObject ( &this->FramePen ) ;
		CBrush* pOldBrush = NULL ;
		switch ( this->nType )
		{
		case 1:	pOldBrush = cdc.SelectObject ( &this->RedBrush ) ;		break ;
		case 2:	pOldBrush = cdc.SelectObject ( &this->GreenBrush ) ;	break ;
		case 3:	pOldBrush = cdc.SelectObject ( &this->BlueBrush ) ;		break ;
		}
		cdc.Rectangle ( &this->PaintRect ) ;
		if ( pOldBrush )
			cdc.SelectObject ( pOldBrush ) ;
		cdc.SelectObject ( pOldPen ) ;

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSimMouseMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 模拟鼠标左键单击消息
void CSimMouseMsgDlg::OnBnClickedLeftDown()
{
	// 任取绘图区域内的一点
	CPoint point(50,100) ;
	// 把客户区域坐标转化为屏幕坐标
	this->ClientToScreen ( &point ) ;

	// 保存原来的鼠标位置
	POINT OldPoint ;
	::GetCursorPos ( &OldPoint ) ;
	// 设置鼠标到新位置
	::SetCursorPos ( point.x, point.y ) ;
	// 模拟鼠标单击消息
	mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 ) ;
	// 设置鼠标到原来位置
	::SetCursorPos ( OldPoint.x, OldPoint.y ) ;
}

// 模拟鼠标右键单击消息
void CSimMouseMsgDlg::OnBnClickedRightDown()
{
	// 任取绘图区域内的一点
	CPoint point(50,100) ;
	// 把客户区域坐标转化为屏幕坐标
	this->ClientToScreen ( &point ) ;

	// 保存原来的鼠标位置
	POINT OldPoint ;
	::GetCursorPos ( &OldPoint ) ;
	// 设置鼠标到新位置
	::SetCursorPos ( point.x, point.y ) ;
	// 模拟鼠标单击消息
	mouse_event ( MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0 ) ;
	mouse_event ( MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 ) ;
	// 设置鼠标到原来位置
	::SetCursorPos ( OldPoint.x, OldPoint.y ) ;
}

// 模拟鼠标移动消息
void CSimMouseMsgDlg::OnBnClickedMouseMove()
{
	// 任取绘图区域内的一点
	CPoint point ( 50, 100 ) ;
	// 模拟鼠标移动消息，这里使用的是相对坐标
	this->PostMessage ( WM_MOUSEMOVE, 0, point.x|(point.y<<16) ) ;
}

void CSimMouseMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 只响应绘图区域内的鼠标消息
	if ( this->PaintRect.PtInRect(point) == TRUE )
	{
		this->nType = 1 ;
		// 通知应用程序重绘
		this->InvalidateRect ( &this->PaintRect, FALSE ) ;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CSimMouseMsgDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 只响应绘图区域内的鼠标消息
	if ( this->PaintRect.PtInRect(point) == TRUE )
	{
		this->nType = 2 ;
		// 通知应用程序重绘
		this->InvalidateRect ( &this->PaintRect, FALSE ) ;
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CSimMouseMsgDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 只响应绘图区域内的鼠标消息
	if ( this->PaintRect.PtInRect(point) == TRUE )
	{
		this->nType = 3 ;
		// 通知应用程序重绘
		this->InvalidateRect ( &this->PaintRect, FALSE ) ;
	}
	CDialog::OnMouseMove(nFlags, point);
}
