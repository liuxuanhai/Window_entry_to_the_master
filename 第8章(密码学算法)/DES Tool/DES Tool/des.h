
#ifndef _DES_H
#define _DES_H

class CDES {
public:
	BYTE bOriMsg[8] ;				// 初始消息 ( 64 bit )
	BYTE LMsg[17][4], RMsg[17][4] ;	// 中间变量L0-->L16,R0-->R16 ( 32 bit )
	BYTE bKey[8] ;					// 保存密钥 ( 64 bit )
	BYTE bSubKey[17][6] ;			// 保存子密钥K1-->K16 ( 48 bit )
	BYTE bCryptedMsg[8] ;			// 密文

public:
	void DES_Encrypt ( ) ;
	void DES_Decrypt ( ) ;
	BOOL DES_Encrypt ( LPBYTE lpSour, LPBYTE lpDest, UINT uLen, LPBYTE lpKey ) ;
	BOOL DES_Decrypt ( LPBYTE lpSour, LPBYTE lpDest, UINT uLen, LPBYTE lpKey ) ;

public:
	// 初始置换
	void DESInitSwap ( ) ;
	// 初始逆置换
	void DESInitReSwap () ;
	// 产生子密钥
	void DESGenSubKey ( ) ;
	// DES的单轮加密过程
	void DESSingleTurn ( BYTE nTurnIndex, BOOL bMode ) ;
	// DES的F函数
	void DES_f ( BYTE bTurnIndex, BOOL bMode ) ;

public:
	// 初始化
	void DES_SysInit () ; 
	// 设置明文
	void DES_SetOriMsg ( PBYTE pMsg, UINT uLen ) ;
	// 设置密钥
	void DES_SetKey ( PBYTE pKey, UINT uLen ) ;
} ;
#endif