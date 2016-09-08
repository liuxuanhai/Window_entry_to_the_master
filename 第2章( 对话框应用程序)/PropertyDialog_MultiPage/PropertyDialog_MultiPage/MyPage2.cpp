// MyPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "PropertyDialog_MultiPage.h"
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
