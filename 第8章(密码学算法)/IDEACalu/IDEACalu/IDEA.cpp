#include "StdAfx.h"
#include "IDEA.h"

#define XOR(a,b) (a^b)
#define ADD(a,b) ((UINT16)(((UINT)a+(UINT)b)&0xFFFF))
#define MUL(a,b) ((UINT16)(((UINT)a*(UINT)b)%0x10001 ))

CIDEA::CIDEA(void)
{
	memset ( this->bKey, 0, sizeof(this->bKey) ) ;
	memset ( this->bEnKey, 0, sizeof(this->bEnKey) ) ;
	memset ( this->bDeKey, 0, sizeof(this->bDeKey) ) ;
}

CIDEA::~CIDEA(void)
{
}

BOOL CIDEA::SetKey ( LPBYTE lpKey, UINT nSize ) 
{
	if ( nSize == 0 )
		return FALSE ;

	UINT nRealSize = ( nSize < 16 ) ? nSize : 16 ;
	memset ( this->bKey, 0, 16 ) ;
	memcpy ( this->bKey, lpKey, nRealSize ) ;
	this->GenEncryptKey () ;
	this->GenDecryptKey () ;
	return TRUE ;
}

BOOL CIDEA::EncryptData ( LPBYTE lpData, UINT nSize, LPBYTE lpBuf, UINT* lpSize )
{
	UINT nNeedSize = ( nSize % 8 ) ? ( nSize / 8 * 8 + 8 ) : nSize ;
	if ( *lpSize < nNeedSize )
		return FALSE ;

	memset ( lpBuf, 0, sizeof(lpBuf) ) ;
	memcpy ( lpBuf, lpData, nNeedSize ) ;

	UINT16 a, b, c, d ;
	int nTime = nNeedSize / 8 ;
	for ( int i = 0; i < nTime; i++ )
	{
		a = (lpBuf[8*i]<<8) + lpBuf[8*i+1];
		b = (lpBuf[8*i+2]<<8) + lpBuf[8*i+3];
		c = (lpBuf[8*i+4]<<8) + lpBuf[8*i+5];
		d = (lpBuf[8*i+6]<<8) + lpBuf[8*i+7];

		for ( int j = 1; j <= 8; j++ )
			this->EnRoundTransform ( a, b, c, d, j ) ;
		this->EnRoundTransform ( a, c, b, d, 9 ) ;

		lpBuf[8*i] = (BYTE)(a>>8);
		lpBuf[8*i+1] = (a&0xFF);
		lpBuf[8*i+2] = (BYTE)(c>>8);
		lpBuf[8*i+3] = (c&0xFF);
		lpBuf[8*i+4] = (BYTE)(b>>8);
		lpBuf[8*i+5] = (b&0xFF);
		lpBuf[8*i+6] = (BYTE)(d>>8);
		lpBuf[8*i+7] = (d&0xFF);
	}
	*lpSize = nNeedSize ;
	return TRUE ;
}

BOOL CIDEA::DecryptData ( LPBYTE lpData, UINT nSize, LPBYTE lpBuf, UINT* lpSize )
{
	UINT nNeedSize = ( nSize % 8 ) ? ( nSize / 8 * 8 + 8 ) : nSize ;
	if ( *lpSize < nNeedSize )
		return FALSE ;

	memset ( lpBuf, 0, sizeof(lpBuf) ) ;
	memcpy ( lpBuf, lpData, nNeedSize ) ;

	UINT16 a, b, c, d ;
	int nTime = nNeedSize / 8 ;
	for ( int i = 0; i < nTime; i++ )
	{
		a = (lpBuf[8*i]<<8) + lpBuf[8*i+1];
		b = (lpBuf[8*i+2]<<8) + lpBuf[8*i+3];
		c = (lpBuf[8*i+4]<<8) + lpBuf[8*i+5];
		d = (lpBuf[8*i+6]<<8) + lpBuf[8*i+7];

		for ( int j = 1; j <= 8; j++ )
			this->DeRoundTransform ( a, b, c, d, j ) ;
		this->DeRoundTransform ( a, c, b, d, 9 ) ;

		lpBuf[8*i] = (BYTE)(a>>8);
		lpBuf[8*i+1] = (a&0xFF);
		lpBuf[8*i+2] = (BYTE)(c>>8);
		lpBuf[8*i+3] = (c&0xFF);
		lpBuf[8*i+4] = (BYTE)(b>>8);
		lpBuf[8*i+5] = (b&0xFF);
		lpBuf[8*i+6] = (BYTE)(d>>8);
		lpBuf[8*i+7] = (d&0xFF);
	}
	*lpSize = nNeedSize ;
	return TRUE ;
}

