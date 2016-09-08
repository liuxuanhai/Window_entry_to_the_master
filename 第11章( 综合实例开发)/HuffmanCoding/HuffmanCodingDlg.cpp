// HuffmanCodingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HuffmanCoding.h"
#include "HuffmanCodingDlg.h"

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
// CHuffmanCodingDlg dialog

CHuffmanCodingDlg::CHuffmanCodingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHuffmanCodingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHuffmanCodingDlg)
	m_EncodingString = _T("");	
	m_CodedFileName = _T("");
	m_OriginalFileName = _T("");
	m_DecodingString = _T("");
	m_TextString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHuffmanCodingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHuffmanCodingDlg)
	DDX_Text(pDX, IDC_ENCODING_STRING, m_EncodingString);	
	DDX_Text(pDX, IDC_CODED_FILENAME, m_CodedFileName);
	DDX_Text(pDX, IDC_ORIGINAL_FILENAME, m_OriginalFileName);
	DDX_Text(pDX, IDC_DECODING_STRING, m_DecodingString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHuffmanCodingDlg, CDialog)
	//{{AFX_MSG_MAP(CHuffmanCodingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENCODING, OnEncoding)
	ON_BN_CLICKED(IDC_DECODING, OnDecoding)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_SEL_ORIGINAL_FILE, OnSelOriginalFile)
	ON_BN_CLICKED(IDC_SEL_CODED_FILE, OnSelCodedFile)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuffmanCodingDlg message handlers

BOOL CHuffmanCodingDlg::OnInitDialog()
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
	//初始化列表控件
	CListCtrl*	m_CodingInfoList = GetListCtrl() ;
	m_CodingInfoList->InsertColumn ( 0, "ASCII",	LVCFMT_CENTER, 45 ) ;
	m_CodingInfoList->InsertColumn ( 1, "字符",		LVCFMT_CENTER, 45 ) ;
	m_CodingInfoList->InsertColumn ( 2, "记数",		LVCFMT_CENTER, 60 ) ;
	m_CodingInfoList->InsertColumn ( 3, "编码串",	LVCFMT_CENTER, 200 ) ;
	m_CodingInfoList->InsertColumn ( 4, "位数",		LVCFMT_CENTER, 60 ) ;
	m_CodingInfoList->SetExtendedStyle ( m_CodingInfoList->GetExtendedStyle()|LVS_EX_FULLROWSELECT ) ;

	OnRadio1() ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHuffmanCodingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHuffmanCodingDlg::OnPaint() 
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
HCURSOR CHuffmanCodingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/************************************************
 *				Self-Defined Code				*
 ************************************************/

CListCtrl*	CHuffmanCodingDlg::GetListCtrl()
{
	return (CListCtrl*)this->GetDlgItem ( IDC_CODING_INFO_LIST ) ;
}

void CHuffmanCodingDlg::OnEncoding() 
{
	this->GetDlgItemText ( IDC_TEXT_STRING, m_TextString ) ;
	this->UpdateData ( false ) ;
	this->SetTimer ( 1, 1, NULL ) ;

	if ( bOperFlag ) 
	{
		if ( m_TextString.GetLength() != 0 )
		{
			m_EncodingString = TextHuffSys.TextHuffEncoding ( m_TextString ) ;
			ShowCodingInfoList () ;	
			this->UpdateData ( false ) ;
		}
	}
	else
	{
		if ( m_OriginalFileName.GetLength() == 0 )
			this->MessageBox ( "请选择原始文件!" ) ;
		else
		{
			this->SetDlgItemText ( IDC_FLAG, "编码信息——正在编码……" ) ;
			FileHuffSys.FileHuffEncoding ( m_OriginalFileName ) ;
			ShowCodingInfoList() ;	
			this->SetDlgItemText ( IDC_FLAG, "编码信息" ) ;
		}
	}

	this->KillTimer ( 1 ) ;
}

void CHuffmanCodingDlg::OnDecoding() 
{
	this->UpdateData ( false ) ;
	this->SetTimer ( 1, 10, NULL ) ;

	LPBYTE lpStr = NULL ;
	if ( bOperFlag ) 
	{
		if ( m_EncodingString.GetLength() != 0 )
		{
			lpStr = TextHuffSys.TextHuffDecoding ( m_EncodingString ) ;
			this->m_DecodingString.Format ( "%s", (char*)lpStr ) ;
		}
		else
			m_DecodingString = "" ;	
		this->UpdateData ( false ) ;
	}
	else
	{
		if ( m_CodedFileName.GetLength() == 0 )
			this->MessageBox ( "请选择已编码文件!" ) ;
		else
		{
			this->SetDlgItemText ( IDC_FLAG, "编码信息——正在解码……" ) ;
			FileHuffSys.FileHuffDecoding ( m_CodedFileName ) ;
			ShowCodingInfoList() ;	
			this->SetDlgItemText ( IDC_FLAG, "编码信息" ) ;
		}
	}

	this->KillTimer ( 1 ) ;
}

