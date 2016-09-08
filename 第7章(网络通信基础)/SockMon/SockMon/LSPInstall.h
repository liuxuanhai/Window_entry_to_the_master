#pragma once

#include <ws2spi.h>
#include <Sporder.h>
#pragma comment ( lib, "Ws2_32.lib" )

#include <rpc.h>
#pragma comment ( lib, "Rpcrt4.lib" )		// 实现UuidCreate函数

class CLSPInstall
{
public:
	CLSPInstall(void);
public:
	~CLSPInstall(void);

private:	
	GUID				ProviderGuid ;			// LSP的全局唯一标识（GUID）
	UINT				nProtocolNum ;			// 协议数量
	LPWSAPROTOCOL_INFOW lpProtocolInfo ;		// 指向协议信息结构数组

public:
	// 安装LSP
	int		InstallProvider ( LPWSTR lpProviderPath ) ;
	// 卸载LSP
	VOID	RemoveProvider () ;

protected:
	// 枚举协议目录，保存信息到lpProtocolInfo
	BOOL	GetProvider () ;
	// 根据目录入口ID查找协议信息结构
	BOOL	SearchProviderByEntryId ( DWORD dwEntryId, LPWSAPROTOCOL_INFOW lpInfo ) ;
	// 根据协议类型查找协议信息结构
	BOOL	SearchProviderByProtocol ( int iProtocol, LPWSAPROTOCOL_INFOW lpInfo ) ;
};
