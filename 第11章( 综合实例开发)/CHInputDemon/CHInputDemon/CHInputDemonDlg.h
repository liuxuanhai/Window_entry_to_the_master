// CHInputDemonDlg.h : 头文件
//

#pragma once

#include "PYTrie.h"
#include "LXWord.h"


// CCHInputDemonDlg 对话框
class CCHInputDemonDlg : public CDialog
{
// 构造
public:
	CCHInputDemonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHINPUTDEMON_DIALOG };

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
	CString szString;
public:
	afx_msg void OnBnClickedEmpty();
public:
	CString szPY;
public:
	UINT nTotalWord;
public:
	UINT nTotalPage;
public:
	UINT nCurPage;
public:
	afx_msg void OnEnChangePinyin();


private:
	CPYTrie PYTrie ;
	CLXWord LXWord ;
	CString szSearchWord;
	VOID	OnNum ( UINT nIndex ) ;
	VOID	OnPrevPage () ;
	VOID	OnNextPage () ;
	VOID	UpdateWord () ;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
