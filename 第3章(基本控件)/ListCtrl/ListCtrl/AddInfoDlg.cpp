// AddInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrl.h"
#include "AddInfoDlg.h"


// CAddInfoDlg 对话框

IMPLEMENT_DYNAMIC(CAddInfoDlg, CDialog)

CAddInfoDlg::CAddInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddInfoDlg::IDD, pParent)
	, szName(_T(""))
	, szId(_T(""))
	, szGender(_T(""))
	, szAddr(_T(""))
{

}

CAddInfoDlg::~CAddInfoDlg()
{
}

void CAddInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, szName);
	DDX_Text(pDX, IDC_ID, szId);
	DDX_Text(pDX, IDC_GENDER, szGender);
	DDX_Text(pDX, IDC_ADDR, szAddr);
}


BEGIN_MESSAGE_MAP(CAddInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CAddInfoDlg::OnBnClickedAdd)
	ON_EN_CHANGE(IDC_NAME, &CAddInfoDlg::OnEnChangeName)
	ON_EN_CHANGE(IDC_ID, &CAddInfoDlg::OnEnChangeId)
	ON_EN_CHANGE(IDC_GENDER, &CAddInfoDlg::OnEnChangeGender)
	ON_EN_CHANGE(IDC_ADDR, &CAddInfoDlg::OnEnChangeAddr)
END_MESSAGE_MAP()


// CAddInfoDlg 消息处理程序

void CAddInfoDlg::OnBnClickedAdd()
{
	if ( this->szName.IsEmpty() || this->szId.IsEmpty() || \
		this->szGender.IsEmpty () || this->szAddr.IsEmpty () )
	{
		this->MessageBox ( L"请填写完整信息", L"Warning" ) ;
		return ;
	}

	CDialog::OnOK () ;
}

void CAddInfoDlg::OnEnChangeName()
{
	this->UpdateData () ;
}

void CAddInfoDlg::OnEnChangeId()
{
	this->UpdateData () ;
}

void CAddInfoDlg::OnEnChangeGender()
{
	this->UpdateData () ;
}

void CAddInfoDlg::OnEnChangeAddr()
{
	this->UpdateData () ;
}

CString CAddInfoDlg::NEW_GetName()
{
	return this->szName ;
}

CString CAddInfoDlg::NEW_GetId ()
{
	return this->szId ;
}

CString CAddInfoDlg::NEW_GetGender () 
{
	return this->szGender ;
}

CString CAddInfoDlg::NEW_GetAddr ()
{
	return this->szAddr ;
}
