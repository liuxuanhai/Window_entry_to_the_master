#if !defined(AFX_ADDDIRDLG_H__7410D4B3_96A1_4226_983E_AA9CB9008A6A__INCLUDED_)
#define AFX_ADDDIRDLG_H__7410D4B3_96A1_4226_983E_AA9CB9008A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddDirDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddDirDlg dialog

class CAddDirDlg : public CDialog
{
// Construction
public:
	CAddDirDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddDirDlg)
	enum { IDD = IDD_ADD_DIR };
	CString	m_SubDirName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddDirDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddDirDlg)
	afx_msg void OnChangeSubDirName();
	virtual void OnOK();
	afx_msg void OnLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDDIRDLG_H__7410D4B3_96A1_4226_983E_AA9CB9008A6A__INCLUDED_)
