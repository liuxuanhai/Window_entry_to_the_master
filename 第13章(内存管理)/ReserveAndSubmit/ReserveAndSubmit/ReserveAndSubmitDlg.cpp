// ReserveAndSubmitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReserveAndSubmit.h"
#include "ReserveAndSubmitDlg.h"

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


// CReserveAndSubmitDlg 对话框




CReserveAndSubmitDlg::CReserveAndSubmitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReserveAndSubmitDlg::IDD, pParent)
	, szBaseAddr(_T(""))
	, szCurAddr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReserveAndSubmitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BASE_ADDR, szBaseAddr);
	DDX_Text(pDX, IDC_CUR_ADDR, szCurAddr);
}

BEGIN_MESSAGE_MAP(CReserveAndSubmitDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ALLOC, &CReserveAndSubmitDlg::OnBnClickedAlloc)
	ON_BN_CLICKED(IDC_RELEASE, &CReserveAndSubmitDlg::OnBnClickedRelease)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CReserveAndSubmitDlg 消息处理程序

BOOL CReserveAndSubmitDlg::OnInitDialog()
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
	this->PaintRegion.InitialRect ( CRect(20,30,380,70) ) ;
	this->WhiteBrush.CreateSolidBrush ( RGB(255,255,255) ) ;
	this->GreyBrush.CreateSolidBrush ( RGB(128,128,128) ) ;
	this->BlackBrush.CreateSolidBrush ( RGB(0,0,0) ) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CReserveAndSubmitDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReserveAndSubmitDlg::OnPaint()
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
		CRect rect ;
		this->GetWindowRect( &rect ) ;
		this->ScreenToClient ( &rect ) ;

		CClientDC dc(this) ;
		CDC	mdc ;
		mdc.CreateCompatibleDC ( &dc ) ;
		CBitmap bmp ;
		bmp.CreateCompatibleBitmap ( &dc, rect.Width(), rect.Height() ) ;
		CBitmap* pOldBmp = mdc.SelectObject ( &bmp ) ;
		CBrush* pOldBrush ;

		pOldBrush = mdc.SelectObject ( &this->WhiteBrush ) ;
		mdc.Rectangle ( this->PaintRegion.GetStautsRect() ) ;
		
		if ( this->PaintRegion.IsValid() )
		{
			for ( int i = 0; i < MAX_PAGE_NUM; i++ )
			{
				if ( this->PaintRegion.GetStatusByIndex(i) )
					mdc.SelectObject ( &this->BlackBrush ) ;
				else
					mdc.SelectObject ( &this->GreyBrush ) ;
				mdc.Rectangle ( this->PaintRegion.GetRectByIndex(i) ) ;
			}
		}
		
		CRect StatusRect = this->PaintRegion.GetStautsRect() ;
		dc.BitBlt ( StatusRect.left, StatusRect.top, StatusRect.Width(), \
			StatusRect.Height(), &mdc, StatusRect.left, StatusRect.top, SRCCOPY ) ;

		dc.SelectObject ( pOldBrush ) ;
		dc.SelectObject ( pOldBmp ) ;
		bmp.DeleteObject () ;
		mdc.DeleteDC () ;

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CReserveAndSubmitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CReserveAndSubmitDlg::OnBnClickedAlloc()
{
	if ( this->PaintRegion.IsValid() )
		return ;
	this->PaintRegion.SetValid ( TRUE ) ;
	this->Invalidate ( FALSE ) ;
	this->szBaseAddr.Format ( L"0x%08X", this->PaintRegion.GetBaseAddr () ) ;
	this->UpdateData ( FALSE ) ;
}

void CReserveAndSubmitDlg::OnBnClickedRelease()
{
	if ( this->PaintRegion.IsValid() == FALSE )
		return ;
	this->PaintRegion.SetValid ( FALSE ) ;
	this->Invalidate ( FALSE ) ;
	this->szBaseAddr = L"" ;
	this->UpdateData ( FALSE ) ;
}

void CReserveAndSubmitDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( this->PaintRegion.IsValid () )
	{
		this->PaintRegion.ChangeStatusByPt ( point ) ;
		this->Invalidate ( FALSE ) ;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CReserveAndSubmitDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( this->PaintRegion.IsValid () )
	{
		DWORD dwPageAddr = this->PaintRegion.GetAddrByStatus (point) ;
		if ( dwPageAddr == 0 )
			this->szCurAddr = L"" ;
		else
			this->szCurAddr.Format ( L"0x%08X", dwPageAddr ) ;
		this->UpdateData ( FALSE ) ;
	}

	CDialog::OnMouseMove(nFlags, point);
}
