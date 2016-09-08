// NagGenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NagGen.h"
#include "NagGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_BUF_SIZE		2048
#define MAX_TITLE_SIZE		64
#define MAX_CONTENT_SIZE	256

TCHAR a[] = TEXT("fjda") ;
TCHAR b[] = TEXT("fdsadagfdsa") ;

void StubCode ()
{	
	PTCHAR pTitle	= NULL ;
	PTCHAR pContent	= NULL ;

	DWORD begin, end ;
	__asm
	{
		push	edx
		call	$+5
		pop		edx
		add		edx, 7
		mov		begin, edx
BEGIN:
		pushad
		push	0
		push	pTitle
		push	pContent
		push	0
		call	dword ptr MessageBoxW
		popad
END:	
		call	$+5
		pop		edx
		add		edx, -5
		mov		end, edx
		pop		edx
	}
}

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


// CNagGenDlg 对话框




CNagGenDlg::CNagGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNagGenDlg::IDD, pParent)
	, m_PathName(_T(""))
	, m_NagTitle(_T(""))
	, m_NagContent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNagGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH, m_PathName);
	DDX_Text(pDX, IDC_NAG_TITLE, m_NagTitle);
	DDV_MaxChars(pDX, m_NagTitle, 62);
	DDX_Text(pDX, IDC_NAG_CONTENT, m_NagContent);
	DDV_MaxChars(pDX, m_NagContent, 252);
}

