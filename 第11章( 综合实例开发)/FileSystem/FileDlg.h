#if !defined(AFX_FILEDLG_H__83482571_135B_4F88_AFED_E3C6200A90C7__INCLUDED_)
#define AFX_FILEDLG_H__83482571_135B_4F88_AFED_E3C6200A90C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog

class CFileDlg : public CDialog
{
// Construction
public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDlg)
	enum { IDD = IDD_FILE };
	CString	m_Text;
	CString	m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileDlg)
	afx_msg void OnChangeText();
	afx_msg void OnChangeTitle();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDLG_H__83482571_135B_4F88_AFED_E3C6200A90C7__INCLUDED_)
