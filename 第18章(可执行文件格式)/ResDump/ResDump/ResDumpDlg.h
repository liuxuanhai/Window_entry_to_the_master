// ResDumpDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "PEInfo.h"


#define MAX_DEF_RES_TYPE 24

// CResDumpDlg 对话框
class CResDumpDlg : public CDialog
{
// 构造
public:
	CResDumpDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RESDUMP_DIALOG };

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
	CTreeCtrl		m_ResDirectory;						// 树型控件
	CPEInfo			PEInfo ;								
	BOOL			isDumpValid ;						// 标识树型控件的当前所选项是否可以DUMP（即是否叶子结点）
	LPVOID			lpDumpResStart ;					// 当前选定的资源的VA
	DWORD			dwResSize ;							// 当前选定的资源的SIZE
	CString			szResourceType[MAX_DEF_RES_TYPE] ;	// 资源类型的ID与字符串对应表

public:
	afx_msg void OnBnClickedOk();						// 用于DUMP当前选定的资源
public:
	afx_msg void OnBnClickedCancel();
public:
	BOOL GetResourceInfo(LPTSTR lpFilePath);			// 取得目标的文件的相关信息（主要为资源信息）
public:
	CString m_FilePath;									// 目标文件名
public:
	CString m_RootNameEntryNum;	
public:
	CString m_RootIdEntryNum;
public:
	CString m_SubNameEntryNum;
public:
	CString m_SubIdEntryNum;
public:
	CString m_DataOffset;
public:
	CString m_DataRVA;
public:
	CString m_DataSize;
public:
	afx_msg void OnBnClickedBrowse();
public:
	void UpdateTreeCtrl(void);
public:
	// 取得当前选定项在其兄弟结点中的索引
	int GetIndexOfTreeCtrl(HTREEITEM hTreeItem);
public:
	// 响应树型控件消息Selected Item Changed
	afx_msg void OnTvnSelchangedResDirectory(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CString m_ResInfo;			// 资源附加信息
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
