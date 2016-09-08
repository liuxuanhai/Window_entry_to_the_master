#pragma once


// CMyPage1 对话框

class CMyPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage1)

public:
	CMyPage1();
	virtual ~CMyPage1();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT OnWizardBack();
public:
	virtual BOOL OnWizardFinish();
public:
	virtual LRESULT OnWizardNext();
public:
	virtual BOOL OnSetActive();
public:
	virtual INT_PTR DoModal();
};
