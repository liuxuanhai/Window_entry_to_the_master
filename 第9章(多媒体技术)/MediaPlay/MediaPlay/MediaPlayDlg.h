// MediaPlayDlg.h : 头文件
//

#pragma once
#include "player.h"


// CMediaPlayDlg 对话框
class CMediaPlayDlg : public CDialog
{
// 构造
public:
	CMediaPlayDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MEDIAPLAY_DIALOG };

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
	CPlayer MediaPlayer;
public:
	afx_msg void OnEnChangeUrl();
public:
	afx_msg void OnBnClickedSet();
public:
	CString szMediaUrl;
};
