// VirtualMemoryDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "VMInfo.h"
#include "afxwin.h"


// CVirtualMemoryDlg 对话框
class CVirtualMemoryDlg : public CDialog
{
// 构造
public:
	CVirtualMemoryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VIRTUALMEMORY_DIALOG };

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
private:
	DWORD		dwSelProcessId ;
	CListCtrl	VMList;
	CVMInfo		VMInfo ;
	CString		szProcess;

private:
	afx_msg void OnBnClickedFlush();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSelect();
	CButton CommitCheck;
	CButton ReserveCheck;
	CButton FreeCheck;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
};
