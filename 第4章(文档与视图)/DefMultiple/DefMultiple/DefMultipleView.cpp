// DefMultipleView.cpp : CDefMultipleView 类的实现
//

#include "stdafx.h"
#include "DefMultiple.h"

#include "DefMultipleDoc.h"
#include "DefMultipleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDefMultipleView

IMPLEMENT_DYNCREATE(CDefMultipleView, CView)

BEGIN_MESSAGE_MAP(CDefMultipleView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDefMultipleView 构造/析构

CDefMultipleView::CDefMultipleView()
{
	// TODO: 在此处添加构造代码

}

CDefMultipleView::~CDefMultipleView()
{
}

BOOL CDefMultipleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDefMultipleView 绘制

void CDefMultipleView::OnDraw(CDC* /*pDC*/)
{
	CDefMultipleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDefMultipleView 打印

BOOL CDefMultipleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDefMultipleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDefMultipleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CDefMultipleView 诊断

#ifdef _DEBUG
void CDefMultipleView::AssertValid() const
{
	CView::AssertValid();
}

void CDefMultipleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDefMultipleDoc* CDefMultipleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDefMultipleDoc)));
	return (CDefMultipleDoc*)m_pDocument;
}
#endif //_DEBUG


// CDefMultipleView 消息处理程序
