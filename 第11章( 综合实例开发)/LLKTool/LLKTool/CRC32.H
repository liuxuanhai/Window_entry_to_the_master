

#ifndef _CRC32_H
#define _CRC32_H

#define TABLE_LENGTH 256

#include <windows.h>

class CRC32 {
private:
	DWORD dwTable[TABLE_LENGTH] ;

public:
	CRC32 () ;
	~CRC32 () {}

protected:
	void	GenTable () ;

public:
	DWORD	CRC32_Calc ( LPBYTE lpData, UINT uLength ) ;
} ;

#endif 