// ButtonDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CButtonDlg 对话框
class CButtonDlg : public CDialog
{
// 构造
public:
	CButtonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BUTTON_DIALOG };

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
	CButton game1;
	CButton game2;
	CButton game3;
	CButton game4;
	CButton game5;
	CButton game6;
public:
	CButton addr1;
	CButton addr2;
	CButton addr3;
	CButton addr4;
public:
	afx_msg void OnBnClickedSure();
};
