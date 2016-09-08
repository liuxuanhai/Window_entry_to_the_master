// ModalDlg_win32Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ModalDlg_win32.h"
#include "ModalDlg_win32Dlg.h"

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


// CModalDlg_win32Dlg 对话框




CModalDlg_win32Dlg::CModalDlg_win32Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModalDlg_win32Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModalDlg_win32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CModalDlg_win32Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CREATE_DILAOG, &CModalDlg_win32Dlg::OnBnClickedCreateDilaog)
END_MESSAGE_MAP()


// CModalDlg_win32Dlg 消息处理程序

BOOL CModalDlg_win32Dlg::OnInitDialog()
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

void CModalDlg_win32Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CModalDlg_win32Dlg::OnPaint()
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
HCURSOR CModalDlg_win32Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

INT_PTR CALLBACK DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	// 处理对话框初始化消息
	case WM_INITDIALOG:
		// 此时的lParam就是DialogBoxParam的最后一个参数dwInitParam所设置的值
		return TRUE ;
	// 处理WM_COMMAND消息
	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			// 响应对话框的结束消息
			case IDCANCEL:
				::EndDialog ( hwndDlg, 0 ) ;
				return TRUE ;
			}
		}
		break ;
	}
	// 返回FALSE，表示把消息继续传递给系统默认的消息处理过程
	// 返回TRUE，就表示不需要继续传递消息
	return FALSE ;
}

void CModalDlg_win32Dlg::OnBnClickedCreateDilaog()
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle ( NULL ) ;

	::DialogBox ( hInstance, MAKEINTRESOURCE(IDD_MODAL_DIALOG), this->m_hWnd, DialogProc ) ;

	//::DialogBoxParam ( hInstance, MAKEINTRESOURCE(IDD_MODAL_DIALOG), this->m_hWnd, DialogProc, NULL ) ;
}