// 生成加密子密钥
VOID CIDEA::GenEncryptKey ()
{
	int nBitIndex = 1 ;
	// 依次生成52个子密钥
	for ( int nKeyIndex = 1; nKeyIndex <= 52; nKeyIndex++ )
	{
		//  每个子密钥16位
		for ( int i = 0; i < 16; i++ )
		{
			this->bEnKey[nKeyIndex] <<= 1 ;
			this->bEnKey[nKeyIndex] += this->GetBitByIndex( (nBitIndex-1)&0x7F ) ;
			nBitIndex++ ;
		}
		// 左移25位
		if ( ( nKeyIndex % 8 ) == 0 )
			nBitIndex += 25 ;
	}
}

// 产生解密子密钥
VOID CIDEA::GenDecryptKey ()
{
	// 定义解密子密钥到加密子密钥的映射表
	int map[53] = { 0, 49, 50, 51, 52, 47, 48, \
					   43, 45, 44, 46, 41, 42, \
					   37, 39, 38, 40, 35, 36, \
					   31, 33, 32, 34, 29, 30, \
					   25, 27, 26, 28, 23, 24, \
					   19, 21, 20, 22, 17, 18, \
					   13, 15, 14, 16, 11, 12, \
					   7,  9,  8,  10,  5,  6, \
					   1,  2,  3,  4 } ;
	// 定义从加密子密钥到解密子密钥的映射类型
	// 1：乘法逆元
	// 0：原样
	// -1：模2^16负
	int flag[53] = { 0, 1,-1,-1,1,0,0,1,-1,-1,1,0,0,1,-1,-1,1,0,0,1,-1,-1,1,0,0, \
						1,-1,-1,1,0,0,1,-1,-1,1,0,0,1,-1,-1,1,0,0,1,-1,-1,1,0,0,1,-1,-1,1} ;

	for ( int nKeyIndex = 1; nKeyIndex <= 52; nKeyIndex++ )
	{
		switch ( flag[nKeyIndex] )
		{
		case 1:
			this->bDeKey[nKeyIndex] = this->ExtendedEuclid(this->bEnKey[map[nKeyIndex]], 0x10001) ;
			break ;
		case 0:
			this->bDeKey[nKeyIndex] = this->bEnKey[map[nKeyIndex]] ;
			break ;
		case -1:
			this->bDeKey[nKeyIndex] = -1 * this->bEnKey[map[nKeyIndex]] ;
			break ;
		}
	}
}

VOID CIDEA::EnRoundTransform ( UINT16& x1, UINT16& x2, UINT16& x3, UINT16& x4, UINT nRoundIndex )
{
	if ( nRoundIndex > 9 )
		return ;
	else if ( nRoundIndex == 9 )
	{
		x1 = MUL(x1,this->bEnKey[49]) ;
		x2 = ADD(x2,this->bEnKey[50]) ;
		x3 = ADD(x3,this->bEnKey[51]) ;
		x4 = MUL(x4,this->bEnKey[52]) ;
		return ;
	}

	UINT16 k1 = this->bEnKey[(nRoundIndex-1)*6+1] ;
	UINT16 k2 = this->bEnKey[(nRoundIndex-1)*6+2] ;
	UINT16 k3 = this->bEnKey[(nRoundIndex-1)*6+3] ;
	UINT16 k4 = this->bEnKey[(nRoundIndex-1)*6+4] ;
	UINT16 k5 = this->bEnKey[(nRoundIndex-1)*6+5] ;
	UINT16 k6 = this->bEnKey[(nRoundIndex-1)*6+6] ;

	UINT16 v1 = MUL(x1,k1) ;
	UINT16 v2 = ADD(x2,k2) ;
	UINT16 v3 = ADD(x3,k3) ;
	UINT16 v4 = MUL(x4,k4) ;
	UINT16 v5 = XOR(v1,v3) ;
	UINT16 v6 = XOR(v2,v4) ;
	UINT16 v7 = MUL(v5,k5) ;
	UINT16 v8 = ADD(v6,v7) ;
	UINT16 v9 = MUL(v8,k6) ;
	UINT16 v10 = ADD(v7,v9) ;
	UINT16 v11 = XOR(v1,v9) ;
	UINT16 v12 = XOR(v3,v9) ;
	UINT16 v13 = XOR(v2,v10) ;
	UINT16 v14 = XOR(v4,v10) ;

	x1 = v11 ;
	x2 = v12 ;
	x3 = v13 ;
	x4 = v14 ;
}

