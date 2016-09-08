#pragma once

class CIDEA
{
public:
	CIDEA(void);
	~CIDEA(void);

private:
	BYTE	bKey[16] ;		// 用户密钥，128位
	UINT16	bEnKey[53] ;	// bEnKey[0]未用，1--52
	UINT16	bDeKey[53] ;	// bDeKey[0]未用，1--52

public:
	// 设置128位密钥，并生成加密和解密子密钥分组
	BOOL	SetKey ( LPBYTE lpKey, UINT nSize ) ;
	// 加密数据
	BOOL	EncryptData ( LPBYTE lpData, UINT nSize, LPBYTE lpBuf, UINT* lpSize ) ;
	// 解密数据
	BOOL	DecryptData ( LPBYTE lpData, UINT nSize, LPBYTE lpBuf, UINT* lpSize ) ;

private:
	// 产生加密子密钥分组
	VOID	GenEncryptKey () ;
	// 产生解密子密钥分组
	VOID	GenDecryptKey () ;
	// 加密过程轮变换
	VOID	EnRoundTransform ( UINT16& x1, UINT16& x2, UINT16& x3, UINT16& x4, UINT nRoundIndex ) ;
	// 解密过程轮变换
	VOID	DeRoundTransform ( UINT16& x1, UINT16& x2, UINT16& x3, UINT16& x4, UINT nRoundIndex ) ;

private:
	// 取得密钥指定位的值
	inline int	GetBitByIndex ( UINT nIndex ) ;
	// 扩展欧几里得算法，求乘法逆元
	int		ExtendedEuclid ( int e, int f ) ;
};
