// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystem.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
	m_UserName = _T("");
	m_UserPwd = _T("");
	//}}AFX_DATA_INIT
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Text(pDX, IDC_USER_NAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 12);
	DDX_Text(pDX, IDC_USER_PWD, m_UserPwd);
	DDV_MaxChars(pDX, m_UserPwd, 12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_EN_CHANGE(IDC_USER_NAME, OnChangeUserName)
	ON_EN_CHANGE(IDC_USER_PWD, OnChangeUserPwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

void CLogDlg::OnChangeUserName() 
{
	this->UpdateData () ;
}

void CLogDlg::OnChangeUserPwd() 
{
	this->UpdateData () ;
}

void CLogDlg::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_UserName.IsEmpty() )
	{
		this->MessageBox ( "用户名不能为空!" ) ;
		return ;
	}
	CDialog::OnOK();
}
