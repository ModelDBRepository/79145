#include "Resource.h"
#if !defined(AFX_ATTENTIONDIALOG_H__1A26B0A6_2401_4D25_89D9_EC3E16C00BD2__INCLUDED_)
#define AFX_ATTENTIONDIALOG_H__1A26B0A6_2401_4D25_89D9_EC3E16C00BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AttentionDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAttentionDialog dialog

class CAttentionDialog : public CDialog
{
// Construction
public:
	CAttentionDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttentionDialog)
	enum { IDD = IDD_DIALOG1 };
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttentionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttentionDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTENTIONDIALOG_H__1A26B0A6_2401_4D25_89D9_EC3E16C00BD2__INCLUDED_)
