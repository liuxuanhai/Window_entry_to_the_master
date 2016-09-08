// FileSystemDlg.h : header file
//

#if !defined(AFX_FILESYSTEMDLG_H__51ACB58A_7E45_4F67_9CF4_D4F5920EE9D8__INCLUDED_)
#define AFX_FILESYSTEMDLG_H__51ACB58A_7E45_4F67_9CF4_D4F5920EE9D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "System.h"
#include "AddDirDlg.h"
#include "LogDlg.h"
#include "FileDlg.h"
#include "AddUserDlg.h"
#include "ShowUserDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFileSystemDlg dialog

class CFileSystemDlg : public CDialog
{
// Construction
public:
	CFileSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFileSystemDlg)
	enum { IDD = IDD_FILESYSTEM_DIALOG };
	CListCtrl	m_FileList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFileSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStall();
	afx_msg void OnLowFormat();
	afx_msg void OnHighFormat();
	afx_msg void OnCreateDir();
	afx_msg void OnDeleteDir();
	afx_msg void OnUpdate();
	afx_msg void OnLog();
	afx_msg void OnDeleteFile();
	afx_msg void OnCreateFile();
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAuthor();
	afx_msg void OnShowUser();
	afx_msg void OnCreateUser();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL		bInstall ;
	BOOL		bLogging ;
	char		szCurPath[512] ;
	char		szCurName[12] ;
	CSystem		SYSTEM ;

	CAddDirDlg		AddDirDlg ;
	CLogDlg			LogDlg ;
	CAddUserDlg		AddUserDlg ;
	CShowUserDlg	ShowUserDlg ;

public:
	void	EnumDirectory ( LPSTR lpPath ) ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESYSTEMDLG_H__51ACB58A_7E45_4F67_9CF4_D4F5920EE9D8__INCLUDED_)
