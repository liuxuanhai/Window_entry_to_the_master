#pragma once

class CProvider {
private:
	UINT	nProtocolNum ;
	LPWSAPROTOCOL_INFOW lpProtocolInfo ;

public:
	CProvider() 
	{
		this->nProtocolNum = 0 ;
		this->lpProtocolInfo = NULL ;
	}
	~CProvider()
	{
		GlobalFree ( this->lpProtocolInfo ) ;
	}
	BOOL	GetProvider ()
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
	VOID	OutputProviderInfo ()
	{
		for ( int i = 0; i < this->nProtocolNum; i++ )
		{
			printf ( "Protocol: %ws\n", this->lpProtocolInfo[i].szProtocol ) ;
			printf ( "CatalogEntryId: %d\tChainLen: %d\n\n", \
				this->lpProtocolInfo[i].dwCatalogEntryId, this->lpProtocolInfo[i].ProtocolChain.ChainLen ) ;
		}
	}
	LPWSAPROTOCOL_INFOW SearchProviderByEntryId ( DWORD dwEntryId )
	{
		for ( int i = 0; i < this->nProtocolNum; i++ )
		{
			if ( this->lpProtocolInfo[i].dwCatalogEntryId == dwEntryId )
				return &(this->lpProtocolInfo[i]) ;
		}
		return NULL ;
	}
} ;
