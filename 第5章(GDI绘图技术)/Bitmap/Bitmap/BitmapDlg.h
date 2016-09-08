// BitmapDlg.h : 头文件
//

#pragma once

typedef struct _MY_BITMAP { 
	CRect	rect ;
	CBitmap bmp ;
} MY_BITMAP ;

// CBitmapDlg 对话框
class CBitmapDlg : public CDialog
{
// 构造
public:
	CBitmapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BITMAP_DIALOG };

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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	MY_BITMAP MyBmp[9] ;
};
