// SimMouseMsgDlg.h : 头文件
//

#pragma once


// CSimMouseMsgDlg 对话框
class CSimMouseMsgDlg : public CDialog
{
// 构造
public:
	CSimMouseMsgDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMMOUSEMSG_DIALOG };

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
	afx_msg void OnBnClickedLeftDown();
public:
	afx_msg void OnBnClickedRightDown();
public:
	afx_msg void OnBnClickedMouseMove();

private:
	CRect	PaintRect ;	// 定义绘图区域
	CPen	FramePen ;	// 定义矩形框的边线使用的画笔
	UINT	nType ;		// 定义模拟的消息类型（1：左键单击，2：右键单击，3：移动鼠标）
	CBrush	RedBrush ;	// 红色画刷
	CBrush	GreenBrush ;	// 绿色画刷
	CBrush	BlueBrush ;	// 蓝色画刷

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
