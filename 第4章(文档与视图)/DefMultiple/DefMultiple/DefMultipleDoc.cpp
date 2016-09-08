// DefMultipleDoc.cpp : CDefMultipleDoc 类的实现
//

#include "stdafx.h"
#include "DefMultiple.h"

#include "DefMultipleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDefMultipleDoc

IMPLEMENT_DYNCREATE(CDefMultipleDoc, CDocument)

BEGIN_MESSAGE_MAP(CDefMultipleDoc, CDocument)
END_MESSAGE_MAP()


// CDefMultipleDoc 构造/析构

CDefMultipleDoc::CDefMultipleDoc()
{
	// TODO: 在此添加一次性构造代码

}

CDefMultipleDoc::~CDefMultipleDoc()
{
}

BOOL CDefMultipleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CDefMultipleDoc 序列化

void CDefMultipleDoc::Serialize(CArchive& ar)
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


// CDefMultipleDoc 诊断

#ifdef _DEBUG
void CDefMultipleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDefMultipleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDefMultipleDoc 命令

void CDefMultipleDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	lpszTitle = L"[第4章]多文档应用程序示例" ;

	CDocument::SetTitle(lpszTitle);
}
