#pragma once
#include "afxwin.h"


// CSelProcess 对话框

typedef struct _MY_PROCESS_INFO {
	DWORD	dwProcessId ;
	CString	szFilePath ;
} MY_PROCESS_INFO, *PMY_PROCESS_INFO ;

class CSelProcess : public CDialog
{
	DECLARE_DYNAMIC(CSelProcess)

public:
	CSelProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelProcess();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	int		nCurSel ;
	vector<MY_PROCESS_INFO> ProcList ;
public:
	DWORD	GetSelProcessId () ;
	CString	GetSelProcessName () ;



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelect();
public:
	afx_msg void OnBnClickedFlush();
public:
	CListBox ProcInfoList;
public:
	virtual BOOL OnInitDialog();
};