//选择当前的操作对象为输入的字符串
void CHuffmanCodingDlg::OnRadio1() 
{
	bOperFlag	= true ;
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck( BST_CHECKED ) ;

	((CEdit*)GetDlgItem(IDC_TEXT_STRING))->EnableWindow ( true ) ;
	((CEdit*)GetDlgItem(IDC_ENCODING_STRING))->EnableWindow ( true ) ;
	((CEdit*)GetDlgItem(IDC_DECODING_STRING))->EnableWindow ( true ) ;

	((CEdit*)GetDlgItem(IDC_ORIGINAL_FILENAME))->EnableWindow ( false ) ;
	((CEdit*)GetDlgItem(IDC_CODED_FILENAME))->EnableWindow ( false ) ;
	((CButton*)GetDlgItem(IDC_SEL_ORIGINAL_FILE))->EnableWindow ( false ) ;
	((CButton*)GetDlgItem(IDC_SEL_CODED_FILE))->EnableWindow ( false ) ;
}

//选择当前的操作对象是文件
void CHuffmanCodingDlg::OnRadio2() 
{
	bOperFlag = false ;
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck( BST_CHECKED ) ;

	((CEdit*)GetDlgItem(IDC_TEXT_STRING))->EnableWindow ( false ) ;
	((CEdit*)GetDlgItem(IDC_ENCODING_STRING))->EnableWindow ( false ) ;
	((CEdit*)GetDlgItem(IDC_DECODING_STRING))->EnableWindow ( false ) ;

	((CEdit*)GetDlgItem(IDC_ORIGINAL_FILENAME))->EnableWindow ( true ) ;
	((CEdit*)GetDlgItem(IDC_CODED_FILENAME))->EnableWindow ( true ) ;
	((CButton*)GetDlgItem(IDC_SEL_ORIGINAL_FILE))->EnableWindow ( true ) ;
	((CButton*)GetDlgItem(IDC_SEL_CODED_FILE))->EnableWindow ( true ) ;	
}

// 在列表控件中显示频率及编码信息
void CHuffmanCodingDlg::ShowCodingInfoList ()
{
	CListCtrl*	m_CodingInfoList = this->GetListCtrl() ;
	m_CodingInfoList->DeleteAllItems () ;

	CHuffSystem* CurSys ;
	if ( bOperFlag )
		CurSys = &TextHuffSys ;
	else
		CurSys = &FileHuffSys ;

	UINT	nItemNum = 0 ;
	CString TempStr ;
	for ( int i = 0; i < 256; i++ )
	{
		if ( CurSys->Item[i].GetCount() )
		{
			TempStr.Format ( " %d", i ) ;
			m_CodingInfoList->InsertItem ( nItemNum, TempStr ) ;

			TempStr.Format ( "%c", i ) ;
			m_CodingInfoList->SetItemText ( nItemNum, 1, TempStr ) ;

			TempStr.Format ( "%d", CurSys->Item[i].GetCount() ) ;
			m_CodingInfoList->SetItemText ( nItemNum, 2, TempStr ) ;

			TempStr = CurSys->CodingInBinStr[i] ;
			m_CodingInfoList->SetItemText ( nItemNum, 3, TempStr ) ;

			TempStr.Format ( "%d", TempStr.GetLength() ) ;
			m_CodingInfoList->SetItemText ( nItemNum, 4, TempStr ) ;

			nItemNum ++ ;
		}
	}
}

void CHuffmanCodingDlg::OnSelOriginalFile() 
{
	CFileDialog dlg ( true ) ;
	if ( dlg.DoModal() == IDOK )
	{
		this->m_OriginalFileName = dlg.GetPathName() ;
		this->UpdateData ( false ) ;
	}
}

void CHuffmanCodingDlg::OnSelCodedFile() 
{
	CFileDialog dlg ( true, NULL, 0, 4|2, "哈夫曼编码文件(*.huf)|*.huf||" ) ;
	if ( dlg.DoModal() == IDOK )
	{
		this->m_CodedFileName = dlg.GetPathName() ;
		this->UpdateData ( false ) ;
	}	
}
