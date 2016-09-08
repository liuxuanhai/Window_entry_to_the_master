// File DivideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "File Divide.h"
#include "File DivideDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CFileDivideDlg dialog

CFileDivideDlg::CFileDivideDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDivideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDivideDlg)
	m_DivideFileName = _T("");
	m_PartNum = 0;
	m_ConnFileName = _T("");
	m_FileSize = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileDivideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDivideDlg)
	DDX_Control(pDX, IDC_COMBO, m_ComList);
	DDX_Text(pDX, IDC_FILE_TO_CONNECT, m_ConnFileName);
	DDX_Text(pDX, IDC_PART_NUM, m_PartNum);
	DDX_Text(pDX, IDC_FILE_TO_DIVIDE, m_DivideFileName );
	DDX_Text(pDX, IDC_FILE_SIZE, m_FileSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileDivideDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDivideDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_FILE, OnConnectFile)
	ON_BN_CLICKED(IDC_DIVIDE_FILE, OnDivideFile)
	ON_BN_CLICKED(IDC_SELECT_FILE_TO_CONNECT, OnSelectFileToConnect)
	ON_BN_CLICKED(IDC_SELECT_FILE_TO_DIVIDE, OnSelectFileToDivide)
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDivideDlg message handlers

BOOL CFileDivideDlg::OnInitDialog()
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
	uPartSizeList[0] = 512 * 1024 ;
	for ( UINT i = 1; i < PART_SIZE_NUM; i++ )
		uPartSizeList[i] = uPartSizeList[i-1] + uPartSizeList[0] ;

	CString TempStr ;
	for ( UINT i = 0; i < PART_SIZE_NUM; i++ )
	{
		TempStr.Format ( "%dK", (i+1)*512 ) ;
		m_ComList.AddString ( TempStr ) ;
	}
	m_ComList.SetCurSel ( 1 ) ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileDivideDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileDivideDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileDivideDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFileDivideDlg::OnConnectFile() 
{
	if ( this->m_ConnFileName.IsEmpty() )
	{
		this->MessageBox ( "请选择要还原的文件!" ) ;
		return ;
	}

	// 取得文件信息
	DWORD dwRet = FD.FDGetFileInfo ( this->m_ConnFileName, &this->FileItemCon ) ;
	if ( dwRet )
	{
		CString ErrMsg ;
		GetErrorMessage ( dwRet, ErrMsg.GetBuffer ( 512 )  ) ;
		this->MessageBox ( ErrMsg ) ;
		return ;
	}

	dwRet = FD.FDFileConnect ( &FileItemCon ) ;
	if ( dwRet )
	{
		CString ErrMsg ;
		GetErrorMessage ( dwRet, ErrMsg.GetBuffer ( 512 )  ) ;
		this->MessageBox ( ErrMsg ) ;
		return ;
	}
	else
	{
		this->MessageBox ( "文件还原成功!" ) ;
	}
}

void CFileDivideDlg::OnDivideFile() 
{
	if ( this->m_DivideFileName.IsEmpty() )
	{
		this->MessageBox ( "请选择要分割的文件!" ) ;
		return ;
	}

	if ( this->m_PartNum <= 1 )
	{
		this->MessageBox ( "文件块数量过少，无法分割!请调整块大小!" ) ;
		return ;
	}
	
	// 分割文件
	DWORD dwRet = FD.FDFileDivide ( &FileItemDiv ) ;
	if ( dwRet )
	{
		CString ErrMsg ;
		GetErrorMessage ( dwRet, ErrMsg.GetBuffer ( 512 )  ) ;
		this->MessageBox ( ErrMsg ) ;
		return ;
	}
	else
	{
		this->MessageBox ( "文件分割成功!" ) ;
	}
}

void CFileDivideDlg::OnSelectFileToConnect() 
{
	// 设置过滤器
	static char szFilter[] = "Divide Information File (*.INFO)|*.INFO||";
	CFileDialog dlg ( 
		true,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter,
		NULL);
	
	if ( dlg.DoModal() == IDOK )
	{
		this->m_ConnFileName = dlg.GetPathName() ;
		this->UpdateData ( false ) ;
	}
}

void CFileDivideDlg::OnSelectFileToDivide() 
{
	CFileDialog dlg ( true ) ;
	if ( dlg.DoModal() == IDOK )
	{
		// 取得所选择的路径全名，并显示到界面
		this->m_DivideFileName = dlg.GetPathName() ;
		this->UpdateData ( false ) ;

		// 产生文件信息头
		DWORD dwRet = FD.FDGenFileInfo ( this->m_DivideFileName, &FileItemDiv ) ;
		if ( dwRet )
		{
			CString ErrMsg ;
			GetErrorMessage ( dwRet, ErrMsg.GetBuffer ( 512 )  ) ;
			this->MessageBox ( ErrMsg ) ;
			return ;
		}
	
		// 在界面中显示文件信息
		this->m_FileSize	= FileItemDiv.dwLowFileSize ;
		this->m_PartNum		= FileItemDiv.dwPartNum ;
		this->UpdateData ( false ) ;

		m_ComList.SetCurSel ( 1 ) ;
	}
}


void CFileDivideDlg::GetErrorMessage ( DWORD dwErrorCode, char pMessage[] )
{
	HLOCAL hLocal = NULL ;
	FormatMessage ( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		dwErrorCode,
		MAKELANGID ( LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(PTSTR)&hLocal, 
		0,
		NULL 
		) ;
	
	if ( hLocal != NULL )
	{
		strcpy ( pMessage, (char*)hLocal ) ;
		LocalFree ( hLocal ) ;
	}
}

void CFileDivideDlg::OnSelchangeCombo() 
{
	// TODO: Add your control notification handler code here
	UINT uCurSel = m_ComList.GetCurSel () ;
	FileItemDiv.dwPartSize	= this->uPartSizeList[uCurSel] ;
	FileItemDiv.dwPartNum	= FileItemDiv.dwLowFileSize / FileItemDiv.dwPartSize ;
	if ( FileItemDiv.dwLowFileSize % FileItemDiv.dwPartSize > 0 )
		FileItemDiv.dwPartNum += 1 ;

	this->m_PartNum = FileItemDiv.dwPartNum ;
	this->UpdateData ( false ) ;
}
