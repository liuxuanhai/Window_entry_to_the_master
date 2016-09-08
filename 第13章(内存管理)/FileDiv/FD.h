// FD.h: interface for the CFD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FD_H__070D6219_0D5B_41B5_B37A_3BF5CE6E5AF0__INCLUDED_)
#define AFX_FD_H__070D6219_0D5B_41B5_B37A_3BF5CE6E5AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _FILE_ITEM_INFO {
	DWORD	dwLowFileSize ;
	DWORD	dwHighFileSize ;
	DWORD	dwPartSize ;
	DWORD	dwPartNum ;
	CString	szFileName ;
	_FILE_ITEM_INFO ()
	{
		dwLowFileSize = dwHighFileSize = 0 ;
		dwPartSize = 1024 * 1024 ;
		dwPartNum = 0 ;
		szFileName = "" ;
	}
} FILE_ITEM_INFO ;
typedef FILE_ITEM_INFO* PFILE_ITEM_INFO ;

typedef struct _MAP_ITEM_INFO {
	HANDLE	hFile ;
	HANDLE	hMapFile ;
	LPVOID	lpBase ;
	_MAP_ITEM_INFO ()
	{
		hFile = hMapFile = INVALID_HANDLE_VALUE ;
		lpBase = 0 ;
	}
} MAP_ITEM_INFO ;
typedef MAP_ITEM_INFO* PMAP_ITEM_INFO ;

class CFD  
{
public:
	CFD();
	virtual ~CFD();

public:
	DWORD	FDFileDivide	( PFILE_ITEM_INFO pItem ) ;
	DWORD	FDFileConnect	( PFILE_ITEM_INFO pItem ) ;

	DWORD	FDGenFileInfo	( CString szFileName, PFILE_ITEM_INFO pItem ) ;
	DWORD	FDGetFileInfo	( CString szFileName, PFILE_ITEM_INFO pItem ) ;

	void	DeleteAllPartFiles ( CString szFileName ) ;
};

#endif // !defined(AFX_FD_H__070D6219_0D5B_41B5_B37A_3BF5CE6E5AF0__INCLUDED_)
