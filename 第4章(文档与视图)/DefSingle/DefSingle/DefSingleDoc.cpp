// DefSingleDoc.cpp : CDefSingleDoc 类的实现
//

#include "stdafx.h"
#include "DefSingle.h"

#include "DefSingleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDefSingleDoc

IMPLEMENT_DYNCREATE(CDefSingleDoc, CDocument)

BEGIN_MESSAGE_MAP(CDefSingleDoc, CDocument)
END_MESSAGE_MAP()


// CDefSingleDoc 构造/析构

CDefSingleDoc::CDefSingleDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDefSingleDoc::~CDefSingleDoc()
{
}

BOOL CDefSingleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDefSingleDoc 序列化

void CDefSingleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CDefSingleDoc 诊断

#ifdef _DEBUG
void CDefSingleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDefSingleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDefSingleDoc 命令

void CDefSingleDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	lpszTitle = L"[第4章]单文档应用程序示例" ;

	CDocument::SetTitle(lpszTitle);
}
