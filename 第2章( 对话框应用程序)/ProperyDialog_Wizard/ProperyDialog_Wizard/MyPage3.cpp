// MyPage3.cpp : 实现文件
//

#include "stdafx.h"
#include "ProperyDialog_Wizard.h"
#include "MyPage3.h"


// CMyPage3 对话框

IMPLEMENT_DYNAMIC(CMyPage3, CPropertyPage)

CMyPage3::CMyPage3()
	: CPropertyPage(CMyPage3::IDD)
{

}

CMyPage3::~CMyPage3()
{
}

void CMyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPage3, CPropertyPage)
END_MESSAGE_MAP()


// CMyPage3 消息处理程序

BOOL CMyPage3::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	((CPropertySheet*)this->GetParent())->SetWizardButtons ( PSWIZB_BACK|PSWIZB_FINISH ) ;
	return CPropertyPage::OnSetActive();
}
