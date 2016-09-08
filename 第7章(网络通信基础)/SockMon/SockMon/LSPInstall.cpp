#include "StdAfx.h"
#include "LSPInstall.h"


CLSPInstall::CLSPInstall() 
{
	this->nProtocolNum = 0 ;
	this->lpProtocolInfo = NULL ;

	GUID TempGuid = { 0xd3c21122, 0x85e1, 0x48f3, {0x9a,0xb6,0x23,0xd9,0x0c,0x73,0x07,0xef} } ;
	memcpy ( &this->ProviderGuid, &TempGuid, sizeof(GUID) ) ;
}

CLSPInstall::~CLSPInstall()
{
	GlobalFree ( this->lpProtocolInfo ) ;
}

BOOL	CLSPInstall::GetProvider ()
{
	GlobalFree ( this->lpProtocolInfo ) ;

	DWORD dwSize = 0 ;
	int dwErrorCode = 0 ;
	if ( ::WSCEnumProtocols ( NULL, this->lpProtocolInfo, &dwSize, &dwErrorCode ) == SOCKET_ERROR )
	{
		if ( dwErrorCode == WSAENOBUFS )
		{
			this->lpProtocolInfo = (LPWSAPROTOCOL_INFOW)GlobalAlloc ( GPTR, dwSize ) ;
			this->nProtocolNum = ::WSCEnumProtocols ( NULL, this->lpProtocolInfo, &dwSize, &dwErrorCode ) ;
			return ( ( this->nProtocolNum != SOCKET_ERROR ) ? TRUE : FALSE ) ;
		}
		return FALSE ;
	}
}

BOOL CLSPInstall::SearchProviderByEntryId ( DWORD dwEntryId, LPWSAPROTOCOL_INFOW lpInfo )
{
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		if ( this->lpProtocolInfo[i].dwCatalogEntryId == dwEntryId )
		{
			memcpy ( lpInfo, &(this->lpProtocolInfo[i]), sizeof(WSAPROTOCOL_INFOW) ) ;
			return TRUE ;
		}
	}
	return FALSE ;
}

BOOL CLSPInstall::SearchProviderByProtocol ( int iProtocol, LPWSAPROTOCOL_INFOW lpInfo )
{
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		if ( this->lpProtocolInfo[i].iAddressFamily == AF_INET && \
			this->lpProtocolInfo[i].iProtocol == iProtocol )
		{
			memcpy ( lpInfo, &(this->lpProtocolInfo[i]), sizeof(WSAPROTOCOL_INFOW) ) ;
			return TRUE ;
		}
	}
	return FALSE ;
}

