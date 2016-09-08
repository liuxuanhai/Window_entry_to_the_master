// EightPuzzleDlg.h : 头文件
//

#pragma once

#include "EightPuzzleGame.h"


// CEightPuzzleDlg 对话框
class CEightPuzzleDlg : public CDialog
{
// 构造
public:
	CEightPuzzleDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EIGHTPUZZLE_DIALOG };

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
	CWinThread*			pNewThread ;
	BOOL				isShowing ;
	CEightPuzzleGame	EightPuzzle ;
	friend UINT ShowProc ( LPVOID lpParam ) ;

public:
	afx_msg void OnBnClickedRandomBeg();
	afx_msg void OnBnClickedRestart();
	afx_msg void OnBnClickedBegShow();
	afx_msg void OnBnClickedEndShow();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	void MoveUp();
	void MoveLeft();
	void MoveDown();
	void MoveRight();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
