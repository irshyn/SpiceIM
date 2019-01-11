// ClientAppView.cpp : implementation of the CClientAppView class
//

#include "stdafx.h"
#include "ClientApp.h"

#include "ClientAppDoc.h"
#include "ClientAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientAppView

IMPLEMENT_DYNCREATE(CClientAppView, CEditView)

BEGIN_MESSAGE_MAP(CClientAppView, CEditView)
	//{{AFX_MSG_MAP(CClientAppView)
	ON_COMMAND(ID_EDITION_NETTOYER, OnEditionNettoyer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientAppView construction/destruction

CClientAppView::CClientAppView()
{
	// TODO: add construction code here

}

CClientAppView::~CClientAppView()
{
}

BOOL CClientAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CClientAppView drawing

void CClientAppView::OnDraw(CDC* pDC)
{
	CClientAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CClientAppView diagnostics

#ifdef _DEBUG
void CClientAppView::AssertValid() const
{
	CEditView::AssertValid();
}

void CClientAppView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CClientAppDoc* CClientAppView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientAppDoc)));
	return (CClientAppDoc*)m_pDocument;
}
#endif //_DEBUG

void CClientAppView::Message(LPCTSTR lpszMessage)
{
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len,len);
	GetEditCtrl().ReplaceSel(strTemp);
}
void CClientAppView::OnEditionNettoyer() 
{
	SetWindowText(NULL);
	
}
