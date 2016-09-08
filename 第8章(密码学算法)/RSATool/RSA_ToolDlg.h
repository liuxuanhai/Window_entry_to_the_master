// RSA_ToolDlg.h : header file
//

#if !defined(AFX_RSA_TOOLDLG_H__54DC139F_0DF8_400B_B25B_8246C1291545__INCLUDED_)
#define AFX_RSA_TOOLDLG_H__54DC139F_0DF8_400B_B25B_8246C1291545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rsa.h"

/////////////////////////////////////////////////////////////////////////////
// CRSA_ToolDlg dialog

class CRSA_ToolDlg : public CDialog
{
// Construction
public:
	CRSA_ToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRSA_ToolDlg)
	enum { IDD = IDD_RSA_TOOL_DIALOG };
	CComboBox	m_KeySizeCon;
	CString	m_FirstPrime;
	CString	m_SecondPrime;
	CString	m_PrivateKeyD;
	CString	m_PublicKeyN;
	CString	m_EncryptedMsg;
	CString	m_DecryptedMsg;
	CString	m_OriginalMsg;
	CString	m_PublicKeyE;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSA_ToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRSA_ToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGenerateTotal();
	afx_msg void OnEditchangeKeysize();
	afx_msg void OnChangeOrginalMessage();
	afx_msg void OnChangeFirstPrime();
	afx_msg void OnChangeSecondPrime();
	afx_msg void OnChangePublicKeyE() ;
	afx_msg void OnGenerateN();
	afx_msg void OnGenerateD();
	afx_msg void OnDoDecrypt();
	afx_msg void OnDoEncrypt();
	afx_msg void OnAboutDlg() ;
	afx_msg void OnRandGenerateMessage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RSA_PCONTEXT RSA_PContext ;
	HWND hFirstPrime, hSecondPrime, hPublicE ;
	UINT uMsgLenInGroup ;

public:
	BOOL	IsHexString ( CString ) ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSA_TOOLDLG_H__54DC139F_0DF8_400B_B25B_8246C1291545__INCLUDED_)
