// ComboBoxDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CComboBoxDlg 对话框
class CComboBoxDlg : public CDialog
{
// 构造
public:
	CComboBoxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COMBOBOX_DIALOG };

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
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnCbnSelchangeCombo2();

public:
	CString		szFruitName;			// 新的水果名称
	CString		szCurName,szCurName2;	// 当前选择的水果名称
	CComboBox	MyComboBox,MyComboBox2;	// 定义组合框对象
};
