// KeyboardMsgDlg.h : 头文件
//

#pragma once


// CKeyboardMsgDlg 对话框
class CKeyboardMsgDlg : public CDialog
{
// 构造
public:
	CKeyboardMsgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KEYBOARDMSG_DIALOG };

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
	BOOL	bType ;		// 标识响应本进程消息还是全局消息
	CRect	ValidRect ;	// 可移动的有效区域
	CBitmap FaceBmp ;	// 表情位图对象
	CRect	FaceRect ;	// 表情位图的当前区域
	VOID	MoveFace ( UINT nDir ) ;
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedLocal();
	afx_msg void OnBnClickedTotal();
};
