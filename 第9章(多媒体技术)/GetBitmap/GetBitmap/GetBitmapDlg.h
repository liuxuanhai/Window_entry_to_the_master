// GetBitmapDlg.h : 头文件
//

#pragma once


// CGetBitmapDlg 对话框
class CGetBitmapDlg : public CDialog
{
// 构造
public:
	CGetBitmapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GETBITMAP_DIALOG };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);

private:
	POINT	StartPt ;
	POINT	StopPt ;
	RECT	SelRect ;
	BOOL	bStatus ;
	CDC		ScreenDc ;
	CBitmap	ScreenBmp ;
	CDC		CurMemDc ;
	CBitmap	SelBmp ;
	CBrush	RectBrush ;
	VOID	GetScreenBmp () ;
	BOOL	ConvertTwoPtToRect () ;
	VOID	UpdateView () ;
	VOID	GetSelRectBitmap () ;
	BOOL	SaveSelBmpToClipboard () ;
	BOOL	SaveSelBmpToFile () ;
};
