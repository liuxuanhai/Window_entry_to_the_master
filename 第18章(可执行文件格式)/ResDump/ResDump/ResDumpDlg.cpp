// ResDumpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ResDump.h"
#include "ResDumpDlg.h"

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


// CResDumpDlg 对话框




CResDumpDlg::CResDumpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResDumpDlg::IDD, pParent)
	, m_FilePath(_T(""))
	, m_RootNameEntryNum(_T(""))
	, m_RootIdEntryNum(_T(""))
	, m_SubNameEntryNum(_T(""))
	, m_SubIdEntryNum(_T(""))
	, m_DataOffset(_T(""))
	, m_DataRVA(_T(""))
	, m_DataSize(_T(""))
	, m_ResInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for ( int i = 0; i < MAX_DEF_RES_TYPE; i++ )
		this->szResourceType[i] = TEXT("") ;

	this->szResourceType[1]		= TEXT("Cursor") ;
	this->szResourceType[2]		= TEXT("Bitmap") ;
	this->szResourceType[3]		= TEXT("Icon") ;
	this->szResourceType[4]		= TEXT("Menu") ;
	this->szResourceType[5]		= TEXT("Dialog") ;
	this->szResourceType[6]		= TEXT("String") ;
	this->szResourceType[7]		= TEXT("FontDir") ;
	this->szResourceType[8]		= TEXT("Font") ;
	this->szResourceType[9]		= TEXT("Accelerator") ;
	this->szResourceType[10]	= TEXT("Raw data") ;
	this->szResourceType[11]	= TEXT("Message Table") ;
	this->szResourceType[12]	= TEXT("Group Cursor") ;
	this->szResourceType[14]	= TEXT("Group Icon") ;
	this->szResourceType[16]	= TEXT("Version") ;
	this->szResourceType[21]	= TEXT("Animated Cursor") ;
	this->szResourceType[22]	= TEXT("Animated Icon") ;
}

void CResDumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RES_DIRECTORY, m_ResDirectory);
	DDX_Text(pDX, IDC_PATH, m_FilePath);
	DDX_Text(pDX, IDC_ROOT_NAMEENTRYNUM, m_RootNameEntryNum);
	DDX_Text(pDX, IDC_ROOT_IDENTRYNUM, m_RootIdEntryNum);
	DDX_Text(pDX, IDC_SUB_NAMEENTRYNUM, m_SubNameEntryNum);
	DDX_Text(pDX, IDC_SUB_IDENTRYNUM, m_SubIdEntryNum);
	DDX_Text(pDX, IDC_DATA_OFFSET, m_DataOffset);
	DDX_Text(pDX, IDC_DATA_RVA, m_DataRVA);
	DDX_Text(pDX, IDC_DATA_SIZE, m_DataSize);
	DDX_Text(pDX, IDC_INFO, m_ResInfo);
}

BEGIN_MESSAGE_MAP(CResDumpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CResDumpDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CResDumpDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BROWSE, &CResDumpDlg::OnBnClickedBrowse)
	ON_NOTIFY(TVN_SELCHANGED, IDC_RES_DIRECTORY, &CResDumpDlg::OnTvnSelchangedResDirectory)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CResDumpDlg 消息处理程序

BOOL CResDumpDlg::OnInitDialog()
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

	// 树型控件初始化，完全清空
	this->m_ResDirectory.DeleteAllItems () ;

	this->isDumpValid = FALSE ;
	this->lpDumpResStart = NULL ;
	this->dwResSize = 0 ;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CResDumpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CResDumpDlg::OnPaint()
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
HCURSOR CResDumpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CResDumpDlg::GetResourceInfo(LPTSTR lpFilePath)
{
	if ( this->PEInfo.CreateMap ( lpFilePath ) == FALSE )
	{
		this->MessageBox ( TEXT("Please ensure this file is not been using !") ) ;
		return FALSE;
	}
	
	this->PEInfo.GetResInfo () ;
	return TRUE ;
}

