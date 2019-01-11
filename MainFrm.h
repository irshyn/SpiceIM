// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__536ABE58_348C_42FF_9136_D33FA2C86BE4__INCLUDED_)
#define AFX_MAINFRM_H__536ABE58_348C_42FF_9136_D33FA2C86BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
private:
	int m_timer_id;
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnServeurDeconnecter();
	afx_msg void OnServeurConnecter();
	afx_msg void OnServeurInformation();
	afx_msg void OnServeurListeClients();
	afx_msg void OnServeurPingServeur();
	afx_msg void OnClientDemandedeconnection();
	afx_msg void OnClientTransfertdefichier();
	afx_msg void OnClientFermetureconnection();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEditionMonitor();
	afx_msg void OnUpdateEditionMonitor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClientCompressiondefichierLZ78();
	afx_msg void OnClientDecompressiondefichierLZ78();
	afx_msg void OnClientCompressiondefichierLZ77();
	afx_msg void OnClientDecompressiondefichierLZ77();
	afx_msg void OnClientCalculdecrc();
	afx_msg void OnClientCodageHamming();
	afx_msg void OnClientDecodageHamming();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__536ABE58_348C_42FF_9136_D33FA2C86BE4__INCLUDED_)
