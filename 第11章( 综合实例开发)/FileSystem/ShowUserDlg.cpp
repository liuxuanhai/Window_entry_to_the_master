// ShowUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSystem.h"
#include "ShowUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowUserDlg dialog


CShowUserDlg::CShowUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowUserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowUserDlg)
	DDX_Control(pDX, IDC_USER_LIST, m_UserList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowUserDlg, CDialog)
	//{{AFX_MSG_MAP(CShowUserDlg)
	ON_BN_CLICKED(IDC_DELETE_USER, OnDeleteUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowUserDlg message handlers
void CShowUserDlg::OnDeleteUser() 
{
	szUserToDel = "" ;

	int nCurIndex = m_UserList.GetNextItem ( -1, LVNI_SELECTED ) ;
	if ( nCurIndex < 0 )
	{
		this->MessageBox ( "请选择一个用户!" ) ;
		return ;
	}
	else
	{
		szUserToDel = m_UserList.GetItemText ( nCurIndex, 0 ) ;
		if ( !SYSTEM.SysDeleteUser ( szUserToDel.GetBuffer(MAX_USER_NAME_LEN+1) ) )
		{
			this->MessageBox ( "删除用户失败!" ) ;
		}
		else
		{
			m_UserList.DeleteItem ( nCurIndex ) ;
		}

	}
}

void CShowUserDlg::OnCancel() 
{
	szUserToDel = "" ;
	CDialog::OnCancel();
}

BOOL CShowUserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_UserList.InsertColumn ( 0, " 用 户 名",	LVCFMT_LEFT, 80 ) ;
	this->m_UserList.InsertColumn ( 1, " 密    码",	LVCFMT_LEFT, 85 ) ;
	this->m_UserList.SetExtendedStyle ( this->m_UserList.GetExtendedStyle()|LVS_EX_FULLROWSELECT );

	UINT nCount = 0 ;
	USER_INFO Item ;
	UINT nCurAddr = SYSTEM.SysGetUserInfo ( &Item, DIRECTORY_ITEM_SIZE ) ;
	while ( nCurAddr != 0 )
	{
		char szName[13] = {0}, szPwd[13] = {0} ;
		memcpy ( szName, Item.szUserName, MAX_USER_NAME_LEN ) ;
		memcpy ( szPwd, Item.szUserPwd, MAX_USER_PWD_LEN ) ;
		
		this->m_UserList.InsertItem	( nCount, szName ) ;
		this->m_UserList.SetItemText( nCount++, 1, szPwd ) ;

		nCurAddr = SYSTEM.SysGetUserInfo ( &Item, nCurAddr ) ;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