void CResDumpDlg::OnBnClickedBrowse()
{
	if ( this->PEInfo.isReady == TRUE )
		this->PEInfo.DeleteMap () ;

	this->m_ResDirectory.DeleteAllItems () ;

	static TCHAR szFilter[] = TEXT ( "PE Files (*.exe)|*.exe||" ) ;
	CFileDialog dlg ( true, 0, 0, 4|2, szFilter ) ;
	if ( dlg.DoModal() == IDOK )
	{
		this->m_FilePath = dlg.GetPathName() ;
		this->UpdateData ( false ) ;
		if ( !this->GetResourceInfo ( this->m_FilePath.GetBuffer(this->m_FilePath.GetLength()) ) )
			return ;
		this->UpdateTreeCtrl () ;
		
		// 设置并显示附加信息
		DWORD	dwVirtualAddr	= this->PEInfo.pOptionalHeader->DataDirectory[2].VirtualAddress ;
		DWORD	dwSize			= this->PEInfo.pOptionalHeader->DataDirectory[2].Size ;
		this->m_ResInfo.Format ( TEXT("资源信息: FileOffset=0X%08X, RVA=0X%08X, DataSize=0X%08X"), \
			(DWORD)this->PEInfo.lpResBase - (DWORD)this->PEInfo.MapItem.lpMapAddr, dwVirtualAddr, dwSize ) ;
		this->UpdateData ( false ) ;
	}
}

void CResDumpDlg::UpdateTreeCtrl(void)
{
	CString TempStr ;
	list<RES_ITEM>::iterator p ;
	HTREEITEM	pRoot, pSub ;
	DWORD	i, j, k, dwTypeNum = this->PEInfo.dwNamedEntry + this->PEInfo.dwIdEntry ;
	for ( i = 0; i < dwTypeNum; i++ )
	{
		// 判断资源类型为ID/字符串
		if ( this->PEInfo.ResDirectory[i].dwNameOrId & 0x80000000 )
		{
			LPTSTR	lpStr	= (LPTSTR)((this->PEInfo.ResDirectory[i].dwNameOrId & 0x7fffffff)+(DWORD)this->PEInfo.lpResBase );
			WORD	nNum	= *((short int*)lpStr) ;		// 所指的首字符（TCHAR）为字符串长度
			TempStr.Format ( TEXT("%s"), (LPTSTR)((DWORD)lpStr+2) ) ;
			if ( TempStr.GetLength() > nNum )				// 根据对应的长度进行截取
				TempStr.SetAt ( nNum, '\0' ) ;
		}
		else
		{
			DWORD dwResType = this->PEInfo.ResDirectory[i].dwNameOrId ;
			if ( dwResType < MAX_DEF_RES_TYPE && this->szResourceType[dwResType] != TEXT("") )
				TempStr.Format ( TEXT("%s"), this->szResourceType[dwResType] ) ;
			else
				TempStr.Format ( TEXT("%d"), this->PEInfo.ResDirectory[i].dwNameOrId ) ;
		}

		pRoot = this->m_ResDirectory.InsertItem ( TempStr.GetBuffer(TempStr.GetLength()) ) ;

		for ( p = this->PEInfo.ResList[i].begin(); p != this->PEInfo.ResList[i].end(); p++ )
		{
			// file offset
			// DWORD tt = (p->dwNameOrId & 0x7fffffff) + (DWORD)this->PEInfo.lpResBase - (DWORD)this->PEInfo.MapItem.lpMapAddr ;
			if ( p->dwNameOrId & 0x80000000 )
			{
				LPTSTR	lpStr	= (LPTSTR)((p->dwNameOrId & 0x7fffffff)+(DWORD)this->PEInfo.lpResBase ) ;
				WORD	nNum	= *((short int*)lpStr) ;
				TempStr.Format ( TEXT("%s"), (LPTSTR)((DWORD)lpStr+2) ) ;
				if ( TempStr.GetLength() > nNum )
					TempStr.SetAt ( nNum, '\0' ) ;
			}
			else
				TempStr.Format ( TEXT("%d"), p->dwNameOrId ) ;
			
			pSub = this->m_ResDirectory.InsertItem ( TempStr.GetBuffer(TempStr.GetLength()), pRoot ) ;
		}
	}

	this->m_RootNameEntryNum.Format ( TEXT("%04X"), this->PEInfo.dwNamedEntry ) ;
	this->m_RootIdEntryNum.Format ( TEXT("%04X"), this->PEInfo.dwIdEntry ) ;
	this->UpdateData ( false ) ;
}

