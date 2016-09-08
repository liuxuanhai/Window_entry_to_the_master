// RSA_Tool.h : main header file for the RSA_TOOL application
//

#if !defined(AFX_RSA_TOOL_H__D7AB966F_2851_4ABD_853D_CBB97E21789E__INCLUDED_)
#define AFX_RSA_TOOL_H__D7AB966F_2851_4ABD_853D_CBB97E21789E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRSA_ToolApp:
// See RSA_Tool.cpp for the implementation of this class
//

class CRSA_ToolApp : public CWinApp
{
public:
	CRSA_ToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRSA_ToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRSA_ToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSA_TOOL_H__D7AB966F_2851_4ABD_853D_CBB97E21789E__INCLUDED_)
