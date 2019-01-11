// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ClientApp.h"


#include "MainFrm.h"
#include "ClientAppDoc.h"
#include "ChatView.h"
#include "ClientAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SERVEUR_DECONNECTER, OnServeurDeconnecter)
	ON_COMMAND(ID_SERVEUR_CONNECTER, OnServeurConnecter)
	ON_COMMAND(ID_SERVEUR_INFORMATION, OnServeurInformation)
	ON_COMMAND(ID_SERVEUR_LISTE_CLIENTS, OnServeurListeClients)
	ON_COMMAND(ID_SERVEUR_PINGSERVEUR, OnServeurPingServeur)
	ON_COMMAND(ID_CLIENT_DEMANDEDECONNECTION, OnClientDemandedeconnection)
	ON_COMMAND(ID_CLIENT_TRANSFERTDEFICHIER, OnClientTransfertdefichier)
	ON_COMMAND(ID_CLIENT_FERMETURECONNECTION, OnClientFermetureconnection)
	ON_WM_TIMER()
	ON_COMMAND(ID_EDITION_MONITOR, OnEditionMonitor)
	ON_UPDATE_COMMAND_UI(ID_EDITION_MONITOR, OnUpdateEditionMonitor)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LZ77_COMPRESSION, &CMainFrame::OnClientCompressiondefichierLZ77)
	ON_COMMAND(ID_LZ77_DECOMPRESSION, &CMainFrame::OnClientDecompressiondefichierLZ77)

	ON_COMMAND(ID_LZ78_COMPRESSION, &CMainFrame::OnClientCompressiondefichierLZ78)
	ON_COMMAND(ID_LZ78_DECOMPRESSION, &CMainFrame::OnClientDecompressiondefichierLZ78)

	ON_COMMAND(ID_CALCULDECRC, &CMainFrame::OnClientCalculdecrc)
	ON_COMMAND(ID_HAMMING_CODAGE, &CMainFrame::OnClientCodageHamming)
	ON_COMMAND(ID_HAMMING_DECODAGE, &CMainFrame::OnClientDecodageHamming)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_timer_id=0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	m_timer_id=SetTimer(1,100,NULL);
	if (m_wndSplitter.CreateStatic(this,2,1))
	{
		CRect rect;
		GetClientRect(&rect);
		CSize size = rect.Size();
		size.cy-=150;
		if (m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CClientAppView),size,pContext))
		{
			if (m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CChatView),CSize(0,0),pContext))
			{
				SetActiveView((CView*)m_wndSplitter.GetPane(1,0));
				return TRUE;
			}
		}
	}

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnServeurDeconnecter() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->FermetureClient();
}

void CMainFrame::OnServeurConnecter() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->EnregistrementServeur();
}

void CMainFrame::OnServeurInformation() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->AfficherInfoClient();
	
}

void CMainFrame::OnServeurPingServeur()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->PingServeur();
	
}


void CMainFrame::OnServeurListeClients() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->ListeClients();
	
}

void CMainFrame::OnClientDemandedeconnection() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->ConnectionClient();
}


void CMainFrame::OnClientTransfertdefichier() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->TransfertFichier();
	
}

void CMainFrame::OnClientFermetureconnection() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->FermetureConnection();
	
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	
	CFrameWnd::OnTimer(nIDEvent);

	((CClientAppDoc*)GetActiveDocument())->m_pclient->ReceivePacket();
}

BOOL CMainFrame::DestroyWindow() 
{
	if(m_timer_id)
		KillTimer(m_timer_id);

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnEditionMonitor() 
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->m_monitor=(((CClientAppDoc*)GetActiveDocument())->m_pclient->m_monitor?0:1);
	
}

void CMainFrame::OnUpdateEditionMonitor(CCmdUI* pCmdUI) 
{
	 pCmdUI->SetCheck(((CClientAppDoc*)GetActiveDocument())->m_pclient->m_monitor);
	
}

void CMainFrame::OnClientCompressiondefichierLZ77()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->CompressionFichierLZ77();
}

void CMainFrame::OnClientDecompressiondefichierLZ77()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->DecompressionFichierLZ77();
}

void CMainFrame::OnClientCompressiondefichierLZ78()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->CompressionFichierLZ78();
}

void CMainFrame::OnClientDecompressiondefichierLZ78()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->DecompressionFichierLZ78();
}

void CMainFrame::OnClientCalculdecrc()
{
	((CClientAppDoc*)GetActiveDocument())->m_pclient->CalculCRC();
}

void CMainFrame::OnClientCodageHamming()
{
	//((CClientAppDoc*)GetActiveDocument())->m_pclient->EncryptionFichier();
}
void CMainFrame::OnClientDecodageHamming()
{
	//((CClientAppDoc*)GetActiveDocument())->m_pclient->DecryptionFichier();
}