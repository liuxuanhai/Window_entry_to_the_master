// JoyTester.h : main header file for the JOYTESTER application
//

#if !defined(AFX_JOYTESTER_H__DE91203F_A554_4E3A_ADFC_1BFC4C3101E9__INCLUDED_)
#define AFX_JOYTESTER_H__DE91203F_A554_4E3A_ADFC_1BFC4C3101E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJoyTesterApp:
// See JoyTester.cpp for the implementation of this class
//

class CJoyTesterApp : public CWinApp
{
public:
	CJoyTesterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyTesterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJoyTesterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYTESTER_H__DE91203F_A554_4E3A_ADFC_1BFC4C3101E9__INCLUDED_)
