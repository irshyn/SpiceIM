// ClientApp.h : main header file for the CLIENTAPP application
//

#if !defined(AFX_CLIENTAPP_H__BA1DD1EA_6504_449E_8289_4A893AF650F0__INCLUDED_)
#define AFX_CLIENTAPP_H__BA1DD1EA_6504_449E_8289_4A893AF650F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CClientAppApp:
// See ClientApp.cpp for the implementation of this class
//

class CClientAppApp : public CWinApp
{
public:
	CClientAppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CClientAppApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTAPP_H__BA1DD1EA_6504_449E_8289_4A893AF650F0__INCLUDED_)
