// DefSingleView.cpp : CDefSingleView 类的实现
//

#include "stdafx.h"
#include "DefSingle.h"

#include "DefSingleDoc.h"
#include "DefSingleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDefSingleView

IMPLEMENT_DYNCREATE(CDefSingleView, CView)

BEGIN_MESSAGE_MAP(CDefSingleView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDefSingleView 构造/析构

CDefSingleView::CDefSingleView()
{
	// TODO: 在此处添加构造代码

}

CDefSingleView::~CDefSingleView()
{
}

BOOL CDefSingleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDefSingleView 绘制

void CDefSingleView::OnDraw(CDC* /*pDC*/)
{
	CDefSingleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CDefSingleView 打印

BOOL CDefSingleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDefSingleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDefSingleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CDefSingleView 诊断

#ifdef _DEBUG
void CDefSingleView::AssertValid() const
{
	CView::AssertValid();
}

void CDefSingleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDefSingleDoc* CDefSingleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDefSingleDoc)));
	return (CDefSingleDoc*)m_pDocument;
}
#endif //_DEBUG


// CDefSingleView 消息处理程序
