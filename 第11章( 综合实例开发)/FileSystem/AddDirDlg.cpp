// AddDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystem.h"
#include "AddDirDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddDirDlg dialog


CAddDirDlg::CAddDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddDirDlg)
	m_SubDirName = _T("");
	//}}AFX_DATA_INIT
}


void CAddDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDirDlg)
	DDX_Text(pDX, IDC_SUB_DIR_NAME, m_SubDirName);
	DDV_MaxChars(pDX, m_SubDirName, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDirDlg, CDialog)
	//{{AFX_MSG_MAP(CAddDirDlg)
	ON_EN_CHANGE(IDC_SUB_DIR_NAME, OnChangeSubDirName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddDirDlg message handlers

void CAddDirDlg::OnChangeSubDirName() 
{
	this->UpdateData ( ) ;
}

void CAddDirDlg::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_SubDirName.IsEmpty() )
	{
		this->MessageBox ( "子目录名不能为空!" ) ;
		return ;
	}
	
	CDialog::OnOK();
}
