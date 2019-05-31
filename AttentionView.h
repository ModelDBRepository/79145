// AttentionView.h : interface of the CAttentionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTENTIONVIEW_H__7E542A26_AB6E_42A5_87C8_E933E87DFD8C__INCLUDED_)
#define AFX_ATTENTIONVIEW_H__7E542A26_AB6E_42A5_87C8_E933E87DFD8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAttentionView : public CScrollView
{
protected: // create from serialization only
	CAttentionView();
	DECLARE_DYNCREATE(CAttentionView)

// Attributes
public:
	CAttentionDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttentionView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAttentionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAttentionView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AttentionView.cpp
inline CAttentionDoc* CAttentionView::GetDocument()
   { return (CAttentionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTENTIONVIEW_H__7E542A26_AB6E_42A5_87C8_E933E87DFD8C__INCLUDED_)
