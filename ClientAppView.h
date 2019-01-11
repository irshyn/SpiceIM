// ClientAppView.h : interface of the CClientAppView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTAPPVIEW_H__A8B3440E_2B33_41BA_878E_23EC7078C091__INCLUDED_)
#define AFX_CLIENTAPPVIEW_H__A8B3440E_2B33_41BA_878E_23EC7078C091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CClientAppView : public CEditView
{
protected: // create from serialization only
	CClientAppView();
	DECLARE_DYNCREATE(CClientAppView)

// Attributes
public:
	CClientAppDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientAppView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClientAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Message(LPCTSTR lpszMessage);
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CClientAppView)
	afx_msg void OnEditionNettoyer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ClientAppView.cpp
inline CClientAppDoc* CClientAppView::GetDocument()
   { return (CClientAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTAPPVIEW_H__A8B3440E_2B33_41BA_878E_23EC7078C091__INCLUDED_)
