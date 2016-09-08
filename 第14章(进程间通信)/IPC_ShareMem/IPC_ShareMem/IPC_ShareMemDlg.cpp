// IPC_ShareMemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IPC_ShareMem.h"
#include "IPC_ShareMemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEF_DATA_SIZE		4096
#define SHARE_MEM_NOTIFY	WM_USER+1


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


// CIPC_ShareMemDlg 对话框




CIPC_ShareMemDlg::CIPC_ShareMemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPC_ShareMemDlg::IDD, pParent)
	, szData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPC_ShareMemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATA, szData);
}

BEGIN_MESSAGE_MAP(CIPC_ShareMemDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CREATE, &CIPC_ShareMemDlg::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_CLOSE, &CIPC_ShareMemDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_WRITE, &CIPC_ShareMemDlg::OnBnClickedWrite)
	ON_EN_CHANGE(IDC_DATA, &CIPC_ShareMemDlg::OnEnChangeData)
END_MESSAGE_MAP()


// CIPC_ShareMemDlg 消息处理程序

BOOL CIPC_ShareMemDlg::OnInitDialog()
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
	this->hMapFile = NULL ;
	this->lpData = NULL ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIPC_ShareMemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPC_ShareMemDlg::OnPaint()
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
HCURSOR CIPC_ShareMemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIPC_ShareMemDlg::OnBnClickedCreate()
{	
	if ( this->lpData )
	{
		this->MessageBox ( L"共享内存已经存在！" ) ;
		return ;
	}

	// 创建共享内存
	this->hMapFile = CreateFileMapping ( INVALID_HANDLE_VALUE, \
		NULL,PAGE_READWRITE | SEC_COMMIT,0,DEF_DATA_SIZE,L"ShareMemSample" ) ;
	
	// 映射共享内存
	if ( hMapFile )
		this->lpData = MapViewOfFile ( hMapFile, FILE_MAP_READ|FILE_MAP_WRITE,0,0,0 ) ;

	if ( this->hMapFile == NULL || this->lpData == NULL )
		this->MessageBox ( L"创建共享内存映射文件失败!" ) ;
}

void CIPC_ShareMemDlg::OnBnClickedClose()
{
	// 撤消映射的共享内存
	if ( this->lpData )
		UnmapViewOfFile ( this->lpData ) ;

	// 关闭共享内存
	if ( this->hMapFile )
		CloseHandle ( this->hMapFile ) ;
}

void CIPC_ShareMemDlg::OnBnClickedWrite()
{
	if ( this->lpData == NULL )
	{
		this->MessageBox ( L"写入失败！" ) ;
		return ;
	}

	UINT	nLen = this->szData.GetLength()*2+2 ;
	LPWSTR	lpBuf = this->szData.GetBuffer( nLen ) ;

	// 写入共享内存
	memcpy ( this->lpData, (char*)lpBuf, nLen ) ;

	// 检测是否需要通知到客户端
	if ( ((CButton*)this->GetDlgItem(IDC_AUTO_NOTIFY))->GetCheck() == BST_CHECKED )
	{
		HWND hWnd = ::FindWindow ( NULL, L"[第14章]进程间通信之共享内存——读取端" ) ;
		if ( hWnd )
		{
			// 使用自定义消息
			::PostMessage ( hWnd, SHARE_MEM_NOTIFY, 0, 0 ) ;
		}
	}
}

void CIPC_ShareMemDlg::OnEnChangeData()
{
	this->UpdateData () ;
}