BEGIN_MESSAGE_MAP(CNagGenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_PATH, &CNagGenDlg::OnEnChangePath)
	ON_EN_CHANGE(IDC_NAG_TITLE, &CNagGenDlg::OnEnChangeNagTitle)
	ON_EN_CHANGE(IDC_NAG_CONTENT, &CNagGenDlg::OnEnChangeNagContent)
	ON_BN_CLICKED(IDC_BROWSE, &CNagGenDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDOK, &CNagGenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNagGenDlg 消息处理程序

BOOL CNagGenDlg::OnInitDialog()
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
	this->m_NagTitle = TEXT("NAG Sample !") ;
	this->m_NagContent = TEXT ( "This is a nag sample !(Insert a NAG MessageBox to the original PE(exe) file" ) ;
	this->UpdateData ( false ) ;

	//StubCode () ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNagGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CNagGenDlg::OnPaint()
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
HCURSOR CNagGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNagGenDlg::OnEnChangePath()
{
	this->UpdateData () ;
}

void CNagGenDlg::OnEnChangeNagTitle()
{
	this->UpdateData () ;
}

void CNagGenDlg::OnEnChangeNagContent()
{
	this->UpdateData () ;
}

void CNagGenDlg::OnBnClickedBrowse()
{	
	static TCHAR szFilter[] = TEXT ( "PE Files (*.exe)|*.exe||" ) ;
	CFileDialog dlg ( true, 0, 0, 4|2, szFilter ) ;
	if ( dlg.DoModal() == IDOK )
	{
		this->m_PathName = dlg.GetPathName() ;
		this->PEInfo.isReady = TRUE ;
		this->UpdateData ( false ) ;
	}
}

void CNagGenDlg::OnBnClickedOk()
{
	if ( this->PEInfo.isReady == FALSE )
	{
		this->MessageBox ( TEXT("Select a PE File!") ) ;
		return ;
	}

	if ( this->m_NagTitle.IsEmpty() == TRUE )
	{
		this->MessageBox ( TEXT("NAG Title should not be empty!") ) ;
		return ;
	}

	if ( this->m_NagContent.IsEmpty() == TRUE )
	{
		this->MessageBox ( TEXT("NAG Content should not be empty!") ) ;
		return ;
	}

	MAP_ITEM OldMap = {0}, NewMap = {0} ;

	TCHAR	pBuf[MAX_BUF_SIZE] = {0} ;
	BYTE	pData[MAX_BUF_SIZE] ={0} ;
	wsprintf ( pBuf, this->m_PathName.GetBuffer(this->m_PathName.GetLength()) ) ;
	if ( this->PEInfo.CreateMap(pBuf, &OldMap) == FALSE )
	{
		this->MessageBox ( TEXT("Please ensure this file is not been used!") ) ;
		return ;
	}
	
	// 取得PE文件基本信息
	this->PEInfo.GetBaseInfo ( &OldMap ) ;
	// 初始化新区段信息
	this->PEInfo.InitNewSection () ;

	// 产生并组合数据，保存到pData缓冲区，并根据所需要的长度对区段信息进行调整
	DWORD dwNeedSize = this->GenerateData ( pData, OldMap.lpMapAddr );
	if ( this->PEInfo.NewSection.SizeOfRawData < dwNeedSize )
		this->PEInfo.NewSection.SizeOfRawData = dwNeedSize ;
	this->PEInfo.AdjustSectionSize () ;

	// 建立新文件(*_nag.exe)
	CString TempStr = this->m_PathName ;
	DWORD	dwNewFileSize = this->PEInfo.NewSection.PointerToRawData + this->PEInfo.NewSection.SizeOfRawData ;
	TempStr.SetAt ( TempStr.GetLength()-4, 0 ) ;
	wsprintf ( pBuf, TEXT("%s%s"), TempStr.GetBuffer(TempStr.GetLength()), TEXT("_nag.exe") ) ;
	if ( this->PEInfo.CreateNewMap ( pBuf, &NewMap, dwNewFileSize ) == FALSE )
	{
		this->MessageBox ( TEXT("Could't create new file!") ) ;
		DeleteFile ( pBuf ) ;
		return ;
	}

	// 添加区块信息
	this->PEInfo.AddNewSection ( &OldMap, &NewMap, dwNewFileSize ) ;

	// 删除原PE文件的映射
	this->PEInfo.DeleteMap ( &OldMap ) ;

	// 把pData中的数据写入到新区块（ contain IID, NewDllName, NewFunName, NewIat）
	LPVOID	lpNewSection = LPVOID(DWORD(NewMap.lpMapAddr)+this->PEInfo.NewSection.PointerToRawData) ;
	memcpy ( lpNewSection, pData, this->PEInfo.NewSection.SizeOfRawData ) ;

	// 取得PE头信息
	PIMAGE_DOS_HEADER	pDosHeader	= (PIMAGE_DOS_HEADER)(NewMap.lpMapAddr) ;
	PIMAGE_NT_HEADERS	pNtHeader	= (PIMAGE_NT_HEADERS)((LONG)(NewMap.lpMapAddr)+ (pDosHeader->e_lfanew)) ;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNtHeader->OptionalHeader) ;

	// 写入新IID信息
	// 如果OriginalFirstThunk 字段的值为0，加载就不会改写IAT，也就是说，需要自己向IAT写入对应函数的地址
	// 这里把OriginalFirstThunk的值设置为与FirstThunk相同，这样初始化的时候加载器就会改写IAT
	PIMAGE_IMPORT_DESCRIPTOR pIID = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)lpNewSection + 0x14 * this->dwIIDNum) ;
	pIID->Name			= this->dwDllNameOff + this->PEInfo.NewSection.VirtualAddress ;
	pIID->FirstThunk	= this->dwIATOff + this->PEInfo.NewSection.VirtualAddress ;
	pIID->OriginalFirstThunk	= pIID->FirstThunk ;
	pIID->ForwarderChain	= 0xFFFFFFFF ;
	pIID->TimeDateStamp		= 0xFFFFFFFF ;
	*((DWORD*)(this->dwIATOff + (DWORD)lpNewSection)) = this->dwFunNameOff + this->PEInfo.NewSection.VirtualAddress ;
	
	
	// 修改数据目录项中输入表项信息
	pOptionalHeader->DataDirectory[1].VirtualAddress	= this->PEInfo.NewSection.VirtualAddress ;
	pOptionalHeader->DataDirectory[1].Size			   += 0x14 ;

	// 清楚绑定输入表
	pOptionalHeader->DataDirectory[11].VirtualAddress	= 0 ;
	pOptionalHeader->DataDirectory[11].Size				= 0 ;
	
	// 修改OEP
	pOptionalHeader->AddressOfEntryPoint = this->dwNewEntryOff + this->PEInfo.NewSection.VirtualAddress ;
	
	this->PEInfo.FlushData ( LPVOID(this->PEInfo.NewSection.VirtualAddress), this->PEInfo.NewSection.SizeOfRawData ) ;
	this->PEInfo.DeleteMap ( &NewMap ) ;

	this->MessageBox ( TEXT("添加Nag窗口成功!") ) ;
}

