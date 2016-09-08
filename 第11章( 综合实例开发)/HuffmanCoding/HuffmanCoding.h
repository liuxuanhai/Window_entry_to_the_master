// HuffmanCoding.h : main header file for the HUFFMANCODING application
//

#if !defined(AFX_HUFFMANCODING_H__CC193871_6784_463B_8705_CE5AAE75F64B__INCLUDED_)
#define AFX_HUFFMANCODING_H__CC193871_6784_463B_8705_CE5AAE75F64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHuffmanCodingApp:
// See HuffmanCoding.cpp for the implementation of this class
//

class CHuffmanCodingApp : public CWinApp
{
public:
	CHuffmanCodingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuffmanCodingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHuffmanCodingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUFFMANCODING_H__CC193871_6784_463B_8705_CE5AAE75F64B__INCLUDED_)
