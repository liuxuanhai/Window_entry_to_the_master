#pragma once


// CMyModalDialog 对话框

class CMyModalDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyModalDialog)

public:
	CMyModalDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyModalDialog();

// 对话框数据
	enum { IDD = IDD_MODAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
