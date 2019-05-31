// AttentionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Attention.h"
#include "AttentionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttentionDialog dialog


CAttentionDialog::CAttentionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAttentionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttentionDialog)
	m_Text = _T("");
	//}}AFX_DATA_INIT
}


void CAttentionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttentionDialog)
	DDX_Text(pDX, IDC_EDIT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttentionDialog, CDialog)
	//{{AFX_MSG_MAP(CAttentionDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttentionDialog message handlers

BOOL CAttentionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
