// SingleDoc.cpp : CSingleDoc 类的实现
//

#include "stdafx.h"
#include "Single.h"

#include "SingleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSingleDoc

IMPLEMENT_DYNCREATE(CSingleDoc, CDocument)

BEGIN_MESSAGE_MAP(CSingleDoc, CDocument)
END_MESSAGE_MAP()


// CSingleDoc 构造/析构

CSingleDoc::CSingleDoc()
{
	// TODO: 在此添加一次性构造代码
	this->ResetGrid () ;
}

CSingleDoc::~CSingleDoc()
{
}

//BOOL CSingleDoc::OnNewDocument()
//{
//	if (!CDocument::OnNewDocument())
//		return FALSE;
//
//	// TODO: 在此添加重新初始化代码
//	// (SDI 文档将重用该文档)
//
//	return TRUE;
//}




// CSingleDoc 序列化

void CSingleDoc::Serialize(CArchive& ar)
{	
	if (ar.IsStoring())
	{
		for ( int i = 0; i < MAX_X_NUM; i++ )
		{
			for ( int j = 0; j < MAX_Y_NUM; j++) 
				ar << this->grid[i][j] ;
		}
	}
	else
	{
		for ( int i = 0; i < MAX_X_NUM; i++ )
		{
			for ( int j = 0; j < MAX_Y_NUM; j++) 
				ar >> this->grid[i][j] ;
		}
	}
}


// CSingleDoc 诊断

#ifdef _DEBUG
void CSingleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSingleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSingleDoc 命令
void CSingleDoc::SetGrid ( int x, int y )
{
	if ( x < 0 || x >= MAX_X_NUM )
		return ;
	if ( y < 0 || y >= MAX_Y_NUM )
		return ;

	this->grid[x][y] = !this->grid[x][y] ;
}

BOOL CSingleDoc::GetGridStatus ( int x, int y )
{
	if ( x < 0 || x >= MAX_X_NUM )
		return FALSE;
	if ( y < 0 || y >= MAX_Y_NUM )
		return FALSE;
	return this->grid[x][y] ;
}

VOID CSingleDoc::ResetGrid ()
{
	memset ( this->grid, 0, sizeof(this->grid) ) ;
}
void CSingleDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	lpszTitle = L"[第4章]单文档应用程序与文档串行化" ;
	CDocument::SetTitle(lpszTitle);
}

BOOL CSingleDoc::OnNewDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->ResetGrid () ;
	return CDocument::OnNewDocument();
}
