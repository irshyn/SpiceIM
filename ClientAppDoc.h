// ClientAppDoc.h : interface of the CClientAppDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTAPPDOC_H__913E3918_C31B_471C_9C2C_5D2CA2C4AA5D__INCLUDED_)
#define AFX_CLIENTAPPDOC_H__913E3918_C31B_471C_9C2C_5D2CA2C4AA5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Client.h"

class CClientAppDoc : public CDocument
{
protected: // create from serialization only
	CClientAppDoc();
	DECLARE_DYNCREATE(CClientAppDoc)

// Attributes
public:
	Client *m_pclient;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientAppDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClientAppDoc();
	void DisplayMsg(LPCTSTR lpszText);
	void ActivateChatView(BOOL state);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CClientAppDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTAPPDOC_H__913E3918_C31B_471C_9C2C_5D2CA2C4AA5D__INCLUDED_)
