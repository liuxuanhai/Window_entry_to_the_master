#include "StdAfx.h"
#include "Blowfish.h"
#include "bf_data.h"

#define BF_B(x,i)		( *(((LPBYTE)&x)+i) )
#define BF_F(SBox,x)	( ((SBox[0][BF_B(x,0)]+SBox[1][BF_B(x,1)])^SBox[2][BF_B(x,2)])+SBox[3][BF_B(x,3)] )

CBlowfish::CBlowfish(void)
{	
	this->bMode = 0 ;
	memset ( this->PArray, 0, sizeof(this->PArray) ) ;
	memset ( this->SBox, 0, sizeof(this->SBox) ) ;
}

CBlowfish::~CBlowfish(void)
{
}

BOOL CBlowfish::BF_SetKey ( LPBYTE lpKey, UINT nLen ) 
{
	if ( nLen < MIN_KEY_SIZE || nLen > MAX_KEY_SIZE )
		return FALSE ;

	// 步骤1：初始化子密钥和S盒
	int i, j ;
	for ( i = 0; i < 18; i++ )
		this->PArray[i] = P[i] ;

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 256; j++ )
			this->SBox[i][j] = S[i][j] ;
	}

	// 步骤2：将P和K按位异或
	UINT index = 0 ;
	BYTE lpDoubleKey[DEF_BUF_SIZE] = {0} ;
	memcpy ( lpDoubleKey, lpKey, nLen ) ;
	memcpy ( lpDoubleKey+nLen, lpKey, nLen ) ;
	for ( i = 0; i < 18; i++ )
	{
		this->PArray[i] ^= *((DWORD*)(lpDoubleKey+index)) ;
		index = ( index + 4 ) % nLen ;
	}

	// 步骤3：初始加密
	DWORD pl = 0, pr = 0 ;
	for ( i = 0 ; i < 18; i += 2 )
	{
		this->BF_Encode ( pl, pr ) ;
		this->PArray[i] = pl ;
		this->PArray[i+1] = pr ;
	}

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 256; j += 2 )
		{
			this->BF_Encode ( pl, pr ) ;
			this->SBox[i][j] = pl ;
			this->SBox[i][j+1] = pr ;
		}
	}
}

VOID CBlowfish::BF_SetMode ( BYTE bMode ) 
{
	// 目前仅支持0和1
	if ( bMode == 0 || bMode == 1 )
		this->bMode = bMode ;
}

BOOL CBlowfish::BF_Encrypt ( LPBYTE lpData, UINT nSize, LPBYTE lpOut, UINT* lpSize )
{
	memset ( lpOut, 0, *lpSize ) ;
	DWORD dwNeedSize = ( (nSize&0x7) ? (nSize+8-(nSize&0x7)) : nSize ) ;
	if ( dwNeedSize > *lpSize )
		return FALSE ;

	*lpSize = dwNeedSize ;
	memcpy ( lpOut,lpData, nSize ) ;
	
	DWORD pl = 0, pr = 0 ;
	for ( int i = 0 ; i < dwNeedSize; i += 8 )
	{ 
		// ECB模式
		if ( this->bMode == 0 )
		{
			this->BF_Encode ( *((DWORD*)(lpOut+i)), *((DWORD*)(lpOut+i+4)) ) ; 
		}
		// BCB模式
		else if ( this->bMode == 1 )
		{
			pl ^= *((DWORD*)(lpOut+i)) ;
			pr ^= *((DWORD*)(lpOut+i+4)) ;
			this->BF_Encode ( pl, pr ) ; 
			*((DWORD*)(lpOut+i)) = pl ;
			*((DWORD*)(lpOut+i+4)) = pr ;
		}
	}
		
	return TRUE ;
}

BOOL CBlowfish::BF_Decrypt ( LPBYTE lpData, UINT nSize, LPBYTE lpOut, UINT lpSize )
{
	memset ( lpOut, 0, lpSize ) ;
	if ( lpSize < nSize || nSize == 0 ) 
		return FALSE ;

	memcpy ( lpOut, lpData, nSize ) ;

	DWORD pl = 0, pr = 0 ;
	for ( int i = 0 ; i < nSize; i += 8 )
	{ 
		// ECB模式
		if ( this->bMode == 0 )
		{
			this->BF_Decode ( *((DWORD*)(lpOut+i)), *((DWORD*)(lpOut+i+4)) ) ; 
		}
		// BCB模式
		else if ( this->bMode == 1 )
		{
			DWORD cur_pl = pl ;
			DWORD cur_pr = pr ;
			pl = *((DWORD*)(lpOut+i)) ;
			pr = *((DWORD*)(lpOut+i+4)) ;
			this->BF_Decode ( *((DWORD*)(lpOut+i)), *((DWORD*)(lpOut+i+4)) ) ; 
			*((DWORD*)(lpOut+i)) ^= cur_pl ;
			*((DWORD*)(lpOut+i+4)) ^= cur_pr ;
		}
	}

	return TRUE ;
}

VOID CBlowfish::BF_Encode ( DWORD& pl, DWORD& pr )
{
	DWORD next_pl, next_pr ;
	for ( int i = 0; i < 16; i++ )
	{
		next_pr = pl ^ this->PArray[i] ;
		next_pl = BF_F(this->SBox,next_pr) ^ pr ;
		pl = next_pl ;
		pr = next_pr ;
	}
	next_pl = pr ^ this->PArray[17] ;
	next_pr = pl ^ this->PArray[16] ;
	pl = next_pl ;
	pr = next_pr ;
}

VOID CBlowfish::BF_Decode ( DWORD& pl, DWORD& pr )
{
	DWORD next_pr, next_pl ;
	for ( int i = 0; i < 16; i++ )
	{
		next_pr = pl ^ this->PArray[17-i] ;
		next_pl = BF_F(this->SBox,next_pr) ^ pr ;
		pl = next_pl ;
		pr = next_pr ;
	}
	next_pl = pr ^ this->PArray[0] ;
	next_pr = pl ^ this->PArray[1] ;
	pl = next_pl ;
	pr = next_pr ;
}