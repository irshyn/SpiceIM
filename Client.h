#if !defined(AFX_CLIENT_H__2E481AD7_B65A_439D_91CB_2DEA4A9998C0__INCLUDED_)
#define AFX_CLIENT_H__2E481AD7_B65A_439D_91CB_2DEA4A9998C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Client.h : header file
//

#include "Defs.h"
#include <vector>
using namespace std;

#define TX_IDLE		0
#define TX_BUSY		1
#define IDLE		0
#define	REQ			1
#define ACTIVE		2
#define ACTIVE_TX	3
#define ACTIVE_RX	4

class CClientAppDoc;

/*************************************************************************************************/
//Sous-classe contenant la liste de client et les fonction memebres qui permettent le
// traitement de la liste
class RegistredClient
{
	private:
		CString alias;
		int id;
		int status;
	public:
		RegistredClient(int state, int ClientId, CString ClientAlias){
			status = state;
			id = ClientId;
			alias = ClientAlias;
		}
//fonctions pour obtenir les attributs prives de clientListe
		int getId() {
			return id;
		}
		CString getAlias(){
			return alias;
		}
		int getStatus() {
			return status;
		}
// fonction permettant afficher l'info du client
		CString PrintClient(){
			CString msg;

			if(getStatus()== 0)
				msg.Format("Alias: %s     Id: %d     Status: idle", getAlias(), getId());
			else
				msg.Format("Alias: %s     Id: %d     Status: busy", getAlias(), getId());
			return (msg);
		}
};

class Client : public CSocket
{
private:
	CClientAppDoc* m_pdocument;

	CString serveur_ipaddress,my_ipaddress;
	UINT serveur_port,my_port;

	CString my_alias, dest_alias;
	int my_id;
	int dest_id;
	int status;

	int chat_status,file_status;

// la liste de clients enregistrees
	vector <RegistredClient> listeClient;
// le fichier pour effectuer l'ecriture ou la lecture (lors de transmission du fichier)
	CFile* file;

	void DisplayMsg(LPCTSTR lpszText);
	void ActivateChatView(BOOL state);
	BOOL DeconnecterClient(void);
	BOOL HandleErrorMsg(unsigned long error_code);

// Attributes
public:
	BOOL m_monitor;

// Operations
public:
	Client(CClientAppDoc* p_document);
	virtual ~Client();

	BOOL EnregistrementServeur(void);
	
	BOOL AfficherInfoClient(void);

	BOOL FermetureClient();

	BOOL ListeClients(void);

	BOOL ConnectionClient(void);

	BOOL TransfertFichier(void);

	BOOL FermetureConnection(void);

	BOOL SendChatMsg(CString txt);

	BOOL ReceivePacket(void);

	BOOL CompressionFichierLZ78(void);

	BOOL DecompressionFichierLZ78(void);
		
	BOOL CompressionFichierLZ77(void);

	BOOL DecompressionFichierLZ77(void);

	BOOL PingServeur(void);

	BOOL CalculCRC(void);

	BOOL CodageHamming(void);

	BOOL DecodageHamming(void);

	// function qui permet de creer un paquet a partir les donnees qui seront utilisees comme les champs (TP1)
	bool SendPacket(unsigned char cmd, unsigned char dest, unsigned char src, unsigned short length, unsigned char* data);
	// fonction qui permet d'afficher la liste de clients
	bool AfficherListeClients(unsigned char* donnees);
	// fonction qui envoie les messages d'erreur au serveur
	bool SignalError (unsigned char dest, unsigned char src, int error);
	// fonction effectuant la lecture d'un caractere dans un fichier a comresser lors de compression LZ77
	bool ReadCharacter (CFile& fichier, unsigned char tableau[], unsigned short& ptr);
	//fonction effectuant l'ecriture d'un jeton lors de compression LZ77
	void SendToken (CFile& Fichier, unsigned short Indice, unsigned short Longueur, unsigned char Car, 
		unsigned short& ptr_lec, unsigned short& ptr_rech, unsigned short& T_rech, unsigned short& N);
	//fonction qui recherche une sequence recurrente dans la fenetre de recherche et retourne la longueur de la premiere sequence trouvee
	unsigned short FindSequence (unsigned char T [], unsigned short& ptr, unsigned short deb_lec, unsigned short fin_lec);
	// fonction qui restitue la sequence encode en jeton
	unsigned short RestituerSequence (unsigned char* jeton, unsigned short& indice, unsigned char& caractere);
	
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Client)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(Client)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDialog_Info_Connect dialog

class CDialog_Info_Connect : public CDialog
{
// Construction
public:
	CDialog_Info_Connect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Info_Connect)
	enum { IDD = IDD_DIALOG_CONNECT };
	UINT	m_port;
	CString	m_alias;
	CString	m_ipaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Info_Connect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Info_Connect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// DialogInfo dialog

class DialogInfo : public CDialog
{
// Construction
public:
	DialogInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DialogInfo)
	enum { IDD = IDD_DIALOG_INFO };
	CString	m_client_alias;
	CString	m_client_id;
	CString	m_client_ip;
	CString	m_client_port;
	CString	m_connect_alias;
	CString	m_conn_id;
	CString	m_conn_status;
	CString	m_serveur_ip;
	CString	m_serveur_port;
	CString	m_statut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DialogInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDialogConnectClient dialog

class CDialogConnectClient : public CDialog
{
// Construction
public:
	CDialogConnectClient(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogConnectClient)
	enum { IDD = IDD_DIALOG_CONNECT_CLIENT };
	CString	m_dest_alias;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConnectClient)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConnectClient)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__2E481AD7_B65A_439D_91CB_2DEA4A9998C0__INCLUDED_)
