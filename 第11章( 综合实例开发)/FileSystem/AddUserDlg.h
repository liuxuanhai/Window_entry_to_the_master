#if !defined(AFX_ADDUSERDLG_H__E298BC01_5EF1_4CC5_8E16_681B03F497DC__INCLUDED_)
#define AFX_ADDUSERDLG_H__E298BC01_5EF1_4CC5_8E16_681B03F497DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg dialog

class CAddUserDlg : public CDialog
{
// Construction
public:
	CAddUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddUserDlg)
	enum { IDD = IDD_ADD_USER };
	CString	m_UserName;
	CString	m_UserPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddUserDlg)
	afx_msg void OnChangeUserName();
	afx_msg void OnChangeUserPwd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUSERDLG_H__E298BC01_5EF1_4CC5_8E16_681B03F497DC__INCLUDED_)
