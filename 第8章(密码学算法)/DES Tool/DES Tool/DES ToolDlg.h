// DES ToolDlg.h : header file
//

#if !defined(AFX_DESTOOLDLG_H__71CCFB76_5360_47E9_A368_B0B87164FD64__INCLUDED_)
#define AFX_DESTOOLDLG_H__71CCFB76_5360_47E9_A368_B0B87164FD64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDESToolDlg dialog

#include "des.h"

class CDESToolDlg : public CDialog
{
// Construction
public:
	CDESToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDESToolDlg)
	enum { IDD = IDD_DESTOOL_DIALOG };
	CString	m_DecryptedMsg;
	CString	m_EncryptedMsg;
	CString	m_OriginalMsg;
	CString	m_PrivateKey;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDESToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDESToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeOridinalMsg();
	afx_msg void OnChangePrivateKey();
	afx_msg void OnBinOri();
	afx_msg void OnBinEn();
	afx_msg void OnBinDe();
	afx_msg void OnBinKey();
	afx_msg void OnShowSubKey();
	afx_msg void OnEncrypted();
	afx_msg void OnDecrypted();
	afx_msg void OnShowTransProc();
	afx_msg void OnHelp();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	UINT	uLastOp ;
	CDES	DESSystem ;
	CString	ConvertStringToBin ( PBYTE pByte, UINT uLen, char ch ) ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESTOOLDLG_H__71CCFB76_5360_47E9_A368_B0B87164FD64__INCLUDED_)
