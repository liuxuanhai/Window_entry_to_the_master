// JoyTesterDlg.h : header file
//

#if !defined(AFX_JOYTESTERDLG_H__830DE759_7059_4DB0_8115_B24D533B5F5C__INCLUDED_)
#define AFX_JOYTESTERDLG_H__830DE759_7059_4DB0_8115_B24D533B5F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CJoyTesterDlg dialog

#include "Joystick.h"

#define MAX_BUTTON_NUM	16

typedef struct _BUTTON_ITEM {
	RECT	rect ;
	BOOL	isPressed ;
} BUTTON_ITEM, *PBUTTON_ITEM ;

class CJoyTesterDlg : public CDialog
{
// Construction
public:
	CJoyTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJoyTesterDlg)
	enum { IDD = IDD_JOYTESTER_DIALOG };
	int		pov0;
	int		pov1;
	int		pov2;
	int		rx;
	int		rz;
	int		pov3;
	int		ry;
	int		s1;
	int		s0;
	int		x;
	int		y;
	int		z;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyTesterDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJoyTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CJoystick	js ;
	BUTTON_ITEM	ButtonItem[MAX_BUTTON_NUM] ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYTESTERDLG_H__830DE759_7059_4DB0_8115_B24D533B5F5C__INCLUDED_)
