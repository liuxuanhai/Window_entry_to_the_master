#if !defined(AFX_LOGDLG_H__1D18B96F_5829_4507_8E52_A4B62965FF44__INCLUDED_)
#define AFX_LOGDLG_H__1D18B96F_5829_4507_8E52_A4B62965FF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialog
{
// Construction
public:
	CLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_LOGGING };
	CString	m_UserName;
	CString	m_UserPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	afx_msg void OnChangeUserName();
	afx_msg void OnChangeUserPwd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDLG_H__1D18B96F_5829_4507_8E52_A4B62965FF44__INCLUDED_)
