// ClientAppDoc.cpp : implementation of the CClientAppDoc class
//

#include "stdafx.h"
#include "ClientApp.h"

#include "ClientAppDoc.h"
#include "ClientAppView.h"
#include "ChatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientAppDoc

IMPLEMENT_DYNCREATE(CClientAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientAppDoc, CDocument)
	//{{AFX_MSG_MAP(CClientAppDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientAppDoc construction/destruction

CClientAppDoc::CClientAppDoc()
{
	m_pclient=new Client(this);
}

CClientAppDoc::~CClientAppDoc()
{
	delete m_pclient;
}

BOOL CClientAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CEditView* pView = (CEditView*)GetNextView(pos);
		pView->SetWindowText(NULL);
		pView->GetEditCtrl().SetReadOnly(TRUE);		
	}

	SetTitle(_T("ELE3705"));	
	

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CClientAppDoc serialization

void CClientAppDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CClientAppDoc diagnostics

#ifdef _DEBUG
void CClientAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClientAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClientAppDoc commands

BOOL CClientAppDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return TRUE;
}

void CClientAppDoc::DisplayMsg(LPCTSTR lpszText)
{

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		CClientAppView* pClientAppView = DYNAMIC_DOWNCAST(CClientAppView, pView);

		if (pClientAppView != NULL)
			pClientAppView->Message(lpszText);
	}
}
void CClientAppDoc::DeleteContents() 
{
	m_pclient->FermetureClient();
	
	CDocument::DeleteContents();
}

void CClientAppDoc::ActivateChatView(BOOL state) {
	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		CChatView* pChatView = DYNAMIC_DOWNCAST(CChatView, pView);

		if (pChatView != NULL) {
			pChatView->GetEditCtrl().SetReadOnly((state==TRUE)?FALSE:TRUE);
			pChatView->SetWindowText(NULL);
			if(state)
				pChatView->GetParentFrame()->SetActiveView((CView*)pChatView);
		}
	}

}