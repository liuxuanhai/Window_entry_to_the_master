#include "StdAfx.h"
#include "Region.h"

CRegion::CRegion(void)
{
	this->bValid = FALSE ;
}

CRegion::~CRegion(void)
{
}

VOID	CRegion::InitialRect ( CRect TagRect )
{
	this->StatusRect = TagRect ;

	LONG nWidth = TagRect.Width() / MAX_PAGE_NUM ;
	this->StatusRect.right = this->StatusRect.left + nWidth * MAX_PAGE_NUM ;
	LONG nLeft = TagRect.left ;
	for ( int i = 0; i < MAX_PAGE_NUM; i++ )
	{
		this->PageRect[i].left = nLeft ;
		this->PageRect[i].right = this->PageRect[i].left + nWidth ;
		this->PageRect[i].top = TagRect.top ;
		this->PageRect[i].bottom = TagRect.bottom ;
		this->bStatus[i] = FALSE ;
		nLeft += nWidth ;
	}
}

CRect&	CRegion::GetRectByIndex ( UINT nIndex )
{
	return this->PageRect[nIndex] ;
}

CRect&	CRegion::GetStautsRect ()
{
	return this->StatusRect ;
}

VOID	CRegion::ChangeStatusByPt ( CPoint pt )
{
	for ( int i = 0; i < MAX_PAGE_NUM; i++ )
	{
		if ( this->PageRect[i].PtInRect(pt) == TRUE )
		{
			this->bStatus[i] = !this->bStatus[i] ;
			LPVOID lpAddr = (LPVOID)((DWORD)this->lpData+i*DEF_PAGE_SIZE) ;
			if ( this->bStatus[i] )
			{
				VirtualAlloc ( lpAddr, DEF_PAGE_SIZE, MEM_COMMIT, PAGE_READWRITE )  ;
			}
			else
			{
				VirtualFree ( lpAddr, DEF_PAGE_SIZE, MEM_DECOMMIT ) ;
			}
			break ;
		}
	}
}

DWORD	CRegion::GetAddrByStatus ( CPoint pt )
{
	for ( int i = 0; i < MAX_PAGE_NUM; i++ )
	{
		if ( this->PageRect[i].PtInRect(pt) == TRUE )
			return ((DWORD)this->lpData+i*DEF_PAGE_SIZE) ;
	}
	return 0 ;
}

BOOL	CRegion::GetStatusByIndex ( UINT nIndex )
{
	return this->bStatus[nIndex] ;
}

BOOL	CRegion::IsValid ()
{
	return this->bValid ;
}

DWORD	CRegion::GetBaseAddr ()
{
	return (DWORD)this->lpData ;
}

VOID	CRegion::SetValid ( BOOL status )
{
	this->bValid = status ;
	if ( this->bValid )
	{
		for ( int i = 0; i < MAX_PAGE_NUM; i++ )
			this->bStatus[i] = FALSE ;
	}

	DWORD dwSize = MAX_PAGE_NUM * DEF_PAGE_SIZE ;
	if ( this->bValid )
	{
		this->lpData = VirtualAlloc ( NULL, dwSize, MEM_RESERVE, PAGE_READWRITE ) ;
		if ( this->lpData == NULL )
		{
			::MessageBox ( 0, L"VirtualAlloc Error!", 0, 0 ) ;
		}
	}
	else
	{
		if ( this->lpData )
		{
			VirtualFree ( this->lpData, dwSize, MEM_DECOMMIT ) ;
			VirtualFree ( this->lpData, dwSize, MEM_RELEASE ) ;
		}
	}
}