// SimKeyMsgDlg.h : 头文件
//

#pragma once


// CSimKeyMsgDlg 对话框
class CSimKeyMsgDlg : public CDialog
{
// 构造
public:
	CSimKeyMsgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMKEYMSG_DIALOG };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedWinR();
	afx_msg void OnBnClickedWinU();
	afx_msg void OnBnClickedCtrlShift();
	afx_msg void OnBnClickedAltTab();
};
