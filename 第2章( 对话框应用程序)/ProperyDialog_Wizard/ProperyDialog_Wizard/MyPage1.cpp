// MyPage1.cpp : 实现文件
//

#include "stdafx.h"
#include "ProperyDialog_Wizard.h"
#include "MyPage1.h"


// CMyPage1 对话框

IMPLEMENT_DYNAMIC(CMyPage1, CPropertyPage)

CMyPage1::CMyPage1()
	: CPropertyPage(CMyPage1::IDD)
{

}

CMyPage1::~CMyPage1()
{
}

void CMyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPage1, CPropertyPage)
END_MESSAGE_MAP()


// CMyPage1 消息处理程序

LRESULT CMyPage1::OnWizardBack()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnWizardBack();
}

BOOL CMyPage1::OnWizardFinish()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnWizardFinish();
}

LRESULT CMyPage1::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnWizardNext();
}

BOOL CMyPage1::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	((CPropertySheet*)this->GetParent())->SetWizardButtons ( PSWIZB_NEXT ) ;
	return CPropertyPage::OnSetActive();
}

INT_PTR CMyPage1::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类
	return CPropertyPage::DoModal();
}
