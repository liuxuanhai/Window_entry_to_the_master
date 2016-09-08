// HuffmanCodingDlg.h : header file
//

#if !defined(AFX_HUFFMANCODINGDLG_H__F6AEB40F_2117_4F18_A6BA_82C08A2883BB__INCLUDED_)
#define AFX_HUFFMANCODINGDLG_H__F6AEB40F_2117_4F18_A6BA_82C08A2883BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HuffmanSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CHuffmanCodingDlg dialog
class CHuffmanCodingDlg : public CDialog
{
// Construction
public:
	CHuffmanCodingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHuffmanCodingDlg)
	enum { IDD = IDD_HUFFMANCODING_DIALOG };
	CString	m_EncodingString;
	CString	m_CodedFileName;
	CString	m_OriginalFileName;
	CString	m_DecodingString;
	CString	m_TextString;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuffmanCodingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHuffmanCodingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEncoding();
	afx_msg void OnDecoding();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnSelOriginalFile();
	afx_msg void OnSelCodedFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl*	GetListCtrl() ;
	void		ShowCodingInfoList () ;

public:
	// 一旦定义CHuffSystem对象，程序就异常退出
	// 由此可以判定问题在于构造函数
	CHuffSystem TextHuffSys ;
	CHuffSystem FileHuffSys ;
	BOOL		bOperFlag ;		//表明当前的操作对象是文件或字符串
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUFFMANCODINGDLG_H__F6AEB40F_2117_4F18_A6BA_82C08A2883BB__INCLUDED_)
