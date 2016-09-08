// SingleView.h : CSingleView 类的接口
//


#pragma once


class CSingleView : public CView
{
protected: // 仅从序列化创建
	CSingleView();
	DECLARE_DYNCREATE(CSingleView)

// 属性
public:
	CSingleDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSingleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // SingleView.cpp 中的调试版本
inline CSingleDoc* CSingleView::GetDocument() const
   { return reinterpret_cast<CSingleDoc*>(m_pDocument); }
#endif

