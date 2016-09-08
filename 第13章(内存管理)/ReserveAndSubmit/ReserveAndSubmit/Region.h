#pragma once

#define MAX_PAGE_NUM	50
#define DEF_PAGE_SIZE	0x1000

class CRegion
{
public:
	CRegion(void);
public:
	~CRegion(void);

private:
	LPVOID	lpData ;
	BOOL	bValid ;
	CRect	StatusRect ;
	CRect	PageRect[MAX_PAGE_NUM] ;
	BOOL  	bStatus[MAX_PAGE_NUM] ;

public:
	VOID	InitialRect ( CRect TagRect ) ;
	CRect&	GetRectByIndex ( UINT nIndex ) ;
	CRect&	GetStautsRect () ;
	VOID	ChangeStatusByPt ( CPoint pt ) ;
	DWORD	GetAddrByStatus ( CPoint pt ) ;
	BOOL	GetStatusByIndex ( UINT nIndex ) ;
	BOOL	IsValid () ;
	VOID	SetValid ( BOOL status ) ;
	DWORD	GetBaseAddr () ;
};
