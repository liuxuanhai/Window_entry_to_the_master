// KeyboardMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KeyboardMsg.h"
#include "KeyboardMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PACE_DIST	5

#define HOTKEY_UP		1001
#define HOTKEY_DOWN		1002
#define HOTKEY_LEFT		1003
#define HOTKEY_RIGHT	1004

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


// CKeyboardMsgDlg 对话框




CKeyboardMsgDlg::CKeyboardMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyboardMsgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyboardMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyboardMsgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_KEYUP()
	ON_BN_CLICKED(IDC_LOCAL, &CKeyboardMsgDlg::OnBnClickedLocal)
	ON_BN_CLICKED(IDC_TOTAL, &CKeyboardMsgDlg::OnBnClickedTotal)
END_MESSAGE_MAP()


// CKeyboardMsgDlg 消息处理程序

BOOL CKeyboardMsgDlg::OnInitDialog()
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

	// 初始设置为响应本进程消息

	this->bType = TRUE ;
	((CButton*)this->GetDlgItem(IDC_LOCAL))->SetCheck ( BST_CHECKED ) ;

	this->GetWindowRect ( &this->ValidRect ) ;
	this->ScreenToClient ( &this->ValidRect ) ;
	this->ValidRect.top += 100 ;

	this->FaceBmp.LoadBitmapW ( IDB_FACE ) ;
	BITMAP BmpInfo ;
	this->FaceBmp.GetBitmap ( &BmpInfo ) ;
	this->FaceRect.left = 140 ;
	this->FaceRect.top = 100 ;
	this->FaceRect.right = this->FaceRect.left + BmpInfo.bmWidth ;
	this->FaceRect.bottom = this->FaceRect.top + BmpInfo.bmHeight ;

	::RegisterHotKey ( this->m_hWnd, HOTKEY_UP, 0, VK_UP ) ;
	::RegisterHotKey ( this->m_hWnd, HOTKEY_DOWN, 0, VK_DOWN ) ;
	::RegisterHotKey ( this->m_hWnd, HOTKEY_LEFT, 0, VK_LEFT ) ;
	::RegisterHotKey ( this->m_hWnd, HOTKEY_RIGHT, 0, VK_RIGHT ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKeyboardMsgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKeyboardMsgDlg::OnPaint()
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
		CClientDC	cdc(this) ;
		CDC			mdc, TempDc ;
		CBitmap		bmp ;
		CBrush		BkBrush ;
		CRect		WndRect ;
		this->GetWindowRect ( &WndRect ) ;

		mdc.CreateCompatibleDC ( &cdc ) ;
		TempDc.CreateCompatibleDC ( &cdc ) ;
		bmp.CreateCompatibleBitmap ( &cdc, WndRect.Width(), WndRect.Height() ) ;
		mdc.SelectObject ( &bmp ) ;

		BkBrush.CreateSolidBrush ( RGB(255,255,255) ) ;
		mdc.SelectObject ( &BkBrush ) ;
		mdc.Rectangle ( &this->ValidRect ) ;

		TempDc.SelectObject ( &this->FaceBmp ) ;
		mdc.BitBlt ( this->FaceRect.left, this->FaceRect.top, this->FaceRect.Width(), \
			this->FaceRect.Height(), &TempDc, 0, 0, SRCCOPY ) ;


		cdc.BitBlt ( this->ValidRect.left, this->ValidRect.top,this->ValidRect.Width(),\
			this->ValidRect.Height(), &mdc, this->ValidRect.left, this->ValidRect.top+1, SRCCOPY ) ;

		bmp.DeleteObject () ;
		BkBrush.DeleteObject () ;
		TempDc.DeleteDC () ;
		mdc.DeleteDC () ;

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CKeyboardMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKeyboardMsgDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CKeyboardMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		if ( this->bType == TRUE )
		{
			switch ( pMsg->wParam )
			{
			case 'W':	this->MoveFace ( 0 ) ;	break ;
			case 'S':	this->MoveFace ( 1 ) ;	break ;
			case 'A':	this->MoveFace ( 2 ) ;	break ;
			case 'D':	this->MoveFace ( 3 ) ;	break ;
			}
		}
		break ;
	case WM_HOTKEY:
		if ( this->bType == FALSE )
		{
			switch ( pMsg->wParam )
			{
			case HOTKEY_UP:		this->MoveFace ( 0 ) ;	break ;
			case HOTKEY_DOWN:	this->MoveFace ( 1 ) ;	break ;
			case HOTKEY_LEFT:	this->MoveFace ( 2 ) ;	break ;
			case HOTKEY_RIGHT:	this->MoveFace ( 3 ) ;	break ;
			}
		}
		break ;
	}


	return CDialog::PreTranslateMessage(pMsg);
}

VOID CKeyboardMsgDlg::MoveFace ( UINT nDir )
{
	switch ( nDir )
	{
	case 0:		// 上
		if ( this->FaceRect.top >= this->ValidRect.top + PACE_DIST )
		{
			this->FaceRect.top -= PACE_DIST ;
			this->FaceRect.bottom -= PACE_DIST ;
		}
		break ;
	case 1:		// 下
		if ( this->FaceRect.bottom <= this->ValidRect.bottom - PACE_DIST )
		{
			this->FaceRect.top += PACE_DIST ;
			this->FaceRect.bottom += PACE_DIST ;
		}
		break ;
	case 2:		// 左
		if ( this->FaceRect.left >= this->ValidRect.left + PACE_DIST )
		{
			this->FaceRect.left -= PACE_DIST ;
			this->FaceRect.right -= PACE_DIST ;
		}
		break ;
	case 3:		// 右
		if ( this->FaceRect.right <= this->ValidRect.right - PACE_DIST  )
		{
			this->FaceRect.left += PACE_DIST ;
			this->FaceRect.right += PACE_DIST ;
		}
		break ;
	}
	this->Invalidate ( FALSE ) ;
}
void CKeyboardMsgDlg::OnBnClickedLocal()
{
	this->bType = TRUE ;
}

void CKeyboardMsgDlg::OnBnClickedTotal()
{
	this->bType = FALSE ;
}