DWORD CNagGenDlg::GenerateData(PBYTE pData, LPVOID lpImageBase)
{
	// 取得PE文件基本信息
	PIMAGE_DOS_HEADER	pDosHeader	= (PIMAGE_DOS_HEADER)(lpImageBase) ;
	PIMAGE_NT_HEADERS	pNtHeader	= (PIMAGE_NT_HEADERS)((LONG)(lpImageBase)+ (pDosHeader->e_lfanew)) ;
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNtHeader->OptionalHeader) ;
	PIMAGE_DATA_DIRECTORY pImportData	= (PIMAGE_DATA_DIRECTORY)(&( pOptionalHeader->DataDirectory[1]) ) ;
	PIMAGE_IMPORT_DESCRIPTOR	pIID = (PIMAGE_IMPORT_DESCRIPTOR) ImageRvaToVa ( pNtHeader, lpImageBase, pImportData->VirtualAddress, NULL ) ;

	// 把原IID表拷贝到缓冲区
	memcpy ( pData, LPVOID(pIID), pOptionalHeader->DataDirectory[1].Size ) ;

	// 统计IID数量
	this->dwIIDNum		= 0 ;
	while ( pIID[this->dwIIDNum].FirstThunk )
		this->dwIIDNum ++ ;

	char	szDllName[] = "USER32.dll" ;
	char	szFunName[] = "MessageBoxA" ;

	// 设置偏移信息
	this->dwDllNameOff		= pOptionalHeader->DataDirectory[1].Size + 0x14 ;
	this->dwFunNameOff		= this->dwDllNameOff + sizeof ( szDllName ) + 3 ;
	this->dwIATOff			= this->dwFunNameOff + sizeof ( szFunName ) + 4 ;
	this->dwNagTitleOff		= this->dwIATOff + 12 ;

	// 写入DLL名
	memcpy ( &(pData[this->dwDllNameOff]), szDllName, sizeof(szDllName) ) ;
	// 写入函数名
	memcpy ( &(pData[this->dwFunNameOff+2]), szFunName, sizeof(szFunName) ) ; // 2, 序号


	CHAR pTitle[128] = {0}, pContent[512] = {0} ;
	::GetDlgItemTextA ( this->GetSafeHwnd(), IDC_NAG_TITLE, pTitle, 128 ) ;
	::GetDlgItemTextA ( this->GetSafeHwnd(), IDC_NAG_CONTENT, pContent, 512 ) ;

	// 设置偏移信息（NagTitle + NagContent）
	this->dwNagContentOff	= this->dwNagTitleOff + strlen(pTitle) + 1 ;
	this->dwNewEntryOff		= this->dwNagContentOff + strlen(pContent) + 1 ;

	// 把NagTitle和NagContent信息写入缓冲区
	wsprintf ( PTCHAR(&pData[this->dwNagTitleOff]), TEXT("%s"), pTitle ) ;
	wsprintf ( PTCHAR(&pData[this->dwNagContentOff]), TEXT("%s"), pContent ) ;


	//////////////////////////////////////////////////////////////////////////////////////////
	// Stub Code ( 0x11111111为数据占位符，需修正）
	// pushad
	// push 0
	// push 0x11111111
	// push 0x11111111
	// push 0
	// call dword ptr MessageBoxW
	// popad
	// jmp  0x11111111
	/////////////////////////////////////////////////////////////////////////////////////////
	BYTE pbCommand[] = { 0x60, 0x6A, 0x00, 0x68, 0x11, 0x11, 0x11, 0x11, \
			0x68, 0x11, 0x11, 0x11, 0x11, 0x6A, 0x00, 0xFF, 0x15, 0xBC, 0xF2, 0x42, 0x00, 0x61, \
			0xE9, 0x11, 0x11, 0x11, 0x11 } ;
	
	// 统计Stub Code长度，然后写入pData缓冲区
	DWORD	dwStubLength = sizeof(pbCommand);
	memcpy ( &(pData[this->dwNewEntryOff]), pbCommand, dwStubLength ) ;

	// 计算 JMP 指令的操作数
	DWORD dwOldEntryRVA = pOptionalHeader->AddressOfEntryPoint ;
	DWORD dwNewEntryRVA = this->PEInfo.NewSection.VirtualAddress + this->dwNewEntryOff + 22 ;
	DWORD dwJmpDist		= dwOldEntryRVA - dwNewEntryRVA - 5 ;

	// 修正TitleVA
	*((DWORD*)(&(pData[this->dwNewEntryOff+4])))	= pOptionalHeader->ImageBase + this->PEInfo.NewSection.VirtualAddress + this->dwNagTitleOff ;
	// 修正ContentVA
	*((DWORD*)(&(pData[this->dwNewEntryOff+9])))	= pOptionalHeader->ImageBase + this->PEInfo.NewSection.VirtualAddress + this->dwNagContentOff ;
	// 修正 CALL [0x********]
	*((DWORD*)(&(pData[this->dwNewEntryOff+17])))	= pOptionalHeader->ImageBase + this->PEInfo.NewSection.VirtualAddress + this->dwIATOff ;
	// 修正 JMP 的操作数 ==> E9 ** ** ** **
	*((DWORD*)(&(pData[this->dwNewEntryOff+23])))	= dwJmpDist ;

	// 返回总长度
	return this->dwNewEntryOff + dwStubLength + 0x10 ;
}
