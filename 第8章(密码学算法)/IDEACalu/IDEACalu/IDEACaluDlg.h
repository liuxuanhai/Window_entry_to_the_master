// IDEACaluDlg.h : 头文件
//

#pragma once

#include "IDEA.h"


#define DEF_BUF_SIZE 1024

// CIDEACaluDlg 对话框
class CIDEACaluDlg : public CDialog
{
// 构造
public:
	CIDEACaluDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IDEACALU_DIALOG };

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
	CIDEA idea ;
	afx_msg void OnBnClickedEnDe();
	afx_msg void OnBnClickedSetKey();

private:
	UINT nEnDataSize ;
	BYTE bMsg[DEF_BUF_SIZE] ;
	BYTE bEnMsg[DEF_BUF_SIZE] ;
	BYTE bDeMsg[DEF_BUF_SIZE] ;
};
