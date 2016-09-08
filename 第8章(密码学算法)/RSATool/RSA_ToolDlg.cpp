// RSA_ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RSA_Tool.h"
#include "RSA_ToolDlg.h"

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
// CRSA_ToolDlg dialog

CRSA_ToolDlg::CRSA_ToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRSA_ToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRSA_ToolDlg)
	m_FirstPrime = _T("");
	m_SecondPrime = _T("");
	m_PrivateKeyD = _T("");
	m_PublicKeyN = _T("");
	m_EncryptedMsg = _T("");
	m_DecryptedMsg = _T("");
	m_OriginalMsg = _T("");
	m_PublicKeyE = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Self Contruct 
	RSA_PContext = new RSA_CONTEXT ;
	RSA_InitContext ( RSA_PContext ) ;
}

void CRSA_ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRSA_ToolDlg)
	DDX_Control(pDX, IDC_KEYSIZE, m_KeySizeCon);
	DDX_Text(pDX, IDC_FIRST_PRIME, m_FirstPrime);
	DDV_MaxChars(pDX, m_FirstPrime, 1024);
	DDX_Text(pDX, IDC_SECOND_PRIME, m_SecondPrime);
	DDV_MaxChars(pDX, m_SecondPrime, 1024);
	DDX_Text(pDX, IDC_PUBLICKEY_E, m_PublicKeyE) ;
	DDX_Text(pDX, IDC_PRIVATEKEY_D, m_PrivateKeyD);
	DDV_MaxChars(pDX, m_PrivateKeyD, 1024);
	DDX_Text(pDX, IDC_PUBLICKEY_N, m_PublicKeyN);
	DDV_MaxChars(pDX, m_PublicKeyN, 1024);
	DDX_Text(pDX, IDC_ENCRYPTED_MESSAGE, m_EncryptedMsg);
	DDX_Text(pDX, IDC_DECRYPTED_MESSAGE, m_DecryptedMsg);
	DDX_Text(pDX, IDC_ORGINAL_MESSAGE, m_OriginalMsg);
	DDX_CBString(pDX, IDC_PUBLICKEY_E, m_PublicKeyE);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRSA_ToolDlg, CDialog)
	//{{AFX_MSG_MAP(CRSA_ToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GENERATE_TOTAL, OnGenerateTotal)
	ON_CBN_EDITCHANGE(IDC_KEYSIZE, OnEditchangeKeysize)
	ON_EN_CHANGE(IDC_ORGINAL_MESSAGE, OnChangeOrginalMessage)
	ON_EN_CHANGE(IDC_FIRST_PRIME, OnChangeFirstPrime)
	ON_EN_CHANGE(IDC_SECOND_PRIME, OnChangeSecondPrime)
	ON_EN_CHANGE(IDC_PUBLICKEY_E, OnChangePublicKeyE)
	ON_BN_CLICKED(IDC_GENERATE_N, OnGenerateN)
	ON_BN_CLICKED(IDC_GENERATE_D, OnGenerateD)
	ON_BN_CLICKED(IDC_DO_DECRYPT, OnDoDecrypt)
	ON_BN_CLICKED(IDC_DO_ENCRYPT, OnDoEncrypt)
	ON_BN_CLICKED(IDC_RAND_GENERATE_MESSAGE, OnRandGenerateMessage)
	ON_BN_CLICKED(IDC_ABOUTDLG, OnAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSA_ToolDlg message handlers

BOOL CRSA_ToolDlg::OnInitDialog()
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
	hFirstPrime		= this->GetDlgItem(IDC_FIRST_PRIME)->m_hWnd ;
	hSecondPrime	= this->GetDlgItem(IDC_SECOND_PRIME)->m_hWnd ;
	hPublicE		= this->GetDlgItem(IDC_PUBLICKEY_E)->m_hWnd ;

	//设置密钥长度组合框
	UINT i, uTempKeySize = 32 ;
	CString TempString = "" ;
	for ( i = 0; i < 128; i++, uTempKeySize += 32 )
	{
		TempString.Format ( "%5ld bits", uTempKeySize ) ;
		m_KeySizeCon.AddString ( TempString ) ;
	}
	m_KeySizeCon.SetCurSel ( 15 ) ;

	//设置公钥E的编辑框
	m_PublicKeyE = "11" ;
	this->UpdateData ( false ) ;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRSA_ToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRSA_ToolDlg::OnPaint() 
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
HCURSOR CRSA_ToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/****************************************************
 *				Self Defined Function				*
 ****************************************************/

void CRSA_ToolDlg::OnGenerateTotal() 
{
	// TODO: Add your control notification handler code here
	BeginWaitCursor() ;

	RSA_SetKeySize ( RSA_PContext, (m_KeySizeCon.GetCurSel()+1)*32 ) ;
	RSA_GenerateBigPrime ( RSA_PContext ) ; 
	RSA_UpdateContext ( RSA_PContext ) ;

	mip->IOBASE = 16 ;
	cotstr ( RSA_PContext->p, m_FirstPrime.GetBuffer(RSA_PContext->uKeySize) ) ;
	cotstr ( RSA_PContext->q, m_SecondPrime.GetBuffer(RSA_PContext->uKeySize) ) ;
	cotstr ( RSA_PContext->n, m_PublicKeyN.GetBuffer(RSA_PContext->uKeySize) ) ;
	cotstr ( RSA_PContext->d, m_PrivateKeyD.GetBuffer(RSA_PContext->uKeySize) ) ;
	this->UpdateData ( false ) ;

	// 释放CString对象的多余空间
	m_FirstPrime.ReleaseBuffer();
	m_SecondPrime.ReleaseBuffer();
	m_PublicKeyN.ReleaseBuffer();
	m_PrivateKeyD.ReleaseBuffer();

	if ( m_PublicKeyN.GetLength() > 256 )
	{
		m_EncryptedMsg = "Big key, need press 'Decrypte !!!" ;
		m_DecryptedMsg = "No encrypted message !!!" ;
		this->UpdateData ( false ) ;
	  EndWaitCursor() ;
		return ;
	}	
	else
	{
		OnDoEncrypt() ;
		OnDoDecrypt() ;
	}

	EndWaitCursor() ;
}

void CRSA_ToolDlg::OnEditchangeKeysize() 
{
	RSA_SetKeySize ( RSA_PContext, (m_KeySizeCon.GetCurSel()+1)*32 ) ;
}

void CRSA_ToolDlg::OnChangeOrginalMessage() 
{
	this->UpdateData() ;
	
	if ( !m_OriginalMsg.GetLength() )
	{
		m_EncryptedMsg = m_DecryptedMsg = "" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( m_PublicKeyN.GetLength() > 256 )
	{
		m_EncryptedMsg = "Big key, need press 'Encrypte !!!" ;
		m_DecryptedMsg = "No encrypted message !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	
	else
	{
		OnDoEncrypt() ;
		OnDoDecrypt() ;
	}
}

void CRSA_ToolDlg::OnChangeFirstPrime() 
{
	this->UpdateData () ;
}

void CRSA_ToolDlg::OnChangeSecondPrime() 
{
	this->UpdateData () ;
}

void CRSA_ToolDlg::OnChangePublicKeyE() 
{
	this->UpdateData() ;	
}

void CRSA_ToolDlg::OnGenerateN() 
{
	if ( !m_FirstPrime.GetLength() || !m_SecondPrime.GetLength() )
	{
		m_PublicKeyN = "No prime !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !IsHexString(m_FirstPrime) || !IsHexString(m_SecondPrime) )
	{
		m_PublicKeyN = "Prime is not valid hex-string !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}

	mip->IOBASE = 16 ;
	cinstr ( RSA_PContext->p, m_FirstPrime.GetBuffer (m_FirstPrime.GetLength()) ) ;
	cinstr ( RSA_PContext->q, m_SecondPrime.GetBuffer(m_SecondPrime.GetLength()) ) ;
	cinstr ( RSA_PContext->e, m_PublicKeyE.GetBuffer( m_PublicKeyE.GetLength() ) ) ;

	RSA_UpdateContext ( RSA_PContext ) ;

	cotstr ( RSA_PContext->n, m_PublicKeyN.GetBuffer(MAXKEYSIZE) ) ;
	m_PublicKeyN.ReleaseBuffer () ;

	this->UpdateData ( false ) ;
}

void CRSA_ToolDlg::OnGenerateD() 
{
	// TODO: Add your control notification handler code here
	if ( !m_FirstPrime.GetLength() || !m_SecondPrime.GetLength() )
	{
		m_PrivateKeyD = "No prime !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !IsHexString(m_FirstPrime) || !IsHexString(m_SecondPrime) )
	{
		m_PrivateKeyD = "Prime is not valid hex-string !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !m_PublicKeyE.GetLength() )
	{
		m_PrivateKeyD = "No public key E !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}

	mip->IOBASE = 16 ;
	cinstr ( RSA_PContext->p, m_FirstPrime.GetBuffer (m_FirstPrime.GetLength()) ) ;
	cinstr ( RSA_PContext->q, m_SecondPrime.GetBuffer(m_SecondPrime.GetLength()) ) ;
	cinstr ( RSA_PContext->e, m_PublicKeyE.GetBuffer( m_PublicKeyE.GetLength() ) ) ;

	RSA_UpdateContext ( RSA_PContext ) ;

	cotstr ( RSA_PContext->d, m_PrivateKeyD.GetBuffer(MAXKEYSIZE) ) ;
	m_PrivateKeyD.ReleaseBuffer () ;

	this->UpdateData ( false ) ;
}

void CRSA_ToolDlg::OnDoEncrypt() 
{	
	if ( !m_FirstPrime.GetLength() || !m_SecondPrime.GetLength() )
	{
		m_EncryptedMsg = "No prime !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !m_PublicKeyN.GetLength() )
	{
		m_EncryptedMsg = "No public key N !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	
	else if ( !m_PublicKeyE.GetLength() )
	{
		m_EncryptedMsg = "No public key E !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !m_OriginalMsg.GetLength() )
	{
		m_EncryptedMsg = "No message !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	

	BeginWaitCursor() ;

	//把消息转化为大数
	big DigitalMsg = mirvar(0), EncryptedMsg = mirvar(0), DecryptedMsg = mirvar(0) ;
	bytes_to_big ( m_OriginalMsg.GetLength(),	\
		m_OriginalMsg.GetBuffer(m_OriginalMsg.GetLength()), DigitalMsg ) ;

	//加密消息，并以大数形式保存到EncyptedMsg
	RSA_EncryptMessage ( RSA_PContext, DigitalMsg, EncryptedMsg ) ;

	//把密文转化为字符串
	cotstr ( EncryptedMsg, m_EncryptedMsg.GetBuffer(MAXKEYSIZE) ) ;
	m_EncryptedMsg.ReleaseBuffer() ;

	EndWaitCursor() ;

	this->UpdateData( false ) ;
}

void CRSA_ToolDlg::OnDoDecrypt() 
{
	if ( !m_FirstPrime.GetLength() || !m_SecondPrime.GetLength() )
	{
		m_DecryptedMsg = "No prime !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}
	else if ( !m_PublicKeyE.GetLength() )
	{
		m_DecryptedMsg = "No public key E !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	
	else if ( !m_PrivateKeyD.GetLength() )
	{
		m_DecryptedMsg = "No private key D !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	
	else if ( !m_EncryptedMsg.GetLength() || !IsHexString(m_EncryptedMsg) )
	{
		m_DecryptedMsg = "No crypted message !!!" ;
		this->UpdateData ( false ) ;
		return ;
	}	

	BeginWaitCursor() ;

	big EncryptedMsg = mirvar(0), DecryptedMsg = mirvar(0) ;
	cinstr ( EncryptedMsg, m_EncryptedMsg.GetBuffer( m_EncryptedMsg.GetLength() ) ) ;

	//解密，并把解密后的消息以大数形式保存在EdcryptedMsg
	RSA_DecryptMessage ( RSA_PContext, EncryptedMsg, DecryptedMsg ) ;

	//把解密消息转化为字符串
	char pBuf[MAXKEYSIZE] = {0} ;
	big_to_bytes ( 0, DecryptedMsg,	pBuf, false ) ;
	m_DecryptedMsg.Format ( "%s", pBuf ) ;

	EndWaitCursor() ;

	this->UpdateData ( false ) ;
}

void CRSA_ToolDlg::OnRandGenerateMessage() 
{
	srand ( time(NULL) ) ;

	m_OriginalMsg = "" ;
	for ( UINT i = 0; i < 50; i++ ) 
		m_OriginalMsg += (char)(rand() % 0xA0) ;
	this->UpdateData ( false ) ;

	OnChangeOrginalMessage() ;
}

BOOL CRSA_ToolDlg::IsHexString ( CString TempString )
{
	char  TempCh;
	UINT i, uLen = TempString.GetLength() ;
	for ( i = 0; i < uLen; i++ )
	{
		TempCh = TempString.GetAt(i) ;
		if ( !( TempCh >= '0' && TempCh <= '9' ) && 
			 !( TempCh >= 'A' && TempCh <= 'F' ) )
		{
			return FALSE ;
		}
	}
	return TRUE ;
}

void CRSA_ToolDlg::OnAboutDlg ()
{
	CDialog AboutDlg ( IDD_ABOUTBOX, this ) ;
	AboutDlg.DoModal() ;
}