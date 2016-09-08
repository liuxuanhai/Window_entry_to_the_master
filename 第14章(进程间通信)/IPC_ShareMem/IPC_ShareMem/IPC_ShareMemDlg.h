// IPC_ShareMemDlg.h : 头文件
//

#pragma once


// CIPC_ShareMemDlg 对话框
class CIPC_ShareMemDlg : public CDialog
{
// 构造
public:
	CIPC_ShareMemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IPC_SHAREMEM_DIALOG };

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

private:
	HANDLE	hMapFile ;
	LPVOID	lpData ;
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedWrite();
	afx_msg void OnEnChangeData();
};
