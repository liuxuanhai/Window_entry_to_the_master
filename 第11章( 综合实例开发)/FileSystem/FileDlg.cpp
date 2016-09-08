// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystem.h"
#include "FileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog


CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
	m_Text = _T("");
	m_Title = _T("");
	//}}AFX_DATA_INIT
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
	DDX_Text(pDX, IDC_TEXT, m_Text);
	DDX_Text(pDX, IDC_TITLE, m_Title);
	DDV_MaxChars(pDX, m_Title, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	ON_EN_CHANGE(IDC_TEXT, OnChangeText)
	ON_EN_CHANGE(IDC_TITLE, OnChangeTitle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers

void CFileDlg::OnChangeText() 
{
	this->UpdateData () ;
}

void CFileDlg::OnChangeTitle() 
{
	this->UpdateData () ;
}

void CFileDlg::OnOK() 
{
	if ( m_Title.IsEmpty() )
	{
		this->MessageBox ( "文件标题不能为空!" ) ;
		return ;
	}

	if ( m_Text.IsEmpty() )
	{
		this->MessageBox ( "文件内容不能为空!" ) ;
		return ;
	}
	
	CDialog::OnOK();
}