int CLSPInstall::InstallProvider ( LPWSTR lpProviderPath )
{
	int		nRet = 0, nErrorCode = 0 ;
	WCHAR szLSPName[] = L"My LSP Sample" ;
	WSAPROTOCOL_INFOW	LayeredInfo[3] = {0}, ChainInfo[3] = {0};	// 分层服务信息和协议链信息

	// 取得TCP服务的提供者信息，这里要把LSP安装在它之上
	this->GetProvider () ;
	this->SearchProviderByProtocol ( IPPROTO_IP , &ChainInfo[0] ) ;
	this->SearchProviderByProtocol ( IPPROTO_UDP , &ChainInfo[1] ) ;
	this->SearchProviderByProtocol ( IPPROTO_TCP , &ChainInfo[2] ) ;

	// 填充分层协议信息结构，直接使用下层协议的结构即可
	for ( int i = 0; i < 3; i++ )
	{
		ChainInfo[i].dwServiceFlags1 &= ( ~XP1_IFS_HANDLES ) ;
		memcpy ( &LayeredInfo[i], &ChainInfo[i], sizeof(WSAPROTOCOL_INFOW) ) ;
		LayeredInfo[i].ProtocolChain.ChainLen = LAYERED_PROTOCOL ;
		LayeredInfo[i].dwProviderFlags != PFL_HIDDEN ;
		wcscpy ( LayeredInfo[i].szProtocol, szLSPName ) ;
		switch ( i )
		{
		case 0:	wcscat ( LayeredInfo[i].szProtocol, L"RAW" ) ;	break ;
		case 1:	wcscat ( LayeredInfo[i].szProtocol, L"UDP" ) ;	break ;
		case 2:	wcscat ( LayeredInfo[i].szProtocol, L"TCP" ) ;	break ;
		}
	}

	// 安装分层协议，获取一个Winsock库分配的目录ID号
	DWORD dwLayeredEntryId[3] = {0} ;
	nRet = ::WSCInstallProvider ( &this->ProviderGuid, lpProviderPath, LayeredInfo, 3, &nErrorCode ) ;
	if ( nRet != SOCKET_ERROR )
	{
		this->GetProvider () ;
		for ( int i = 0; i < this->nProtocolNum; i++ )
		{
			if ( memcmp ( &(this->lpProtocolInfo[i].ProviderId), &this->ProviderGuid, sizeof(GUID) ) == 0 )
			{
				switch ( this->lpProtocolInfo[i].iProtocol )
				{
				case IPPROTO_IP:	dwLayeredEntryId[0] = this->lpProtocolInfo[i].dwCatalogEntryId ; break ;
				case IPPROTO_UDP:	dwLayeredEntryId[1] = this->lpProtocolInfo[i].dwCatalogEntryId ; break ;
				case IPPROTO_TCP:	dwLayeredEntryId[2] = this->lpProtocolInfo[i].dwCatalogEntryId ; break ;
				}
			}
		}
	}
	else
	{
		return nErrorCode ;
	}

	// 设置协议链信息结构 
	WCHAR szChainName[MAX_BUF_SIZE] = {0} ;
	for ( int i = 0; i < 3; i++ )
	{
		wsprintf ( szChainName, L"%ws over %ws", szLSPName, ChainInfo[i].szProtocol ) ;
		wcscpy ( ChainInfo[i].szProtocol, szChainName ) ;

		if ( ChainInfo[i].ProtocolChain.ChainLen == 1 )
		{
			ChainInfo[i].ProtocolChain.ChainEntries[1] = ChainInfo[i].dwCatalogEntryId ;
		}
		else
		{
			for ( int j = ChainInfo[i].ProtocolChain.ChainLen; j > 0; j-- )
			{
				ChainInfo[i].ProtocolChain.ChainEntries[j] = ChainInfo[i].ProtocolChain.ChainEntries[j-1] ;
			}
		}
		ChainInfo[i].ProtocolChain.ChainLen ++ ;
		ChainInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredEntryId[i] ;
	}

	// 获取GUID，并安装协议链
	GUID ChainGuid ;
	if ( ::UuidCreate ( &ChainGuid ) == RPC_S_OK )
	{
		nRet = ::WSCInstallProvider ( &ChainGuid, lpProviderPath, ChainInfo, 3, &nErrorCode ) ;
		if ( nRet == SOCKET_ERROR )
		{
			return nErrorCode ;
		}
	}
	else
	{
		return GetLastError() ;
	}

	// 重新排序协议链，将我们自己的协议链提前
	this->GetProvider () ;
	int		nIndex = 0 ;
	DWORD	dwEntryId [ 32] = {0} ;
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		DWORD dwTempEntryId = this->lpProtocolInfo[i].ProtocolChain.ChainEntries[0] ;
		if ( dwTempEntryId == dwLayeredEntryId[0] || \
			dwTempEntryId == dwLayeredEntryId[1] || dwTempEntryId == dwLayeredEntryId[2] ) 
			dwEntryId[nIndex++] = this->lpProtocolInfo[i].dwCatalogEntryId ;
	}
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		//if ( this->lpProtocolInfo[i].ProtocolChain.ChainLen <= 1 || \
		//	 this->lpProtocolInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredEntryId )
		DWORD dwTempEntryId = this->lpProtocolInfo[i].ProtocolChain.ChainEntries[0] ;
		if ( dwTempEntryId != dwLayeredEntryId[0] && \
			dwTempEntryId != dwLayeredEntryId[1] && dwTempEntryId != dwLayeredEntryId[2] ) 
			dwEntryId[nIndex++] = this->lpProtocolInfo[i].dwCatalogEntryId ;
	}

	return ::WSCWriteProviderOrder ( dwEntryId, nIndex ) ;
}

VOID CLSPInstall::RemoveProvider ()
{
	this->GetProvider () ;

	int nErrorCode = 0 ;
	DWORD dwEntryId = 0 ;
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		if ( memcmp ( &this->ProviderGuid, &(this->lpProtocolInfo[i].ProviderId), sizeof(GUID) ) == 0 )
		{
			dwEntryId = this->lpProtocolInfo[i].dwCatalogEntryId ;
			break ;
		}
	}

	if ( dwEntryId == 0 )
		return ;

	// 移除协议链
	for ( int i = 0; i < this->nProtocolNum; i++ )
	{
		if ( this->lpProtocolInfo[i].ProtocolChain.ChainLen > 1 && \
			this->lpProtocolInfo[i].ProtocolChain.ChainEntries[0] == 1083 )
		{
			::WSCDeinstallProvider ( &this->lpProtocolInfo[i].ProviderId, &nErrorCode ) ;
		}
	}

	// 移除分层协议
	::WSCDeinstallProvider ( &this->ProviderGuid, &nErrorCode ) ;
}

