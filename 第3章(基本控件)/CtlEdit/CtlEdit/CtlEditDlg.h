// CtlEditDlg.h : 头文件
//

#pragma once


// CCtlEditDlg 对话框
class CCtlEditDlg : public CDialog
{
// 构造
public:
	CCtlEditDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CTLEDIT_DIALOG };

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
	CString szMsg1;
	CString szMsg2;
	CString szMsg3;
	CString szMsg4;
	CString szMsg5;
	CString szMsg6;
	CString szMsg7;
	CString szMsg8;
	CString szMsg9;
	CString szMsg10;
	CString szMsg11;
	CString szMsg12;
};
