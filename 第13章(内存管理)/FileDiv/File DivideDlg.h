// File DivideDlg.h : header file
//

#if !defined(AFX_FILEDIVIDEDLG_H__E79DCBEC_7C6F_4C6D_8204_8F40FCA02073__INCLUDED_)
#define AFX_FILEDIVIDEDLG_H__E79DCBEC_7C6F_4C6D_8204_8F40FCA02073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FD.h"

#define PART_SIZE_NUM 20

/////////////////////////////////////////////////////////////////////////////
// CFileDivideDlg dialog

class CFileDivideDlg : public CDialog
{
// Construction
public:
	CFileDivideDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDivideDlg)
	enum { IDD = IDD_FILEDIVIDE_DIALOG };
	CComboBox	m_ComList;
	CString	m_DivideFileName;
	UINT	m_PartNum;
	CString	m_ConnFileName;
	UINT	m_FileSize;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDivideDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileDivideDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnectFile();
	afx_msg void OnDivideFile();
	afx_msg void OnSelectFileToConnect();
	afx_msg void OnSelectFileToDivide();
	afx_msg void OnSelchangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void CFileDivideDlg::GetErrorMessage ( DWORD dwErrorCode, char pMessage[] ) ;

public:
	CFD		FD ;
	UINT	uPartSizeList[PART_SIZE_NUM] ;

	FILE_ITEM_INFO FileItemDiv, FileItemCon ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIVIDEDLG_H__E79DCBEC_7C6F_4C6D_8204_8F40FCA02073__INCLUDED_)
