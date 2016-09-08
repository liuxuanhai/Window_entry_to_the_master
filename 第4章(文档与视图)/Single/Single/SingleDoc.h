// SingleDoc.h : CSingleDoc 类的接口
//


#pragma once

#define MAX_X_NUM 80
#define MAX_Y_NUM 50

class CSingleDoc : public CDocument
{
protected: // 仅从序列化创建
	CSingleDoc();
	DECLARE_DYNCREATE(CSingleDoc)

// 属性
public:

// 操作
public:

// 重写
public:
//	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CSingleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	
private:
	BOOL	grid[MAX_X_NUM][MAX_Y_NUM] ;

public:
	VOID	SetGrid ( int x, int y ) ;
	BOOL	GetGridStatus ( int x, int y ) ;
	VOID	ResetGrid () ;
public:
	virtual void SetTitle(LPCTSTR lpszTitle);
public:
	virtual BOOL OnNewDocument();
};


