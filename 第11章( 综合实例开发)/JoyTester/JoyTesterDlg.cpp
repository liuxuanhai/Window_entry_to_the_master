// JoyTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JoyTester.h"
#include "JoyTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	XX	25
#define	YY	225
#define	LX	36
#define	LY	20
#define	DY	5

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyTesterDlg dialog

CJoyTesterDlg::CJoyTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJoyTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJoyTesterDlg)
	pov0 = 0;
	pov1 = 0;
	pov2 = 0;
	rx = 0;
	rz = 0;
	pov3 = 0;
	ry = 0;
	s1 = 0;
	s0 = 0;
	x = 0;
	y = 0;
	z = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJoyTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJoyTesterDlg)
	DDX_Text(pDX, IDC_POV0, pov0);
	DDX_Text(pDX, IDC_POV1, pov1);
	DDX_Text(pDX, IDC_POV2, pov2);
	DDX_Text(pDX, IDC_RX, rx);
	DDX_Text(pDX, IDC_RZ, rz);
	DDX_Text(pDX, IDC_POV3, pov3);
	DDX_Text(pDX, IDC_RY, ry);
	DDX_Text(pDX, IDC_S1, s1);
	DDX_Text(pDX, IDC_SO, s0);
	DDX_Text(pDX, IDC_X, x);
	DDX_Text(pDX, IDC_Y, y);
	DDX_Text(pDX, IDC_Z, z);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJoyTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CJoyTesterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyTesterDlg message handlers

BOOL CJoyTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	memset ( this->ButtonItem, 0, sizeof(this->ButtonItem) ) ;

	this->ButtonItem[0].rect.left	= XX ;
	this->ButtonItem[0].rect.top	= YY ;

	this->ButtonItem[8].rect.left	= XX ;
	this->ButtonItem[8].rect.top	= YY + LY + DY ;

	for ( int i = 1; i <= 7; i ++ )
	{
		this->ButtonItem[i].rect.left	= this->ButtonItem[i-1].rect.left + LX ;
		this->ButtonItem[i].rect.top	= this->ButtonItem[i-1].rect.top ;
	}

	for ( i = 9; i <= 15; i ++ )
	{
		this->ButtonItem[i].rect.left	= this->ButtonItem[i-1].rect.left + LX ;
		this->ButtonItem[i].rect.top	= this->ButtonItem[i-1].rect.top ;
	}

	for ( i = 0; i < MAX_BUTTON_NUM; i++ )
	{
		this->ButtonItem[i].rect.right	= this->ButtonItem[i].rect.left + LX ;
		this->ButtonItem[i].rect.bottom	= this->ButtonItem[i].rect.top + LY ;
	}


	bool bRet ;
	bRet = this->js.JS_Initial() ;
	if ( !bRet )
	{
		this->MessageBox ( "初始化失败！" ) ;
		::ExitProcess ( 0 ) ;
	}


	if ( this->js.JS_GetDeviceNum() == 0 )
		this->MessageBox ( "没有Joystick!" ) ;
	else
	{
		if ( this->js.JS_StartByIndex ( 0, this->m_hWnd ) )
			this->SetTimer ( 1, 50, NULL ) ;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJoyTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJoyTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		
		CPaintDC dc ( this ) ;
		CBrush OldBrush(RGB(192,192,192)), NewBrush(RGB(255,0,0)) ;

		CString TempStr ;
		for ( int i = 0; i < MAX_BUTTON_NUM; i++ )
		{
			if ( this->ButtonItem[i].isPressed )
			{
				dc.FillRect ( &(this->ButtonItem[i].rect), &NewBrush ) ;
			}
			else
			{
				dc.FillRect ( &(this->ButtonItem[i].rect), &OldBrush ) ;
			}
			
			TempStr.Format ( "%d", i ) ;
			dc.SetBkMode ( TRANSPARENT ) ;
			dc.TextOut ( this->ButtonItem[i].rect.left + 13, \
				this->ButtonItem[i].rect.top + 3, TempStr ) ;
		}

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CJoyTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CJoyTesterDlg::OnTimer(UINT nIDEvent) 
{
	DIJOYSTATE2	JoyState = {0} ;
	this->js.JS_GetStateByIndex ( 0, &JoyState ) ;

	//this->pov0 = this->pov1 = this->pov2 = this->pov3 = -1 ;
	//this->x	= this->y = this->z = this->rx = this->ry = this->rz = this->s0 = this->s1 = 0 ;

	this->x		= JoyState.lX ;
	this->y		= JoyState.lY ;
	this->z		= JoyState.lZ ;
	this->rx	= JoyState.lRx ;
	this->ry	= JoyState.lRy ;
	this->rz	= JoyState.lRz ;
	this->s0	= JoyState.rglSlider[0] ;
	this->s1	= JoyState.rglSlider[1] ;
	this->pov0	= JoyState.rgdwPOV[0] ;
	this->pov1	= JoyState.rgdwPOV[1] ;
	this->pov2	= JoyState.rgdwPOV[2] ;
	this->pov3	= JoyState.rgdwPOV[3] ;
	this->UpdateData ( false ) ;

	for ( int i = 0; i < MAX_BUTTON_NUM; i++ )
		this->ButtonItem[i].isPressed = ( JoyState.rgbButtons[i] & 0x80 ) ? true : false ;

	RECT rect = { XX, YY, XX+8*LX, YY+2*LY+DY } ;
	this->InvalidateRect ( &rect, false ) ;
	CDialog::OnTimer(nIDEvent);
}

BOOL CJoyTesterDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	this->KillTimer ( 1 ) ;
	this->js.JS_StopByIndex ( 0 ) ;
	return CDialog::DestroyWindow();
}
