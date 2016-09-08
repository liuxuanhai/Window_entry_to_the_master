// ·þÎñÆ÷Dlg.h : header file
//

#if !defined(AFX_DLG_H__5948966B_D3B4_4F85_8B97_673DC4B9FDD7__INCLUDED_)
#define AFX_DLG_H__5948966B_D3B4_4F85_8B97_673DC4B9FDD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

#define MAX_CONNECT 128

typedef struct {
	HANDLE	hTread ;
	HANDLE	hPipe ;
	HANDLE	hEvent ;
} PIPE_INSTRUCT;

class CMyDlg : public CDialog
{
// Construction
public:
	CMyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyDlg)
	enum { IDD = IDD_MY_DIALOG };
	int		nResValue;
	int		nFirst;
	int		nSecond;
	UINT	nMaxConn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeMaxconn();
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool	bServerStatus ;
	PIPE_INSTRUCT PipeInst[MAX_CONNECT] ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__5948966B_D3B4_4F85_8B97_673DC4B9FDD7__INCLUDED_)
