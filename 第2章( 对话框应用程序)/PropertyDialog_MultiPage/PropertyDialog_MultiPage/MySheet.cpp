// MySheet.cpp : 实现文件
//

#include "stdafx.h"
#include "PropertyDialog_MultiPage.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->AddPage ( &page1 ) ;
	this->AddPage ( &page2 ) ;
	this->AddPage ( &page3 ) ;
}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	this->AddPage ( &page1 ) ;
	this->AddPage ( &page2 ) ;
	this->AddPage ( &page3 ) ;

}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMySheet 消息处理程序
