// DES ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DES Tool.h"
#include "DES ToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

static BYTE bBitMask[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 } ;

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
// CDESToolDlg dialog

CDESToolDlg::CDESToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDESToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDESToolDlg)
	m_DecryptedMsg = _T("");
	m_EncryptedMsg = _T("");
	m_OriginalMsg = _T("");
	m_PrivateKey = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDESToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDESToolDlg)
	DDX_Text(pDX, IDC_DECRYPTED_MSG, m_DecryptedMsg);
	DDX_Text(pDX, IDC_ENCRYPTED_MSG, m_EncryptedMsg);
	DDX_Text(pDX, IDC_ORIDINAL_MSG, m_OriginalMsg);
	DDX_Text(pDX, IDC_PRIVATE_KEY, m_PrivateKey);
	DDV_MaxChars(pDX, m_PrivateKey, 8);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDESToolDlg, CDialog)
	//{{AFX_MSG_MAP(CDESToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_ORIDINAL_MSG, OnChangeOridinalMsg)
	ON_EN_CHANGE(IDC_PRIVATE_KEY, OnChangePrivateKey)
	ON_BN_CLICKED(IDC_BIN_ORI, OnBinOri)
	ON_BN_CLICKED(IDC_BIN_EN, OnBinEn)
	ON_BN_CLICKED(IDC_BIN_DE, OnBinDe)
	ON_BN_CLICKED(IDC_BIN_KEY, OnBinKey)
	ON_BN_CLICKED(IDC_SHOW_SUB_KEY, OnShowSubKey)
	ON_BN_CLICKED(IDC_ENCRYPTED, OnEncrypted)
	ON_BN_CLICKED(IDC_DECRYPTED, OnDecrypted)
	ON_BN_CLICKED(IDC_SHOW_TRANS_PROC, OnShowTransProc)
	ON_BN_CLICKED(IDC_DES_INFO, OnHelp)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDESToolDlg message handlers

BOOL CDESToolDlg::OnInitDialog()
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
	m_OriginalMsg	= _T("learning");
	m_PrivateKey	= _T("computer");
	this->UpdateData ( false ) ;

	this->uLastOp = 0 ;
	DESSystem.DES_SysInit () ;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDESToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDESToolDlg::OnPaint() 
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
HCURSOR CDESToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 把字符串转化为二进制串
CString CDESToolDlg::ConvertStringToBin ( PBYTE pByte, UINT uLen, char ch )
{
	CString TempStr ;
	for ( UINT i = 0; i < uLen; i++ )
	{
		for ( UINT j = 0; j < 8; j++ )
		{
			if ( pByte[i] & bBitMask[(~j)&7] )
				TempStr += '1' ;
			else
				TempStr += '0' ;
		}

		if ( ch == ' ' )
			continue ;

		if ( ( i + 1 ) % 8 == 0 )
			TempStr += '\n' ;
		else
			TempStr += ' ' ;
	}
	TempStr += ch ;

	return TempStr ;
}

void CDESToolDlg::OnChangeOridinalMsg() 
{
	this->UpdateData () ;
}

void CDESToolDlg::OnChangePrivateKey() 
{
	this->UpdateData () ;
}

// 显示明文二进制形式
void CDESToolDlg::OnBinOri() 
{
	UINT uLen = m_OriginalMsg.GetLength() ;
	CString BinInfo = this->ConvertStringToBin ( (PBYTE)m_OriginalMsg.GetBuffer(uLen), uLen, '\n' ) ;
	this->MessageBox ( BinInfo, "Show Binary--OiginalMsg" ) ;
}

// 显示密文二进制形式
void CDESToolDlg::OnBinEn() 
{
	UINT uLen = m_EncryptedMsg.GetLength() ;
	CString BinInfo = this->ConvertStringToBin ( (PBYTE)m_EncryptedMsg.GetBuffer(uLen), uLen, '\n' ) ;
	this->MessageBox ( BinInfo, "Show Binary--EncryptedMsg" ) ;
}

// 显示解密密文二进制形式
void CDESToolDlg::OnBinDe() 
{
	UINT uLen = m_DecryptedMsg.GetLength() ;
	CString BinInfo = this->ConvertStringToBin ( (PBYTE)m_DecryptedMsg.GetBuffer(uLen), uLen, '\n' ) ;
	this->MessageBox ( BinInfo, "Show Binary--DecryptedMsg" ) ;
}

// 显示密钥的二进制形式
void CDESToolDlg::OnBinKey() 
{
	UINT uLen = m_PrivateKey.GetLength() ;
	CString BinInfo = this->ConvertStringToBin ( (PBYTE)m_PrivateKey.GetBuffer(uLen), uLen, '\n' ) ;
	this->MessageBox ( BinInfo, "Show Binary--PrivateKey" ) ;
}

// 显示子密钥
void CDESToolDlg::OnShowSubKey() 
{
	if ( this->m_PrivateKey.IsEmpty() )
	{
		this->MessageBox ( "Please input Private Key!" ) ; 
		return ;
	}

	UINT uLen = m_PrivateKey.GetLength () ;
	DESSystem.DES_SetKey ( (PBYTE)m_PrivateKey.GetBuffer(uLen), uLen ) ;
	DESSystem.DESGenSubKey ( ) ;

	CString TempInfo	= "" ;
	CString	TotalInfo	= "\n------------------------ Sub Key Info ----------------------\n\n" ;

	for ( UINT i = 1; i <= 16; i++ )
	{
		if ( i < 10 )
			TempInfo.Format ( "Key  %d  :  ", i ) ;
		else
			TempInfo.Format ( "Key %d  :  ", i ) ;

		TotalInfo += TempInfo ;
		TotalInfo += this->ConvertStringToBin ( DESSystem.bSubKey[i], 6, '\n' ) ;
		TotalInfo += '\n' ;
	}

	this->MessageBox ( TotalInfo, "Show 16 Sub Key" ) ;
}

