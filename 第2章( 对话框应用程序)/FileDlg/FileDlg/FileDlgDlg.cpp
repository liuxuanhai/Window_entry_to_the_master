// FileDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileDlg.h"
#include "FileDlgDlg.h"

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


// CFileDlgDlg 对话框




CFileDlgDlg::CFileDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlgDlg::IDD, pParent)
	, szSigOpenPath(_T(""))
	, szSigSavePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Path1, szSigOpenPath);
	DDX_Text(pDX, IDC_PATH2, szSigSavePath);
	DDX_Control(pDX, IDC_PATH_LIST, PathList);
}

BEGIN_MESSAGE_MAP(CFileDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Select1, &CFileDlgDlg::OnBnClickedSelect1)
	ON_BN_CLICKED(IDC_SELECT2, &CFileDlgDlg::OnBnClickedSelect2)
	ON_BN_CLICKED(IDC_SELECT3, &CFileDlgDlg::OnBnClickedSelect3)
END_MESSAGE_MAP()


// CFileDlgDlg 消息处理程序

BOOL CFileDlgDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileDlgDlg::OnPaint()
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
HCURSOR CFileDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
} 
 

void CFileDlgDlg::OnBnClickedSelect1()
{
	// 定义“打开”对话框
	CFileDialog dlg ( TRUE ) ;
	if ( dlg.DoModal() == IDOK )
	{
		// 取得文件路径全名
		// szSigOpenPath是CString类型，跟第一个编辑框相关联
		this->szSigOpenPath = dlg.GetPathName() ;
		// 更新界面显示
		this->UpdateData( FALSE ) ;
	}	
}

void CFileDlgDlg::OnBnClickedSelect2()
{
	// 定义“另存为”对话框
	CFileDialog dlg ( FALSE ) ;
	if ( dlg.DoModal() == IDOK )
	{
		// 取得文件路径全名
		// szSigSavePath是CString类型，跟第一个编辑框相关联
		this->szSigSavePath = dlg.GetPathName() ;
		// 更新界面显示
		this->UpdateData( FALSE ) ;
	}	
}

void CFileDlgDlg::OnBnClickedSelect3()
{
	// 设置过滤器，只能选择*.exe或者*.txt类型的文件
	static WCHAR szFilter[] = L"可执行文件(*.exe)|*.exe|文本文件(*.txt)|*.txt||" ;
	// 设置OFN_ALLOWMULTISELECT标记，支持多选
	CFileDialog dlg ( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, szFilter ) ;
	if ( dlg.DoModal() == IDOK )
	{
		// 清空列表框
		this->PathList.ResetContent() ;
		// 取得所选择的第一个文件定位信息。
		POSITION pos = dlg.GetStartPosition() ;
		while ( pos )
			// 取得路径全名并添加到列表框
			this->PathList.AddString ( dlg.GetNextPathName(pos) ) ;
	}
}
