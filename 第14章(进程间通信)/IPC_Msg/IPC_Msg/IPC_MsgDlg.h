// IPC_MsgDlg.h : 头文件
//

#pragma once


// CIPC_MsgDlg 对话框
class CIPC_MsgDlg : public CDialog
{
// 构造
public:
	CIPC_MsgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IPC_MSG_DIALOG };

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
	CString szData;
public:
	afx_msg void OnEnChangeData();
public:
	afx_msg void OnBnClickedSend();
};
