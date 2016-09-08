// TestAStarDlg.h : 头文件
//

#pragma once


// CTestAStarDlg 对话框
class CTestAStarDlg : public CDialog
{
// 构造
public:
	CTestAStarDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTASTAR_DIALOG };

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
	afx_msg void OnBnClickedGenMap();
public:
	afx_msg void OnBnClickedSearchPath();
public:
	afx_msg void OnEnChangeSize();
public:
	afx_msg void OnEnChangeRate();

public:
	UINT	nMapSize, nRate ;
	CListBox	OutputList ;
public:
	afx_msg void OnBnClickedButton1();
};
