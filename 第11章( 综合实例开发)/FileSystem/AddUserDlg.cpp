// AddUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystem.h"
#include "AddUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg dialog


CAddUserDlg::CAddUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddUserDlg)
	m_UserName = _T("");
	m_UserPwd = _T("");
	//}}AFX_DATA_INIT
}


void CAddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddUserDlg)
	DDX_Text(pDX, IDC_USER_NAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 12);
	DDX_Text(pDX, IDC_USER_PWD, m_UserPwd);
	DDV_MaxChars(pDX, m_UserPwd, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddUserDlg, CDialog)
	//{{AFX_MSG_MAP(CAddUserDlg)
	ON_EN_CHANGE(IDC_USER_NAME, OnChangeUserName)
	ON_EN_CHANGE(IDC_USER_PWD, OnChangeUserPwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg message handlers

void CAddUserDlg::OnChangeUserName() 
{
	this->UpdateData () ;
}

void CAddUserDlg::OnChangeUserPwd() 
{
	this->UpdateData () ;
}

void CAddUserDlg::OnOK() 
{
	if ( this->m_UserName.IsEmpty() )
	{
		this->MessageBox ( "用户名不能为空!" ) ;
		return ;
	}

	if ( this->m_UserPwd.IsEmpty() )
	{
		this->MessageBox ( "密码不能为空!" ) ;
		return ;
	}
	
	CDialog::OnOK();
}
