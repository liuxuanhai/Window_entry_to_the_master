#if !defined(AFX_SHOWUSERDLG_H__D8116D8F_4382_47A5_BB80_3942C1A6441B__INCLUDED_)
#define AFX_SHOWUSERDLG_H__D8116D8F_4382_47A5_BB80_3942C1A6441B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowUserDlg.h : header file
//

#include "system.h"

/////////////////////////////////////////////////////////////////////////////
// CShowUserDlg dialog

class CShowUserDlg : public CDialog
{
// Construction
public:
	CShowUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowUserDlg)
	enum { IDD = IDD_SHOW_USER };
	CListCtrl	m_UserList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowUserDlg)
	afx_msg void OnDeleteUser();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString szUserToDel ;
	CSystem	SYSTEM ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWUSERDLG_H__D8116D8F_4382_47A5_BB80_3942C1A6441B__INCLUDED_)
