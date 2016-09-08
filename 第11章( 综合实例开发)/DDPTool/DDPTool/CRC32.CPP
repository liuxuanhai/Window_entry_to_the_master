
#include "stdafx.h"
#include "CRC32.h"


CRC32::CRC32 () 
{
	memset ( this->dwTable, 0, sizeof(this->dwTable) ) ;
	this->GenTable () ;
}

void	CRC32::GenTable () 
{
	int i, j, value = 0 ;
	for ( i = 0; i < TABLE_LENGTH; i++ )
	{
		value = i ;
		for ( j = 0; j < 8; j++ )
			value = ( value & 1 ) ? ( ( value >> 1 ) ^ 0xEDB88320 ) : ( value >> 1 ) ;
		this->dwTable[i] = value ;
	}
}

DWORD	CRC32::CRC32_Calc ( LPBYTE lpData, UINT uLength ) 
{
	DWORD crc = 0xFFFFFFFF ;
	for ( UINT i = 0; i < uLength; i++ )
		crc = this->dwTable[ (crc^lpData[i]) & 0xFF ] ^ ( crc >> 8 ) ;
	
	return crc ;
}
