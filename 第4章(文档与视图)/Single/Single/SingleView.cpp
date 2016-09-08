// SingleView.cpp : CSingleView 类的实现
//

#include "stdafx.h"
#include "Single.h"

#include "SingleDoc.h"
#include "SingleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_POS_X	20
#define START_POS_Y 20
#define GRID_SIZE	8


// CSingleView

IMPLEMENT_DYNCREATE(CSingleView, CView)

BEGIN_MESSAGE_MAP(CSingleView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSingleView 构造/析构

CSingleView::CSingleView()
{
	// TODO: 在此处添加构造代码

}

CSingleView::~CSingleView()
{
}

BOOL CSingleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CSingleView 绘制

void CSingleView::OnDraw(CDC* pDC)
{
	CSingleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 取得客户区域，并转化为相对坐标
	CRect WndRect ;
	this->GetWindowRect ( &WndRect ) ;
	this->ScreenToClient ( &WndRect ) ;

	CDC		mdc ;			// 内存DC
	CBitmap bmp ;			// 临时位图，为内存DC提供绘图环境
	CPen	FramePen ;		// 画矩形框使用画笔
	CPen	PtPen ;			// 画点时使用的画笔
	CBrush	BkBrush ;		// 背景画刷，灰色
	CBrush	PtBrush ;		// 画点用的画刷，红色

	// 初始化GDI对象
	mdc.CreateCompatibleDC ( pDC ) ;
	bmp.CreateCompatibleBitmap ( pDC, WndRect.Width(), WndRect.Height() ) ;
	mdc.SelectObject ( &bmp ) ;
	FramePen.CreatePen ( PS_SOLID, 3, RGB(0,0,0) ) ;
	mdc.SelectObject ( &FramePen ) ;
	BkBrush.CreateSolidBrush ( RGB(128,128,128) ) ;

	// 绘制灰色的背景区域
	mdc.FillRect ( &WndRect, &BkBrush ) ;

	// 绘制矩形区域
	int width = MAX_X_NUM * GRID_SIZE, height = MAX_Y_NUM * GRID_SIZE ;
	mdc.Rectangle ( START_POS_X, START_POS_Y, width+START_POS_X, height+START_POS_Y ) ;

	PtPen.CreatePen ( PS_SOLID, 1, RGB(0,0,0) ) ;
	mdc.SelectObject ( &PtPen ) ;
	PtBrush.CreateSolidBrush ( RGB(255,0,0) ) ;
	mdc.SelectObject ( &PtBrush ) ;

	// 枚举CSingleDoc类中的点集数据
	int i, j ;
	for ( i = 0; i < MAX_X_NUM; i++ )
	{
		for ( j = 0; j < MAX_Y_NUM; j++ )
		{
			// 检测指定点的状态，如果为TRUE则在指定位置绘制红点
			if ( pDoc->GetGridStatus(i,j) )
			{
				int x = START_POS_X + GRID_SIZE * i ;
				int y = START_POS_Y + GRID_SIZE * j ;
				mdc.Ellipse ( x, y, x + GRID_SIZE, y + GRID_SIZE ) ;
			}
		}
	}

	// 把内存DC绘制到屏幕DC
	pDC->BitBlt( 0, 0, WndRect.Width(), WndRect.Height(), &mdc, 0, 0, SRCCOPY ) ;

	//删除临时GDI对象
	bmp.DeleteObject () ;
	FramePen.DeleteObject () ;
	PtPen.DeleteObject () ;
	BkBrush.DeleteObject () ;
	PtBrush.DeleteObject () ;
	mdc.DeleteDC() ;
}


// CSingleView 打印

BOOL CSingleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSingleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSingleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CSingleView 诊断

#ifdef _DEBUG
void CSingleView::AssertValid() const
{
	CView::AssertValid();
}

void CSingleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSingleDoc* CSingleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSingleDoc)));
	return (CSingleDoc*)m_pDocument;
}
#endif //_DEBUG


// CSingleView 消息处理程序

void CSingleView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( point.x < START_POS_X || point.x >= START_POS_X+GRID_SIZE*MAX_X_NUM )
		return ;
	if ( point.y < START_POS_Y || point.y >= START_POS_Y+GRID_SIZE*MAX_Y_NUM )
		return ;

	CSingleDoc* pDoc = GetDocument();
	if ( pDoc )
	{
		int x = ( point.x - START_POS_X ) / GRID_SIZE ;
		int y = ( point.y - START_POS_Y ) / GRID_SIZE ;
		pDoc->SetGrid ( x, y ) ;
		pDoc->SetModifiedFlag () ;
		this->Invalidate ( FALSE ) ;
	}
	else
	{
		this->MessageBox ( L"Error!" ) ;
	}
	CView::OnLButtonUp(nFlags, point);
}

BOOL CSingleView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE ;
}
