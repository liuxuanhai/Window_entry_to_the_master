// FileSystem.h : main header file for the FILESYSTEM application
//

#if !defined(AFX_FILESYSTEM_H__83922CB1_2D3C_4717_A89D_6422D3495064__INCLUDED_)
#define AFX_FILESYSTEM_H__83922CB1_2D3C_4717_A89D_6422D3495064__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileSystemApp:
// See FileSystem.cpp for the implementation of this class
//

class CFileSystemApp : public CWinApp
{
public:
	CFileSystemApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSystemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileSystemApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESYSTEM_H__83922CB1_2D3C_4717_A89D_6422D3495064__INCLUDED_)
