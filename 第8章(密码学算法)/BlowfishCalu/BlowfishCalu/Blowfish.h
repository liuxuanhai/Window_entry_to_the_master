#pragma once

#include <windows.h>

// 定义密钥的最小长度
#define MIN_KEY_SIZE	4

// 定义密钥的最大长度
#define MAX_KEY_SIZE	56

class CBlowfish
{
public:
	CBlowfish(void);
public:
	~CBlowfish(void);

private:
	BYTE	bMode ;			// 工作模式（0：ECB，1：CBC）
	DWORD	PArray[18] ;	// 子密钥表
	DWORD	SBox[4][256] ;	// S盒

public:
	// 设置密钥，并初始化子密钥和S盒
	BOOL	BF_SetKey ( LPBYTE lpKey, UINT nLen ) ;
	// 设置工作模式，当前仅支持ECB和CBC
	VOID	BF_SetMode ( BYTE bMode ) ;
	// 加密
	BOOL	BF_Encrypt ( LPBYTE lpData, UINT nSize, LPBYTE lpOut, UINT* lpSize ) ;
	// 解密
	BOOL	BF_Decrypt ( LPBYTE lpData, UINT nSize, LPBYTE lpOut, UINT lpSize ) ;

private:
	VOID	BF_Encode ( DWORD& pl, DWORD& pr ) ;
	VOID	BF_Decode ( DWORD& pl, DWORD& pr ) ;

};
