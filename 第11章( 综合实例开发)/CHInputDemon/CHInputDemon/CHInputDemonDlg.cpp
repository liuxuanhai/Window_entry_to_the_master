// CHInputDemonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CHInputDemon.h"
#include "CHInputDemonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_WORD_NUM	30720
WCHAR CHWord[MAX_WORD_NUM] ;


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


// CCHInputDemonDlg 对话框




CCHInputDemonDlg::CCHInputDemonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCHInputDemonDlg::IDD, pParent)
	, szString(_T(""))
	, szPY(_T(""))
	, nTotalWord(0)
	, nTotalPage(0)
	, nCurPage(0)
	, szSearchWord(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCHInputDemonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STRING, szString);
	DDX_Text(pDX, IDC_PINYIN, szPY);
	DDV_MaxChars(pDX, szPY, 32);
	DDX_Text(pDX, IDC_TOTAL_WORD, nTotalWord);
	DDX_Text(pDX, IDC_TOTAL_PAGE, nTotalPage);
	DDX_Text(pDX, IDC_CUR_PAGE, nCurPage);
	DDX_Text(pDX, IDC_SEARCH_WORD, szSearchWord);
}

BEGIN_MESSAGE_MAP(CCHInputDemonDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EMPTY, &CCHInputDemonDlg::OnBnClickedEmpty)
	ON_EN_CHANGE(IDC_PINYIN, &CCHInputDemonDlg::OnEnChangePinyin)
END_MESSAGE_MAP()


// CCHInputDemonDlg 消息处理程序

BOOL CCHInputDemonDlg::OnInitDialog()
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
	UINT n = sizeof(WCHAR) ;

	if ( this->PYTrie.PYT_LoadDic ( "PYWord.dic" ) == FALSE )
		this->MessageBox ( _T("没有找到拼音字典文件 PYWord.dic !") ) ;

	if ( this->LXWord.LX_LoadDic ( "LXWord.dic" ) == FALSE )
		this->MessageBox ( _T("没有找到联想字典文件 LXWord.dic !") ) ;

	this->OnEnChangePinyin () ;


	((CEdit*)this->GetDlgItem(IDC_PINYIN))->SetFocus () ;
	return FALSE ;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCHInputDemonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCHInputDemonDlg::OnPaint()
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
HCURSOR CCHInputDemonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCHInputDemonDlg::OnBnClickedEmpty()
{
	this->szString = _T("") ;
	this->UpdateData ( FALSE ) ;
}

void CCHInputDemonDlg::OnEnChangePinyin()
{
	int nBeg, nEnd ;
	((CEdit*)this->GetDlgItem(IDC_PINYIN))->GetSel ( nBeg, nEnd ) ;

	char szPYTemp[128] = {0};
	UINT nLen = ::GetDlgItemTextA ( this->m_hWnd, IDC_PINYIN, szPYTemp, 128 ) ;

	for ( UINT i = 0 ; i < nLen; i++ )
	{
		if ( szPYTemp[i] < 'a' || szPYTemp[i] > 'z' )
		{
			this->UpdateData ( FALSE ) ;
			nBeg -- ; nEnd -- ;
			((CEdit*)this->GetDlgItem(IDC_PINYIN))->SetSel ( nBeg, nEnd, FALSE ) ;
			return ;
		}
	}

	this->UpdateData () ;

	UINT dwWordNum = 0 ;
	if ( this->PYTrie.PYT_SearchWord ( szPYTemp, CHWord, dwWordNum ) )
	{
		this->nCurPage = 1 ;
		this->nTotalWord = dwWordNum ;

		this->nTotalPage = this->nTotalWord / 10 ;
		if ( ( this->nTotalWord % 10 ) > 0 )
			this->nTotalPage ++ ;

		this->UpdateWord () ;
	}
	else
	{
		this->szSearchWord = _T("") ;
		this->nTotalPage = 0 ;
		this->nTotalWord = 0 ;
		this->nCurPage = 0 ;
	}

	this->UpdateData ( FALSE ) ;
}

BOOL CCHInputDemonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_KEYUP )
	{
		switch ( pMsg->wParam )
		{
		case '0':	this->OnNum(0) ;	break ;
		case '1':	this->OnNum(1) ;	break ;
		case '2':	this->OnNum(2) ;	break ;
		case '3':	this->OnNum(3) ;	break ;
		case '4':	this->OnNum(4) ;	break ;
		case '5':	this->OnNum(5) ;	break ;
		case '6':	this->OnNum(6) ;	break ;
		case '7':	this->OnNum(7) ;	break ;
		case '8':	this->OnNum(8) ;	break ;
		case '9':	this->OnNum(9) ;	break ;
		case 0xBB:	this->OnNextPage() ;	break ;
		case 0XBD:	this->OnPrevPage() ;	break ;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CCHInputDemonDlg::OnPrevPage ()
{
	if ( this->nCurPage <= 1 )
		return ;
	this->nCurPage -- ;
	this->UpdateData ( FALSE ) ;
	this->UpdateWord () ;
}

VOID CCHInputDemonDlg::OnNextPage () 
{
	if ( this->nCurPage == 0 || this->nCurPage == this->nTotalPage )
		return ;
	this->nCurPage ++ ;
	this->UpdateData ( FALSE ) ;
	this->UpdateWord () ;
}

VOID CCHInputDemonDlg::OnNum ( UINT nIndex )
{
	if ( nIndex > 9 )
		return ;

	UINT nWordIndex = ( this->nCurPage - 1 ) * 10 ;
	nWordIndex += ( ( nIndex > 0 ) ? ( nIndex - 1 ) : 9  ) ;
	if ( nWordIndex >= this->nTotalWord )
		return ;

	this->szString += CHWord[nWordIndex] ;
	this->szPY = _T("") ;

	LPWSTR lpWord = (LPWSTR)this->szString.GetBuffer(this->szString.GetLength()) ;
	
	// 联想
	UINT	dwWordNum = 0 ;
	memset ( CHWord, 0, sizeof(CHWord) ) ;
	this->LXWord.LX_SearchWord ( lpWord, CHWord, dwWordNum ) ;
	if ( dwWordNum )
	{
		this->nCurPage = 1 ;
		this->nTotalWord = dwWordNum ;

		this->nTotalPage = this->nTotalWord / 10 ;
		if ( ( this->nTotalWord % 10 ) > 0 )
			this->nTotalPage ++ ;

		this->UpdateWord () ;
	}
	else
	{
		this->szSearchWord = _T("") ;
		this->nTotalPage = 0 ;
		this->nTotalWord = 0 ;
		this->nCurPage = 0 ;
	}

	this->UpdateData ( FALSE ) ;
}

VOID CCHInputDemonDlg::UpdateWord ()
{
	this->szSearchWord = _T("   ") ;

	UINT nStartPos = ( this->nCurPage - 1 ) * 10 ;
	for ( int i = 0; i < 10; i++ )
	{
		this->szSearchWord += CHWord[nStartPos+i] ;
		this->szSearchWord += _T("    ") ;
	}
	this->UpdateData ( FALSE ) ;
}