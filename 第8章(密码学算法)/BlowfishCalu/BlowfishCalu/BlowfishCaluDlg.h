// BlowfishCaluDlg.h : 头文件
//

#pragma once

#include "Blowfish.h"


// CBlowfishCaluDlg 对话框
class CBlowfishCaluDlg : public CDialog
{
// 构造
public:
	CBlowfishCaluDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BLOWFISHCALU_DIALOG };

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
	afx_msg void OnEnChangeKey();

private:
	BYTE	szKey[MAX_KEY_SIZE] ;
	BYTE	szMsg[DEF_BUF_SIZE] ;
	BYTE	ECB_szEnMsg[DEF_BUF_SIZE] ;
	BYTE	ECB_szDeMsg[DEF_BUF_SIZE] ;
	BYTE	CBC_szEnMsg[DEF_BUF_SIZE] ;
	BYTE	CBC_szDeMsg[DEF_BUF_SIZE] ;

	CBlowfish	Blowfish ;
	afx_msg void OnBnClickedCoding();
};
