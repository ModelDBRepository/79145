// AttentionDoc.h : interface of the CAttentionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTENTIONDOC_H__CAED4230_D749_4707_858D_CE3D7697B9BD__INCLUDED_)
#define AFX_ATTENTIONDOC_H__CAED4230_D749_4707_858D_CE3D7697B9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAttentionDoc : public CDocument
{
protected: // create from serialization only
	CAttentionDoc();
	DECLARE_DYNCREATE(CAttentionDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttentionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAttentionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAttentionDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTENTIONDOC_H__CAED4230_D749_4707_858D_CE3D7697B9BD__INCLUDED_)
