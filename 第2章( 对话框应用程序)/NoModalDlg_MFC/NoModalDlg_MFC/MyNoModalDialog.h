#pragma once


// CMyNoModalDialog 对话框

class CMyNoModalDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyNoModalDialog)

public:
	CMyNoModalDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyNoModalDialog();

// 对话框数据
	enum { IDD = IDD_NOMODAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
