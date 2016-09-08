
#ifndef _RSA_H
#define _RSA_H

extern "C"
{
	#include "miracl.h"
}


#ifndef _WINDOWS_H
#include <windows.h>
#endif

#define MAXKEYSIZE 4096
#define MAXBIGSIZE 4096

// 定义RSA运算结构
typedef struct _RSA_CONTEXT
{
	UINT		uKeySize ;	// 密钥长度
	big		p, q ;			// 两个大素数
	big		n ;			// 公钥N
	big		e ;			// 加密密钥E
	big		d ;			// 私钥D
} RSA_CONTEXT ;
typedef RSA_CONTEXT*	RSA_PCONTEXT ;


static miracl* mip = mirsys ( MAXBIGSIZE, 0 ) ;

static void RSA_InitContext ( RSA_PCONTEXT RSA_PContext )
{
	RSA_PContext->uKeySize	= 512 ;
	RSA_PContext->p			= mirvar(0) ;
	RSA_PContext->q			= mirvar(0) ;
	RSA_PContext->n			= mirvar(0) ;
	RSA_PContext->e			= mirvar(17) ;
	RSA_PContext->d			= mirvar(0) ;
}

static void RSA_SetKeySize ( RSA_PCONTEXT RSA_PContext, UINT uKeySize = MAXKEYSIZE )
{
	if ( uKeySize >= MAXKEYSIZE || uKeySize < 32 )
		return ;

	RSA_PContext->uKeySize = uKeySize ;
}

static void RSA_GenerateBigPrime ( RSA_PCONTEXT RSA_PContext )
{
	// 初始化随机数生成种子
    irand ( GetTickCount() ) ;

	// 设定随机素数的下限
	big MiniBound = mirvar(1) ;
	sftbit ( MiniBound, RSA_PContext->uKeySize/2-1, MiniBound ) ;

	// 设置RSA_Context中的P、Q值
	while ( true )
	{
		bigdig ( RSA_PContext->uKeySize/8, 16, RSA_PContext->p ) ;
		if ( compare( MiniBound, RSA_PContext->p) == 1 )
			add ( RSA_PContext->p, MiniBound, RSA_PContext->p ) ;

		if ( isprime(RSA_PContext->p) )
			break ;
	}

	while ( true )
	{
		bigdig ( RSA_PContext->uKeySize/8, 16, RSA_PContext->q ) ;
		if ( compare( MiniBound, RSA_PContext->q) == 1 )
			add ( RSA_PContext->q, MiniBound, RSA_PContext->q ) ;

		if ( isprime(RSA_PContext->q) )
			break ;
	}

	mirkill ( MiniBound ) ;
}

static void RSA_GeneratePublicKey_N ( RSA_PCONTEXT RSA_PContext )
{
	//设置RSA_Context中的N值
	multiply ( RSA_PContext->p, RSA_PContext->q, RSA_PContext->n ) ; 
}

static void RSA_GeneratePrivateKey_D ( RSA_PCONTEXT RSA_PContext )
{
	// 计算 TempBig = ( P - 1 ) * ( Q - 1 )
	big TempBig = mirvar(0), TempBigA = mirvar(1), TempBigB = mirvar(1) ;
	subtract ( RSA_PContext->p, TempBigA, TempBigA ) ;
	subtract ( RSA_PContext->q, TempBigB, TempBigB ) ;
	multiply ( TempBigA, TempBigB, TempBig ) ;

	// 利用 xgcd 计算 d = e' mod ((p-1)*(q-1))
	big TempBigD = mirvar(0), TempBigZ = mirvar(0) ;
	//xgcd ( RSA_PContext->e, TempBig, RSA_PContext->d, TempBigD, TempBigZ ) ; 
	multi_inverse ( 1, &RSA_PContext->e, TempBig, &RSA_PContext->d ) ;

	mirkill ( TempBig ) ;	mirkill ( TempBigA ) ;	mirkill ( TempBigB ) ;
	mirkill ( TempBigD ) ;	mirkill ( TempBigZ ) ;
}

static void RSA_SetCryptKey_E ( RSA_PCONTEXT RSA_PContext, big CurBig )
{	
	// 计算 TempBig = ( P - 1 ) * ( Q - 1 )
	big TempBig = mirvar(0), TempBigA = mirvar(1), TempBigB = mirvar(1) ;	
	subtract ( RSA_PContext->p, TempBigA, TempBigA ) ;
	subtract ( RSA_PContext->q, TempBigB, TempBigB ) ;
	multiply ( TempBigA, TempBigB, TempBig ) ;

	// 计算 TempBigZ = gcd ( e, TempBig )
	big TempBigZ = mirvar(0), TempBigStd = mirvar(1) ;
	egcd ( RSA_PContext->e, TempBig, TempBigZ ) ;

	//检测 E 和 ((P-1)*(Q-1))是否互质
	if ( compare( TempBigZ, TempBigStd ) <= 0 )
	{
		mirkill ( TempBig ) ;	mirkill ( TempBigA ) ;	mirkill ( TempBigB ) ;
		mirkill ( TempBigZ ) ;	mirkill ( TempBigStd ) ;

		return ;
	}
	else 
	{	
		mirkill ( TempBig ) ;	mirkill ( TempBigA ) ;	mirkill ( TempBigB ) ;
		mirkill ( TempBigZ ) ;	mirkill ( TempBigStd ) ;

		RSA_PContext->e = CurBig ;
	}
}

static void RSA_UpdateContext ( RSA_PCONTEXT RSA_PContext )
{
	RSA_GeneratePublicKey_N ( RSA_PContext ) ;
	RSA_GeneratePrivateKey_D ( RSA_PContext ) ;
}

static void RSA_EncryptMessage ( RSA_PCONTEXT RSA_PContext, big DigitalMsg, big EncryptedMsg )
{
	powmod ( DigitalMsg, RSA_PContext->e, RSA_PContext->n, EncryptedMsg ) ;
}

static void RSA_DecryptMessage ( RSA_PCONTEXT RSA_PContext, big EncryptedMsg, big DecryptedMsg )
{
	powmod ( EncryptedMsg, RSA_PContext->d, RSA_PContext->n, DecryptedMsg ) ;
}


#endif 