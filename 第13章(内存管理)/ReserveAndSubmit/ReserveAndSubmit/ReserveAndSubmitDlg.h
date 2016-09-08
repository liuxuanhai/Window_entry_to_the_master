// ReserveAndSubmitDlg.h : 头文件
//

#pragma once

#include "Region.h"
#include "afxwin.h"


// CReserveAndSubmitDlg 对话框
class CReserveAndSubmitDlg : public CDialog
{
// 构造
public:
	CReserveAndSubmitDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESERVEANDSUBMIT_DIALOG };

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
	afx_msg void OnBnClickedAlloc();
public:
	afx_msg void OnBnClickedRelease();

private:
	LPVOID	lpData ;
	CRegion	PaintRegion ;
	CBrush	WhiteBrush ;
	CBrush	GreyBrush ;
	CBrush	BlackBrush ;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CString szBaseAddr;
	CString szCurAddr;
};
