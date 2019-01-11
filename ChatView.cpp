// ChatView.cpp : implementation file
//

#include "stdafx.h"
#include "ClientApp.h"
#include "ChatView.h"
#include "ClientAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatView

IMPLEMENT_DYNCREATE(CChatView, CEditView)

CChatView::CChatView()
{
}

CChatView::~CChatView()
{
}


BEGIN_MESSAGE_MAP(CChatView, CEditView)
	//{{AFX_MSG_MAP(CChatView)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDITION_NETTOYER, OnEditionNettoyer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatView drawing

void CChatView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CChatView diagnostics

#ifdef _DEBUG
void CChatView::AssertValid() const
{
	CEditView::AssertValid();
}

void CChatView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatView message handlers


void CChatView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if ((nChar != VK_RETURN) || (nRepCnt!=1) || (GetEditCtrl().GetStyle() & ES_READONLY)) {
		CEditView::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	else {
		CString strText;
		GetEditCtrl().GetWindowText(strText);

		((CClientAppDoc*)GetDocument())->m_pclient->SendChatMsg(strText);
		GetEditCtrl().SetWindowText(NULL);
	}
}

void CChatView::OnEditionNettoyer() 
{
	SetWindowText(NULL);	
}