// 加密过程
void CDESToolDlg::OnEncrypted() 
{
	if ( this->m_PrivateKey.IsEmpty() )
	{
		this->MessageBox ( "Please input Private Key!" ) ; 
		return ;
	}
	
	if ( this->m_OriginalMsg.IsEmpty() )
	{
		this->MessageBox ( "Please input Original message!" ) ;
		return ;
	}

	this->uLastOp = 1 ;

	UINT uLen = m_PrivateKey.GetLength () ;
	DESSystem.DES_SetKey ( (PBYTE)m_PrivateKey.GetBuffer(uLen), uLen ) ;

	uLen = m_OriginalMsg.GetLength() ;
	UINT	uGroup	= ( ( uLen & 7 ) == 0 ? ( uLen / 8 ) : ( uLen / 8 + 1 ) ) ; 

	this->m_EncryptedMsg = "" ;
	PBYTE	pByte	= (PBYTE)m_OriginalMsg.GetBuffer(uLen) ;
	BYTE	bBuf[9] = {0} ;
	for ( UINT i = 0, uStart = 0; i < uGroup; i++, uStart += 8 )
	{
		DESSystem.DES_SetOriMsg ( &(pByte[uStart]), 8 ) ;
		DESSystem.DES_Encrypt () ;
		memcpy ( bBuf, DESSystem.bCryptedMsg, 8 ) ;
		this->m_EncryptedMsg += (char*)bBuf ;
	}

	this->UpdateData ( false ) ;
	//this->MessageBox ( "Encrypted Successful!" ) ;
}

// 解密过程
void CDESToolDlg::OnDecrypted() 
{
	if ( this->m_PrivateKey.IsEmpty() )
	{
		this->MessageBox ( "Please input Private Key!" ) ; 
		return ;
	}
	
	if ( this->m_EncryptedMsg.IsEmpty() )
	{
		this->MessageBox ( "No Encrypted message!" ) ;
		return ;
	}

	this->uLastOp = 2 ;

	UINT	uLen	= m_EncryptedMsg.GetLength() ;
	UINT	uGroup	= ( ( uLen & 7 ) == 0 ? ( uLen / 8 ) : ( uLen / 8 + 1 ) ) ;
	PBYTE	pByte	= (PBYTE)m_EncryptedMsg.GetBuffer(uLen) ;
	BYTE	bBuf[9] = {0} ;
	
	this->m_DecryptedMsg = "" ;
	for ( UINT i = 0, uStart = 0; i < uGroup; i++, uStart += 8 )
	{
		DESSystem.DES_SetOriMsg ( &(pByte[uStart]), 8 ) ;
		DESSystem.DES_Decrypt () ;
		memcpy ( bBuf, DESSystem.bCryptedMsg, 8 ) ;
		this->m_DecryptedMsg += (char*)bBuf ;
	}

	this->UpdateData ( false ) ;
	//this->MessageBox ( "Decrypted successful!" ) ;
}

// 显示16轮变换过程
void CDESToolDlg::OnShowTransProc() 
{
	CString TempInfo	= "" ;
	CString	TotalInfo	= "\n----------------------------------- " ;
		
	if ( uLastOp == 0 )
	{
		this->MessageBox ( "Havn't encrypt or decrypt!" ) ;
		return ;
	}
	else if ( uLastOp == 1 )
		TotalInfo += "Encrypted Trans Info" ;
	else if ( uLastOp == 2 )
		TotalInfo += "Decrypted Trans Info" ;
		
	TotalInfo += "----------------------------------\n\n" ;
		
	for ( UINT i = 1; i <= 16; i++ )
	{
		if ( i < 10 )
			TempInfo.Format ( "L[ %d]-L[ %d]  :  ", i, i ) ;
		else
			TempInfo.Format ( "L[%d]-L[%d]  :  ", i, i ) ;

		TotalInfo += TempInfo ;
		TotalInfo += this->ConvertStringToBin ( DESSystem.LMsg[i], 4, ' ' ) ;
		TotalInfo += this->ConvertStringToBin ( DESSystem.RMsg[i], 4, ' ') ;
		TotalInfo += "    \n\n" ;
	}

	this->MessageBox ( TotalInfo ) ;
}

void CDESToolDlg::OnHelp() 
{
	CFile file; 
	char* pFileName = "DES-Info.chm" ;
	if( !file.Open( pFileName, CFile::modeCreate | CFile::modeWrite, NULL ) )
	{
		AfxMessageBox("Can not create DES-Info file!");
		return ;
	}

	CString path = file.GetFilePath();
	HINSTANCE	insApp		= AfxGetInstanceHandle();
	HRSRC		hResInfo	= FindResource ( insApp, "DES-Info.chm", "DES" ) ;
	HGLOBAL		hRes		= LoadResource ( insApp, hResInfo ) ;  
	
	DWORD	dFileLength = SizeofResource( insApp, hResInfo );  
	file.Write((LPSTR)hRes,dFileLength); 
	file.Close();

	ShellExecute(NULL, "open",path, NULL, NULL, SW_SHOWNORMAL);
}

void CDESToolDlg::OnAbout() 
{
	CAboutDlg dlg ;
	dlg.DoModal () ;
}