// DUMP Resource
void CResDumpDlg::OnBnClickedOk()
{
	if ( this->m_FilePath.IsEmpty() == TRUE )
	{
		this->MessageBox ( TEXT("You should select a PE File first!") ) ;
		return ;
	}
	if ( this->isDumpValid == FALSE )
	{
		this->MessageBox ( TEXT("Can't dump directory!"), TEXT("Warning") ) ;
		return ;
	}
	static TCHAR szFilter[] = TEXT ( "Resource Dump File (*.dmp)|*.dmp||" ) ;
	CFileDialog dlg ( false, 0, 0, 4|2, szFilter ) ;
	if ( dlg.DoModal() == IDOK )
	{
		CString szTarFile =	dlg.GetPathName () ;
		szTarFile += TEXT(".dmp") ;
		if ( this->PEInfo.DumpResource ( szTarFile.GetBuffer(szTarFile.GetLength()), this->lpDumpResStart, this->dwResSize ) == TRUE )
			this->MessageBox ( TEXT("Dump Successful!") ) ;
		else
			this->MessageBox ( TEXT("Dump failed!") ) ;
	}
	
}

void CResDumpDlg::OnBnClickedCancel()
{
	if ( this->PEInfo.isReady == TRUE )
		this->PEInfo.DeleteMap () ;
	OnCancel();
}

int CResDumpDlg::GetIndexOfTreeCtrl(HTREEITEM hTreeItem)
{
	int nCount = 0 ;
	HTREEITEM	hItem = hTreeItem ;
	while ( (hItem=this->m_ResDirectory.GetNextItem(hItem,TVGN_PREVIOUS)) != NULL )
		nCount ++ ;
	return nCount + 1;
}

void CResDumpDlg::OnTvnSelchangedResDirectory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码	
	HTREEITEM	hParent = NULL, hChild = NULL, hCurSel = NULL, hTemp = NULL ;
	hCurSel = this->m_ResDirectory.GetNextItem ( TVI_ROOT, TVGN_CARET ) ;

	hParent	= this->m_ResDirectory.GetParentItem ( hCurSel ) ;
	hChild	= this->m_ResDirectory.GetChildItem ( hCurSel ) ;

	RES_ITEM	CurResItem ;
	int	nRootIndex, nSubIndex ;
	if ( hParent == NULL )
	{
		nRootIndex	= this->GetIndexOfTreeCtrl ( hCurSel ) ;
		this->m_DataOffset = this->m_DataRVA = this->m_DataSize = "" ;
		this->isDumpValid = FALSE ;
	}
	else
	{
		nRootIndex	= this->GetIndexOfTreeCtrl ( hParent ) ;
		nSubIndex	= this->GetIndexOfTreeCtrl ( hCurSel ) ;

		this->PEInfo.GetResItemByIndex ( nRootIndex, nSubIndex, &CurResItem ) ;
		this->m_DataRVA.Format ( TEXT("%08X"), CurResItem.dwDataOffset ) ;
		this->m_DataSize.Format ( TEXT("%08X"), CurResItem.dwDataSize ) ;
		
		DWORD	dwFileOffset = (DWORD)ImageRvaToVa(this->PEInfo.pNtHeader,NULL, CurResItem.dwDataOffset,NULL) ;
		this->m_DataOffset.Format ( TEXT("%08X"), dwFileOffset ) ;

		this->isDumpValid = TRUE ;
		this->lpDumpResStart = (LPVOID)((DWORD)this->PEInfo.MapItem.lpMapAddr + dwFileOffset ) ;
		this->dwResSize = CurResItem.dwDataSize ;
	}

	this->m_SubNameEntryNum.Format ( TEXT("%04X"), this->PEInfo.ResDirectory[nRootIndex-1].wNumOfNamedEntry ) ;
	this->m_SubIdEntryNum.Format ( TEXT("%04X"), this->PEInfo.ResDirectory[nRootIndex-1].wNumOfIdEntry ) ;
	this->UpdateData ( false ) ;

	*pResult = 0;
}

BOOL CResDumpDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CDialog::OnEraseBkgnd(pDC);
}


BOOL WINAPI Dlg_Proc ( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg ) 
	{
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDCANCEL:
			EndDialog ( hDlg, 0 ) ;
			return true ;
		}
		break ;
	}
	
	return false ;
}