// Attention.h : main header file for the ATTENTION application
//

#if !defined(AFX_ATTENTION_H__60871952_DDBD_4099_8D11_23A37A02256F__INCLUDED_)
#define AFX_ATTENTION_H__60871952_DDBD_4099_8D11_23A37A02256F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAttentionApp:
// See Attention.cpp for the implementation of this class
//

class CAttentionApp : public CWinApp
{
public:
	CAttentionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttentionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAttentionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTENTION_H__60871952_DDBD_4099_8D11_23A37A02256F__INCLUDED_)
