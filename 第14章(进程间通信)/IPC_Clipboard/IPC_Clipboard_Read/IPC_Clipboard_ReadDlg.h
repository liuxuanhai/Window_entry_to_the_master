// IPC_Clipboard_ReadDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CIPC_Clipboard_ReadDlg 对话框
class CIPC_Clipboard_ReadDlg : public CDialog
{
// 构造
public:
	CIPC_Clipboard_ReadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IPC_CLIPBOARD_READ_DIALOG };

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
	CListBox InfoList;
public:
	afx_msg void OnBnClickedGetText();
public:
	afx_msg void OnBnClickedGetBitmap();
public:
	afx_msg void OnBnClickedGetSelfDefine();
public:
	afx_msg void OnBnClickedGetAll();

public:
	VOID	ReadOutText () ;
	VOID	ReadOutBitmap () ;
	VOID	ReadOutSelfDefineData () ;
};
