// MyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DAOSample.h"
#include "MyDlg.h"


// CMyDlg 对话框

IMPLEMENT_DYNAMIC(CMyDlg, CDialog)

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
	, szBookName(_T(""))
	, szAuthor(_T(""))
	, szPublish(_T(""))
	, nPrice(0)
{

}

CMyDlg::~CMyDlg()
{
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOOKNAME, szBookName);
	DDX_Text(pDX, IDC_AUTHOR, szAuthor);
	DDX_Text(pDX, IDC_PUBLISH, szPublish);
	DDX_Text(pDX, IDC_PRICE, nPrice);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyDlg 消息处理程序

void CMyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData () ;
	if ( this->szBookName.IsEmpty() || this->szAuthor.IsEmpty() || this->szPublish.IsEmpty() )
	{
		this->MessageBox ( L"信息不能为空！" ) ;
		return ;
	}

	OnOK();
}

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetWindowTextW ( this->bShowMode ? L"添加书籍信息对话框" : L"修改书籍信息对话框" ) ;
	this->GetDlgItem(IDOK)->SetWindowTextW ( this->bShowMode ? L"添加" : L"修改" ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString& CMyDlg::GetBookName ()
{
	return this->szBookName ;
}

CString& CMyDlg::GetAuthor ()
{
	return this->szAuthor ;
}

CString& CMyDlg::GetPublish () 
{
	return this->szPublish ;
}

UINT CMyDlg::GetPrice ()
{
	return this->nPrice ;
}

VOID CMyDlg::SetBookName ( CString str )
{
	this->szBookName = str ;
}

VOID CMyDlg::SetAuthor ( CString str ) 
{
	this->szAuthor = str ;
}

VOID CMyDlg::SetPublish ( CString str )
{
	this->szPublish = str ;
}

VOID CMyDlg::SetPrice ( UINT nPrice ) 
{
	this->nPrice = nPrice ;
}

VOID CMyDlg::SetMode ( BOOL bType ) 
{
	this->bShowMode = bType ;
}