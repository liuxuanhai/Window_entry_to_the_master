// ColorDlgDlg.h : 头文件
//

#pragma once


// CColorDlgDlg 对话框
class CColorDlgDlg : public CDialog
{
// 构造
public:
	CColorDlgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLORDLG_DIALOG };

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
	BYTE	r, g, b;		// 用于保存所设置的颜色值
	CRect	ColorShowRect ;	// 定义绘制指定颜色的区域
public:
	afx_msg void OnBnClickedSetColor();
	afx_msg void OnEnChangeR();
	afx_msg void OnEnChangeG();
	afx_msg void OnEnChangeB();
};