VOID CIDEA::DeRoundTransform ( UINT16& x1, UINT16& x2, UINT16& x3, UINT16& x4, UINT nRoundIndex )
{
	if ( nRoundIndex > 9 )
		return ;
	else if ( nRoundIndex == 9 )
	{
		x1 = MUL(x1,this->bDeKey[49]) ;
		x2 = ADD(x2,this->bDeKey[50]) ;
		x3 = ADD(x3,this->bDeKey[51]) ;
		x4 = MUL(x4,this->bDeKey[52]) ;
		return ;
	}

	UINT16 k1 = this->bDeKey[(nRoundIndex-1)*6+1] ;
	UINT16 k2 = this->bDeKey[(nRoundIndex-1)*6+2] ;
	UINT16 k3 = this->bDeKey[(nRoundIndex-1)*6+3] ;
	UINT16 k4 = this->bDeKey[(nRoundIndex-1)*6+4] ;
	UINT16 k5 = this->bDeKey[(nRoundIndex-1)*6+5] ;
	UINT16 k6 = this->bDeKey[(nRoundIndex-1)*6+6] ;

	UINT16 v1 = MUL(x1,k1) ;
	UINT16 v2 = ADD(x2,k2) ;
	UINT16 v3 = ADD(x3,k3) ;
	UINT16 v4 = MUL(x4,k4) ;
	UINT16 v5 = XOR(v1,v3) ;
	UINT16 v6 = XOR(v2,v4) ;
	UINT16 v7 = MUL(v5,k5) ;
	UINT16 v8 = ADD(v6,v7) ;
	UINT16 v9 = MUL(v8,k6) ;
	UINT16 v10 = ADD(v7,v9) ;
	UINT16 v11 = XOR(v1,v9) ;
	UINT16 v12 = XOR(v3,v9) ;
	UINT16 v13 = XOR(v2,v10) ;
	UINT16 v14 = XOR(v4,v10) ;

	x1 = v11 ;
	x2 = v12 ;
	x3 = v13 ;
	x4 = v14 ;
}

inline int CIDEA::GetBitByIndex ( UINT nIndex )
{
	BYTE bBitMask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 } ;
	return ( this->bKey[nIndex/8] & bBitMask[nIndex&0x7] ) ? 1 : 0 ;
}

/////////////////////////////////////////////////////////
/*
求e的乘法逆元e'，a * e + b * f = 1
用扩展的欧几里德算法。算法描述如下 
ExtendedEuclid(e,f) 
1 （X1,X2,X3):=(1,0,f) 
2  (Y1,Y2,Y3):=(0,1,e) 
3  if (Y3=0) then return  e'=null//无逆元 
4  if (Y3=1) then return  e'=Y2  //Y2为逆元 
5  Q:=X3 div Y3 
6  (T1,T2,T3):=(X1-Q*Y1,X2-Q*Y2,X3-Q*Y3) 
7 （X1,X2,X3):=(Y1,Y2,Y3) 
8  (Y1,Y2,Y3):=(T1,T2,T3) 
9  goto 3 
*/
/////////////////////////////////////////////////////////
int CIDEA::ExtendedEuclid ( int e, int f) 
{
	int x1 = 1, x2 = 0, x3 = f ;
	int y1 = 0, y2 = 1, y3 = e ;
	if ( x3 == 0 || y3 == 0 )
		return 0 ;
	if ( y3 == 1 )
		return y2 ;

	while ( true )
	{
		int b = x3 / y3 ;
		int t1 = x1 - y1 * b ;
		int t2 = x2 - y2 * b ;
		int t3 = x3 - y3 * b ;
		x1 = y1 ; x2 = y2 ; x3 = y3 ;
		y1 = t1 ; y2 = t2 ; y3 = t3 ;
		if ( y3 == 1 )
		{
			if ( y2 > 0 )
				return y2 ;
			else
				return y2 + f ;
		}
	}
	return 0 ;
}