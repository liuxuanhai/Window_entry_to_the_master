#pragma once


// CMyPage2 对话框

class CMyPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage2)

public:
	CMyPage2();
	virtual ~CMyPage2();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
