#pragma once


// CMyPage3 对话框

class CMyPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage3)

public:
	CMyPage3();
	virtual ~CMyPage3();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
};
