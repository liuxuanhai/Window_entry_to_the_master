// NagGenDlg.h : 头文件
//

#pragma once

#include "PEInfo.h"

#include "Dbghelp.h"
#pragma comment ( lib, "Dbghelp.lib" )

// CNagGenDlg 对话框
class CNagGenDlg : public CDialog
{
// 构造
public:
	CNagGenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NAGGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CPEInfo	PEInfo ;
	DWORD	dwDllNameOff, dwFunNameOff, dwIATOff, dwIIDNum, dwNewEntryOff, dwNagTitleOff, dwNagContentOff ;
public:
	CString m_PathName;
public:
	CString m_NagTitle;
public:
	CString m_NagContent;
public:
	afx_msg void OnEnChangePath();
public:
	afx_msg void OnEnChangeNagTitle();
public:
	afx_msg void OnEnChangeNagContent();
public:
	afx_msg void OnBnClickedBrowse();
public:
	afx_msg void OnBnClickedOk();
public:
	DWORD GenerateData(PBYTE pData, LPVOID lpImageBase);
};
