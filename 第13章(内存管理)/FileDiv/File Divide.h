// File Divide.h : main header file for the FILE DIVIDE application
//

#if !defined(AFX_FILEDIVIDE_H__63A61AAA_3A00_4E26_8846_06C0CEBCEC0A__INCLUDED_)
#define AFX_FILEDIVIDE_H__63A61AAA_3A00_4E26_8846_06C0CEBCEC0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFileDivideApp:
// See File Divide.cpp for the implementation of this class
//

class CFileDivideApp : public CWinApp
{
public:
	CFileDivideApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDivideApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFileDivideApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIVIDE_H__63A61AAA_3A00_4E26_8846_06C0CEBCEC0A__INCLUDED_)
