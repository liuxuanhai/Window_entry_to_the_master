// ProgressCtrlDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CProgressCtrlDlg 对话框
class CProgressCtrlDlg : public CDialog
{
// 构造
public:
	CProgressCtrlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PROGRESSCTRL_DIALOG };

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
	afx_msg void OnEnChangeBegin();
public:
	afx_msg void OnEnChangeEnd();
public:
	afx_msg void OnBnClickedShow();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	BOOL	bStatus ;					// 当前状态，是否正在演示
	UINT	nStart;						// 起始值
	UINT	nEnd;						// 终止值
	UINT	nStep;						// 步长
	CString szInfo, szInfo2;			// 当前进度信息
	CProgressCtrl ProgressCtrl;			// 采用定位方式的进度条
	CProgressCtrl ProgressCtrl2;		// 采用逐步方式的进度条
};
