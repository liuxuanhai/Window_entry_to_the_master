// MyPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "ProperyDialog_Wizard.h"
#include "MyPage2.h"


// CMyPage2 对话框

IMPLEMENT_DYNAMIC(CMyPage2, CPropertyPage)

CMyPage2::CMyPage2()
	: CPropertyPage(CMyPage2::IDD)
{

}

CMyPage2::~CMyPage2()
{
}

void CMyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPage2, CPropertyPage)
END_MESSAGE_MAP()


// CMyPage2 消息处理程序

BOOL CMyPage2::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	((CPropertySheet*)this->GetParent())->SetWizardButtons ( PSWIZB_BACK|PSWIZB_NEXT ) ;
	return CPropertyPage::OnSetActive();
}
