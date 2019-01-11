// Client.cpp : implementation file
//

#include "stdafx.h"
#include "ClientApp.h"
#include "Client.h"
#include "ClientAppDoc.h"
#include "Defs.h"
#include "CDERR.H"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* Voici un exemple de code pour permettre d'afficher des messages avec une boîte interactive ou
// dans la fenêtre de l'application

// Affichagee d'un boîte de message

CString msg;
int file_length=1000;
char file_name[64]="fichier.txt";

// La méthode Format de la classe CString est similaire à la fonction printf
msg.Format("Acceptez-vous le transfert du fichier %s (Size: %d Bytes)?",file_name,file_length);
	
AfxMessageBox(msg);

// Affichage d'une boîte interactive Oui/Non
if(AfxMessageBox(msg,MB_YESNO)==IDYES) ;
	// Faire quelque chose pour oui
else ;
	// Faire quelque chose pour non

// Affichage dans la fenêtre d'application
DisplayMsg(msg);
*/

/////////////////////////////////////////////////////////////////////////////
// Client

Client::Client(CClientAppDoc* p_document)
{
	m_pdocument=p_document;
	m_monitor=0;

	status=CLIENT_NOT_CONNECTED;
	my_id=-1;
	dest_id=-1;
	chat_status=IDLE;
	file_status=IDLE;
	my_alias=_T("");
	dest_alias=_T("");

	ActivateChatView(FALSE);
}

Client::~Client()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(Client, CSocket)
	//{{AFX_MSG_MAP(Client)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////
// Client member functions //
/////////////////////////////

// Fonction appelée du GUI pour enregistrer le client au serveur
BOOL Client::EnregistrementServeur(void) {
	if (status == CLIENT_REGISTERED || status == CLIENT_BUSY) {
		DisplayMsg("You are already registred");
		return FALSE;
	}
	
	CDialog_Info_Connect Dialog;

	if (Dialog.DoModal() == IDOK)
	{
		// Récupération des données de la boîte de dialogue
		serveur_ipaddress=Dialog.m_ipaddress;	//adresse IP
		serveur_port=Dialog.m_port;				//le port
	} else
		return TRUE;

	const char *data;
	data=Dialog.m_alias;
	my_alias=Dialog.m_alias;

	// si le socket n'a ete cree, on affiche le msg d'errerur approprie
	if (Client::Create() == false ) {
		DisplayMsg("Creation du socket impossible");
		return FALSE;
	}
// si on n'a pas pu etablir une connexion TCP avec le serveur, on ferme le socket et affiche le msg d'erreur
	if (Client::Connect(serveur_ipaddress,serveur_port)== false ){
		Client::Close();
		DisplayMsg("Connexion au serveur impossible");
		return FALSE;
	}
// si le socket a bien ete cree et la connexion a ete etablie, on recupere l'info sur le socket cree,
	GetSockName(my_ipaddress,my_port);
//change le statut pour CLIENT_CONNECTED,
	status = CLIENT_CONNECTED;
// cree un paquet de la demande d'enregistrement
	SendPacket(REQ_CLIENT_REG,(unsigned char)0,(unsigned char)0, my_alias.GetLength()+1,(unsigned char*)data);
	return TRUE;	
}

// Fonction appelée du GUI pour déconnecter le client du serveur
BOOL Client::FermetureClient(void) {
	// si une connexion avec le serveur existe,
	if (status != CLIENT_NOT_CONNECTED){
// on ferme la connexion avec un autre client (s'il y en a lieu),
		FermetureConnection();
// on envoie la commande de fermeture de connexion au serveur
		SendPacket(IND_CLIENT_CLOSE, 0 ,(unsigned char) my_id  , 0, NULL );
//le msg d'envoie de la commande de fermeture est affiche
		DisplayMsg(_T("Deconnection du serveur est accomplie"));
// et on ferme le socket TCP
		DeconnecterClient();
	}
	else {
// si la connexion avec le serveur qu'on veut fermer n'existe pas, on affiche le message signalant cette situation
		DisplayMsg(_T("La connexion au serveur n'existe pas"));
//et on ferme le socket
		Close();
	}
	return TRUE;
}

BOOL Client:: PingServeur(void) {//ne rien faire ici
	return TRUE;
}

// Fonction appelée du GUI pour afficher les informations du client
BOOL Client::AfficherInfoClient(void) {
	DialogInfo Dialog;

	// Vous devez maintenir adéquatement les variables requises
	// pour afficher les informations sur le client

	switch(status) {
	case CLIENT_NOT_CONNECTED:
		Dialog.m_statut=_T("Non-Connecté");
		Dialog.m_conn_status=_T("Non-Connecté");
		break;
	case CLIENT_CONNECTED:
		Dialog.m_statut=_T("Connecté");
		Dialog.m_conn_status=_T("Non-Connecté");
		Dialog.m_client_ip=my_ipaddress;
		Dialog.m_client_port.Format("%d",my_port);
		Dialog.m_serveur_ip=serveur_ipaddress;
		Dialog.m_serveur_port.Format("%d",serveur_port);		
		break;
	case CLIENT_REGISTERED:
		Dialog.m_statut=_T("Enregistré");
		Dialog.m_conn_status=_T("Non-Connecté");
		Dialog.m_client_ip=my_ipaddress;
		Dialog.m_client_port.Format("%d",my_port);
		Dialog.m_serveur_ip=serveur_ipaddress;
		Dialog.m_serveur_port.Format("%d",serveur_port);
		Dialog.m_client_id.Format("%d",my_id);
		Dialog.m_client_alias=my_alias;
		break;
	case CLIENT_BUSY:
		Dialog.m_statut=_T("Enregistré");
		Dialog.m_conn_status=_T("Connecté");
		if(chat_status==ACTIVE)
			Dialog.m_conn_status+=_T(" - Active chat");
		if(file_status>=ACTIVE)
			Dialog.m_conn_status+=_T(" - Active file transfer");
		Dialog.m_client_ip=my_ipaddress;
		Dialog.m_client_port.Format("%d",my_port);
		Dialog.m_serveur_ip=serveur_ipaddress;
		Dialog.m_serveur_port.Format("%d",serveur_port);
		Dialog.m_client_id.Format("%d",my_id);
		Dialog.m_client_alias=my_alias;
		Dialog.m_connect_alias=dest_alias;
		Dialog.m_conn_id.Format("%d",dest_id);
		break;
	default:
		Dialog.m_statut=_T("Unknown");
		break;
	}

	Dialog.DoModal();

	return TRUE;

}

// Fonction appelée du GUI pour obtenir la liste des clients connectés au serveur
BOOL Client::ListeClients(void) {
	if (status == CLIENT_NOT_CONNECTED){
		DisplayMsg(_T("Not registered: Can not display list of clients."));
	}
	SendPacket (REQ_CLIENT_LIST, (unsigned char )0, (unsigned char ) my_id, 0, NULL);
	listeClient.clear();
	return TRUE;
}
// Fonction appelée du GUI pour se connecter à un autre client
BOOL Client::ConnectionClient(void) {
	// si le status de client signale que le client est deja en session de communication, afficher un msg approprie
	if (status == CLIENT_BUSY) {
		AfxMessageBox("Already connected");
		return false;
	}
	// si la liste de client est vide, afficher le msg demandant faire la requete de la liste des clients
	if(listeClient.size() == 0) {
		DisplayMsg(_T("Afficher d'abord la liste des clients connectes"));
		return false;
	}	
// on demande l'usager d'entrer le nom d'alias qu'il veut joindre
	CDialogConnectClient Dialog;
	if(Dialog.DoModal() == IDOK) { //boîte de dialogue pour récupérer l'alisas du destinataire, préalablement vous devez avoir obtenu la liste des clients
		dest_alias=Dialog.m_dest_alias;
	}
	if (dest_alias == my_alias){
		DisplayMsg(_T("Cannot connect with yourself"));
		return false;
	}
// et on cherche cet alias dans la liste des clients
	bool AliasFound = false;
	for(unsigned int i=0; i < listeClient.size(); i++) {
		if(listeClient[i].getAlias()== dest_alias){
			dest_id= listeClient[i].getId();
			AliasFound = true;
		}
	}
// si l'alias a ete trouve, on envoie une requete de connexion a l'autre client
	if(AliasFound == true){		
		SendPacket(REQ_CONNECTION,(unsigned char)dest_id, (unsigned char)my_id, my_alias.GetLength()+1,(unsigned char*) my_alias.GetString());
		DisplayMsg(_T("Sending connection request"));
		return TRUE;
		}
// sinon, un message d'erreur est affiche
	else {
		AfxMessageBox("This client is not in the list");
		dest_alias = "";
		return FALSE;
	}
}

// Fonction appelée du GUI pour transférrer un fichier à un autre client
BOOL Client::TransfertFichier(void) {
	if (file_status == BUSY_FLAG){
		DisplayMsg(_T("You are already transferring a file "));
		return  FALSE;

	}
	if(status != CLIENT_BUSY) {
		DisplayMsg(_T("You are not connected to another client"));
		return  FALSE;
	}
	OPENFILENAME	tx_file;							// code permettant de récupérer le nom du fichier à transmettre
	_TCHAR file_filter[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename[256]=_T("\0");
	_TCHAR filetitle[256]=_T("\0");
	_TCHAR strTitle[50] = _T("Ouvrir fichier à transférrer");

	tx_file.lStructSize = sizeof (OPENFILENAME);
	tx_file.hwndOwner = NULL;
	tx_file.lpstrFilter = file_filter;
	tx_file.lpstrCustomFilter = NULL;
	tx_file.nMaxCustFilter = 0L;
	tx_file.nFilterIndex = 1L;
	tx_file.lpstrFile = filename;						// tx_file.lpstrFile contient le nom du fichier avec le répertoire
	tx_file.nMaxFile = 256;
	tx_file.lpstrFileTitle = filetitle;					// tx_file.lpstrFileTitle contient le nom du fichier sans le répertoire
	tx_file.nMaxFileTitle = 256;
	tx_file.lpstrInitialDir = NULL;
	tx_file.lpstrTitle = strTitle;
	tx_file.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
	tx_file.lpstrDefExt = NULL;

	if (GetOpenFileName(&tx_file) == 0) {
		if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
			AfxMessageBox(_T("Fichier à transférrer invalide"));
		return FALSE;
	}
// on ouvre le fichier pour le transfert (en mode de lecture)
	file = new CFile;
	if(!file->Open(tx_file.lpstrFile,CFile::modeRead|CFile::shareExclusive)) {
			AfxMessageBox(_T("ICould not open the file to transfer"));
			delete file;
			return FALSE;
	}
	// on recupere le nom du fichier
	CString FileName = tx_file.lpstrFileTitle;
	// la longueur du fichier
	unsigned long file_length = file->GetLength();
	// la longueur du fichier sera transmise en premier 4 octets du champs data du paquet de requete
	unsigned char* data = new unsigned char[FileName.GetLength()+5];
	data[0]=(file_length&0xff000000)>>24;
	data[1]=(file_length&0x00ff0000)>>16;
	data[2]=(file_length&0x0000ff00)>>8;
	data[3]=(file_length&0x000000ff);
	memcpy(&data[4], (char*)(tx_file.lpstrFileTitle), FileName.GetLength()+1);
	// on envoie une requete de transmission d'un fichier
	SendPacket(REQ_FILE_TRANSFER,(unsigned char )dest_id,(unsigned char)my_id, FileName.GetLength()+5, data);
	delete [] data;
	return TRUE;
}
// Fonction appelée du GUI pour terminer une connexion avec un autre client 
BOOL Client::FermetureConnection(void) {
	// si l'usager a une session de communication avec qqn,
	if(status == CLIENT_BUSY)
	{
	// on envoie la demande de fermer la session a l'autre personne,
		SendPacket(REQ_CONNECTION_CLOSE,(unsigned char)dest_id, (unsigned char) my_id, 0, NULL );
		dest_alias = "";
		dest_id = -1;
	// on change le status et envoie la demande de changement de status au serveur
		status = CLIENT_REGISTERED;
		unsigned char a = 0;
		SendPacket(REQ_CLIENT_UPDATE_STATUS, (unsigned char) 0, (unsigned char) my_id,1, &(a));
		DisplayMsg("Connexion shut down");
		ActivateChatView(false);
	}
	return TRUE;
}

//*****************************************************************************************************//
//*****************************************************************************************************//
//*										TP4: Encryption du fichier								*//
//*****************************************************************************************************//
//*****************************************************************************************************//
BOOL Client::CompressionFichierLZ78(void) {
	OPENFILENAME	orig_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier à LIRE
	_TCHAR file_filter[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename[256]=_T("\0");
	_TCHAR filetitle[256]=_T("\0");
	_TCHAR strTitle[50] = _T("Ouvrir fichier à encrypter");

	orig_file.lStructSize = sizeof (OPENFILENAME);
	orig_file.hwndOwner = NULL;
	orig_file.lpstrFilter = file_filter;
	orig_file.lpstrCustomFilter = NULL;
	orig_file.nMaxCustFilter = 0L;
	orig_file.nFilterIndex = 1L;
	orig_file.lpstrFile = filename;  // with entire path
	orig_file.nMaxFile = 256;
	orig_file.lpstrFileTitle = filetitle;  // w/o path
	orig_file.nMaxFileTitle = 256;
	orig_file.lpstrInitialDir = NULL;
	orig_file.lpstrTitle = strTitle;
	orig_file.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
	orig_file.lpstrDefExt = NULL;

	if (GetOpenFileName(&orig_file) == 0) {
		if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
			AfxMessageBox(_T("Fichier à compresser invalide"));
		return FALSE;
	}
	
	CFile infile;
	
	if(!infile.Open(orig_file.lpstrFile,CFile::modeRead|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier à compresser"));		
		return FALSE;
	}

	OPENFILENAME	wr_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier pour l'ÉCRITURE
	_TCHAR file_filter2[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename2[256]=_T("\0");
	_TCHAR filetitle2[256]=_T("\0");
	_TCHAR strTitle2[50] = _T("Nom du fichier à sauvegarder");

	wr_file.lStructSize = sizeof (OPENFILENAME);
	wr_file.hwndOwner = NULL;
	wr_file.lpstrFilter = file_filter2;
	wr_file.lpstrCustomFilter = NULL;
	wr_file.nMaxCustFilter = 0L;
	wr_file.nFilterIndex = 1L;
	wr_file.lpstrFile = filename2;  // with entire path
	wr_file.nMaxFile = 256;
	wr_file.lpstrFileTitle = filetitle2;  // w/o path
	wr_file.nMaxFileTitle = 256;
	wr_file.lpstrInitialDir = NULL;
	wr_file.lpstrTitle = strTitle2;
	wr_file.Flags = 0;
	wr_file.lpstrDefExt = NULL;

	if (GetSaveFileName(&wr_file) == 0) {
		AfxMessageBox(_T("Fichier à sauvegarder invalide"));
		infile.Close();		
		return FALSE;
	}

	CFile outfile;
	
	if(!outfile.Open(wr_file.lpstrFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier pour écriture"));
		infile.Close();
		return FALSE;
	}
/***************************** Algorithme d'encryption ********************************************************/
	unsigned long* key = new unsigned long [4];			// le cle generatrice a partir de laquelle on generera les bits
	key[3] = 0x01;
	key[2]=0;
	key[1]=0;
	key[0]=0x42000000;
	CString msg;
	unsigned short L;									// le nombre des octets reelement lus
	unsigned char* OctetLu = new unsigned char;			// le buffer pour lecture du fichier
	unsigned char Octet;
	unsigned long bit;
	unsigned char sortie;

	outfile.Write(key, 16);						// on ecrit le polynom generateur dans le fichier encrypte

	L = infile.Read(OctetLu,1);
	while (L != 0){
		for (int j = 7; j>=0 ;j--) {
			sortie = (key[3]&0x01);
			bit =((key[0]>>30)&0x01)^((key[0]>>25)&0x01)^sortie;
			for (int i = 3; i>0; i--){
				*(key+i) = ((*(key+i)>>1)&0x7fffffff)|((*(key+i-1)<<31)&0x80000000);
			}
			*(key) = ((*(key)>>1)&0x7fffffff)|((bit<<31)&0x80000000);
			bit = (bit<<j);
			Octet = *(OctetLu);
			Octet = Octet^bit;
			*(OctetLu) = Octet;	
		}
		outfile.Write(OctetLu,1);
		L = infile.Read(OctetLu,1);
	}
	DisplayMsg("File encrypted");
	infile.Close();
	outfile.Close();
	delete OctetLu;
	delete [] key;
	return TRUE;
}

//*****************************************************************************************************//
//*****************************************************************************************************//
//*										TP4: Decryption du fichier									*//
//*****************************************************************************************************//
//*****************************************************************************************************//
BOOL Client::DecompressionFichierLZ78(void) {	
OPENFILENAME	orig_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier à LIRE
	_TCHAR file_filter[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename[256]=_T("\0");
	_TCHAR filetitle[256]=_T("\0");
	_TCHAR strTitle[50] = _T("Ouvrir fichier à decrypter");

	orig_file.lStructSize = sizeof (OPENFILENAME);
	orig_file.hwndOwner = NULL;
	orig_file.lpstrFilter = file_filter;
	orig_file.lpstrCustomFilter = NULL;
	orig_file.nMaxCustFilter = 0L;
	orig_file.nFilterIndex = 1L;
	orig_file.lpstrFile = filename;  // with entire path
	orig_file.nMaxFile = 256;
	orig_file.lpstrFileTitle = filetitle;  // w/o path
	orig_file.nMaxFileTitle = 256;
	orig_file.lpstrInitialDir = NULL;
	orig_file.lpstrTitle = strTitle;
	orig_file.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
	orig_file.lpstrDefExt = NULL;

	if (GetOpenFileName(&orig_file) == 0) {
		if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
			AfxMessageBox(_T("Fichier à decrypter invalide"));
		return FALSE;
	}
	
	CFile infile;
	
	if(!infile.Open(orig_file.lpstrFile,CFile::modeRead|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier à decrypter"));		
		return FALSE;
	}

	OPENFILENAME	wr_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier pour l'ÉCRITURE
	_TCHAR file_filter2[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename2[256]=_T("\0");
	_TCHAR filetitle2[256]=_T("\0");
	_TCHAR strTitle2[50] = _T("Nom du fichier decrypte à sauvegarder");

	wr_file.lStructSize = sizeof (OPENFILENAME);
	wr_file.hwndOwner = NULL;
	wr_file.lpstrFilter = file_filter2;
	wr_file.lpstrCustomFilter = NULL;
	wr_file.nMaxCustFilter = 0L;
	wr_file.nFilterIndex = 1L;
	wr_file.lpstrFile = filename2;  // with entire path
	wr_file.nMaxFile = 256;
	wr_file.lpstrFileTitle = filetitle2;  // w/o path
	wr_file.nMaxFileTitle = 256;
	wr_file.lpstrInitialDir = NULL;
	wr_file.lpstrTitle = strTitle2;
	wr_file.Flags = 0;
	wr_file.lpstrDefExt = NULL;

	if (GetSaveFileName(&wr_file) == 0) {
		AfxMessageBox(_T("Fichier decrypte à sauvegarder invalide"));
		infile.Close();		
		return FALSE;
	}

	CFile outfile;
	
	if(!outfile.Open(wr_file.lpstrFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier pour ecriture"));
		infile.Close();
		return FALSE;
	}

/***************************** Algorithme de decryption ********************************************************/

	unsigned long* key = new unsigned long [4];			// le cle generatrice a partir de laquelle on generera les bits
	key[3] =0;
	key[2]=0;
	key[1]=0;
	key[0]=0;
	CString msg;
	unsigned short L;									// le nombre des octets reelement lus
	unsigned char* OctetLu = new unsigned char;			// le buffer pour lecture du fichier
	unsigned char Octet;
	unsigned long bit;
	unsigned char sortie;

	for (int i=0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			L= infile.Read(OctetLu, 1);
			*(key+i)=((unsigned long)*(OctetLu) << 8*j)|*(key+i);
		}
	}
	L=infile.Read(OctetLu, 1);
	while (L != 0){
		for (int j = 7; j>=0 ;j--) {
			sortie = (key[3]&0x01);
			bit =((key[0]>>30)&0x01)^((key[0]>>25)&0x01)^sortie;
			for (int i = 3; i>0; i--){
				*(key+i) = ((*(key+i)>>1)&0x7fffffff)|((*(key+i-1)<<31)&0x80000000);
			}
			*(key) = ((*(key)>>1)&0x7fffffff)|((bit<<31)&0x80000000);
			bit = (bit<<j);
			Octet = *(OctetLu);
			Octet = Octet^bit;
			*(OctetLu) = Octet;	
		}
		outfile.Write(OctetLu,1);
		L = infile.Read(OctetLu,1);
	}
	DisplayMsg("File decrypted");
	infile.Close();
	outfile.Close();
	delete OctetLu;
	return TRUE;
}
//*****************************************************************************************************//
//*****************************************************************************************************//
//*				TP3: Compression du fichier en utilisant la methode LZ77							*//
//*****************************************************************************************************//
//*****************************************************************************************************//
BOOL Client::CompressionFichierLZ77(void) {
	OPENFILENAME	orig_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier à LIRE
	_TCHAR file_filter[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename[256]=_T("\0");
	_TCHAR filetitle[256]=_T("\0");
	_TCHAR strTitle[50] = _T("Ouvrir fichier à compresser");

	orig_file.lStructSize = sizeof (OPENFILENAME);
	orig_file.hwndOwner = NULL;
	orig_file.lpstrFilter = file_filter;
	orig_file.lpstrCustomFilter = NULL;
	orig_file.nMaxCustFilter = 0L;
	orig_file.nFilterIndex = 1L;
	orig_file.lpstrFile = filename;  // with entire path
	orig_file.nMaxFile = 256;
	orig_file.lpstrFileTitle = filetitle;  // w/o path
	orig_file.nMaxFileTitle = 256;
	orig_file.lpstrInitialDir = NULL;
	orig_file.lpstrTitle = strTitle;
	orig_file.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
	orig_file.lpstrDefExt = NULL;

	if (GetOpenFileName(&orig_file) == 0) {
		if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
			AfxMessageBox(_T("Fichier à compresser invalide"));
		return FALSE;
	}
	
	CFile infile;
	
	if(!infile.Open(orig_file.lpstrFile,CFile::modeRead|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier à compresser"));		
		return FALSE;
	}

	OPENFILENAME	wr_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier pour l'ÉCRITURE
	_TCHAR file_filter2[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename2[256]=_T("\0");
	_TCHAR filetitle2[256]=_T("\0");
	_TCHAR strTitle2[50] = _T("Nom du fichier à sauvegarder");

	wr_file.lStructSize = sizeof (OPENFILENAME);
	wr_file.hwndOwner = NULL;
	wr_file.lpstrFilter = file_filter2;
	wr_file.lpstrCustomFilter = NULL;
	wr_file.nMaxCustFilter = 0L;
	wr_file.nFilterIndex = 1L;
	wr_file.lpstrFile = filename2;  // with entire path
	wr_file.nMaxFile = 256;
	wr_file.lpstrFileTitle = filetitle2;  // w/o path
	wr_file.nMaxFileTitle = 256;
	wr_file.lpstrInitialDir = NULL;
	wr_file.lpstrTitle = strTitle2;
	wr_file.Flags = 0;
	wr_file.lpstrDefExt = NULL;

	if (GetSaveFileName(&wr_file) == 0) {
		AfxMessageBox(_T("Fichier à sauvegarder invalide"));
		infile.Close();		
		return FALSE;
	}

	CFile outfile;
	
	if(!outfile.Open(wr_file.lpstrFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier pour écriture"));
		infile.Close();
		return FALSE;
	}
/***************************** Algorithme de comression ********************************************************/
unsigned char T [2079];					// tableau contenant les fenetres de recherche et de lecture (2047+32=2079)
	unsigned short T_rech = 0;		// la largeur de la fenetre de recherche
	unsigned short ptr_rech = 0;	// pointeur de debut de la fenetre de recherche
	unsigned short deb_lec = 0;		// pointeur de debut de la fenetre de lecture
	unsigned short fin_lec = 0;		// pointeur de fin de la fenetre de lecture
	unsigned short L = 0;			// le nombre d'octets actuellement lus (=0 quand le fichier est parcouru)
	unsigned short deb_n_seq = 0;	// pointeur de debut de recherche de la sequence (une fois que la sequence trouvee,
									//il prend la valeur de debut de cette sequence (voir la foncion RechercherSequence))
	unsigned short longueur_n_seq = 0;//la valeur de longueur de la sequence trouvee
	unsigned short debut_seq =0;	// pointeur de debut de la meilleure sequence trouvee
	unsigned short longueur_seq=0;	// la longueur de la meilleure sequence trouvee
	unsigned short NbrCmr = 0;		// le compteur de caracteres compresses
	unsigned short SizeDico = 0;	// la taille du fichier compresse

	CString msg;

// on rempli la fenetre de lecture avec 32 caracteres du teste lu
	for (int i=0; i<32; i++){
		ReadCharacter (infile, T, fin_lec);
	}	
// puisque la fenetre de recherche est vide, on envoie le premier caractere sans chercher la sequence recurrente
	SendToken (outfile, 0, 0, T[deb_lec], deb_lec, ptr_rech, T_rech, SizeDico);
	NbrCmr=NbrCmr+1;
// autant que le ponteur de debut de lecture n'est pas egal au pointeur de la fin de lecture (ce qui correspond a la fin de comression 
// de tous les caracteres dans la fenetre de lecture)
	while (NbrCmr < infile.GetLength()){
		deb_n_seq = ptr_rech; // le pointeur de debut de recherche de sequence est egal au debut de la fenetre de recherche
		longueur_n_seq = FindSequence(T, deb_n_seq, deb_lec, fin_lec);	// et on effectue la recherche d'une sequence
// si une sequence recurrente n'a pas ete trouvee
		if (longueur_n_seq == 0){
			SendToken (outfile, 0, 0, T[deb_lec], deb_lec, ptr_rech, T_rech, SizeDico); //on envoie le jeton et ajuste les pointeurs
			NbrCmr = NbrCmr+1;
			ReadCharacter (infile, T, fin_lec);						// et on lit le prochain caractere dans le fichier
		}
// si on a trouve une sequence recurrente
		else {
//on recherche la sequence la plus longue possible en sauvegardant les meilleurs resultats dans les variables
//intermediares debut_seq et longueur_seq
			debut_seq = deb_n_seq;
			longueur_seq = longueur_n_seq;
			deb_n_seq = (deb_n_seq+1)%2079;
			while (deb_n_seq != deb_lec){
				longueur_n_seq = FindSequence(T, deb_n_seq, deb_lec, fin_lec);
				if (longueur_n_seq != 0){
					if (longueur_n_seq > longueur_seq) {
						longueur_seq = longueur_n_seq;
						debut_seq = deb_n_seq;
					}
					deb_n_seq = (deb_n_seq+1)%2079;
				}
			}
// on envoie le jeton contenant la sequence compressee et reajuste les pointeurs
			if (debut_seq > deb_lec)
				SendToken (outfile, 2079 - (debut_seq-deb_lec), longueur_seq, T[(deb_lec+longueur_seq)%2079], deb_lec, ptr_rech, T_rech, SizeDico);
			else
				SendToken (outfile, deb_lec - debut_seq, longueur_seq, T[(deb_lec+longueur_seq)%2079], deb_lec, ptr_rech, T_rech, SizeDico);
// et on effectue la lecture des (longueur_seq+1) caracteres avec reajustaton des pointeurs
			for (int i=0; i<longueur_seq+1; i++){
				ReadCharacter (infile, T, fin_lec);
			}
			NbrCmr=NbrCmr+longueur_seq+1;
		}
	}
	msg.Format("Original file length: %d Bytes", infile.GetLength());
	DisplayMsg (msg);
	msg.Format("Compressed file length: %d Bytes", SizeDico);
	DisplayMsg (msg);
	DisplayMsg("The file was successfully compressed");
	infile.Close();
	outfile.Close();
	return TRUE;
}

//*****************************************************************************************************//
//*****************************************************************************************************//
//*				TP3: Decompression du fichier en utilisant la methode LZ77							*//
//*****************************************************************************************************//
//*****************************************************************************************************//
BOOL Client::DecompressionFichierLZ77(void) {

OPENFILENAME	orig_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier à LIRE
	_TCHAR file_filter[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename[256]=_T("\0");
	_TCHAR filetitle[256]=_T("\0");
	_TCHAR strTitle[50] = _T("Ouvrir fichier à decrypter");

	orig_file.lStructSize = sizeof (OPENFILENAME);
	orig_file.hwndOwner = NULL;
	orig_file.lpstrFilter = file_filter;
	orig_file.lpstrCustomFilter = NULL;
	orig_file.nMaxCustFilter = 0L;
	orig_file.nFilterIndex = 1L;
	orig_file.lpstrFile = filename;  // with entire path
	orig_file.nMaxFile = 256;
	orig_file.lpstrFileTitle = filetitle;  // w/o path
	orig_file.nMaxFileTitle = 256;
	orig_file.lpstrInitialDir = NULL;
	orig_file.lpstrTitle = strTitle;
	orig_file.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
	orig_file.lpstrDefExt = NULL;

	if (GetOpenFileName(&orig_file) == 0) {
		if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
			AfxMessageBox(_T("Fichier à decrypter invalide"));
		return FALSE;
	}
	
	CFile infile;
	
	if(!infile.Open(orig_file.lpstrFile,CFile::modeRead|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier à decrypter"));		
		return FALSE;
	}

	OPENFILENAME	wr_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier pour l'ÉCRITURE
	_TCHAR file_filter2[256]=_T("All Files\0*.*\0\0");
	_TCHAR filename2[256]=_T("\0");
	_TCHAR filetitle2[256]=_T("\0");
	_TCHAR strTitle2[50] = _T("Nom du fichier decrypte à sauvegarder");

	wr_file.lStructSize = sizeof (OPENFILENAME);
	wr_file.hwndOwner = NULL;
	wr_file.lpstrFilter = file_filter2;
	wr_file.lpstrCustomFilter = NULL;
	wr_file.nMaxCustFilter = 0L;
	wr_file.nFilterIndex = 1L;
	wr_file.lpstrFile = filename2;  // with entire path
	wr_file.nMaxFile = 256;
	wr_file.lpstrFileTitle = filetitle2;  // w/o path
	wr_file.nMaxFileTitle = 256;
	wr_file.lpstrInitialDir = NULL;
	wr_file.lpstrTitle = strTitle2;
	wr_file.Flags = 0;
	wr_file.lpstrDefExt = NULL;

	if (GetSaveFileName(&wr_file) == 0) {
		AfxMessageBox(_T("Fichier decrypte à sauvegarder invalide"));
		infile.Close();		
		return FALSE;
	}

	CFile outfile;
	
	if(!outfile.Open(wr_file.lpstrFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
		AfxMessageBox(_T("Incapable d'ouvrir le fichier pour ecriture"));
		infile.Close();
		return FALSE;
	}

/***************************** Algorithme de decomression ********************************************************/
	unsigned char T [2079];					// tableau de decompression
	unsigned short ptr_ecr = 0;				// pointeur d'ecriture dans le tableau
	unsigned char* buffer = new unsigned char[3];	// le buffer de lecture (on effectue toujours la lecture de trois octets (un jeton))
	unsigned short L = 0;					//le nombre des caracteres reelement lus
	unsigned char* Car = new unsigned char;					// caractere sauvegarde dans le jeton
	unsigned short indice = 0;				//la valeur de l'indice de decalage de la sequence sauvegardee dans le jeton
	unsigned short longueur = 0;			//la valeur de la longueur de la sequence sauvegarde dans le jeton
	unsigned char Caractere;							// caractere sauvegarde dans le jeton
	unsigned short Taille = 0;				//le nombre des caracteres decompresses total

	CString msg;							// pour afficher

// on effectue la lecture de trois octets (un jeton)
	L= infile.Read(buffer,3);
// autant que la fin du fichier n'est pas atteinte,
	while (L!=0){
// est restitue le contenu du jeton
		longueur = RestituerSequence (buffer, indice, Caractere);
// si le jeton contient la sequence,
		if (longueur != 0){
// on restitue la sequnce
			for (unsigned short i = 0; i<longueur; i++) {
				if (ptr_ecr > indice)
					T[ptr_ecr] = T [ptr_ecr-indice];
				else {
					if (Taille <= 2079){}
						//DisplayMsg(_T("Erreur de decompression"));
					else
						T [ptr_ecr] = T[2079 - (indice-ptr_ecr)];
				}
				(*Car) = T [ptr_ecr];
				outfile.Write(Car, 1);
				Taille++;
				ptr_ecr = (ptr_ecr+1)%2079;
			}
		}
// et on restitue le caractere sauvegarde dans le jeton
		T [ptr_ecr] = Caractere;
		(*Car) = Caractere;
		outfile.Write(Car, 1);
		Taille++;
		ptr_ecr = (ptr_ecr+1)%2079;
		L= infile.Read(buffer,3);
	}
	delete buffer, Car;

	msg.Format("Original file length: %d Bytes", infile.GetLength());
	DisplayMsg (msg);
	msg.Format("Decompressed file length: %d Bytes", Taille);
	DisplayMsg (msg);
	DisplayMsg("The file was successfully decompressed");

	infile.Close();
	outfile.Close();
	return TRUE;
}

// Fonction appelée du GUI pour coder un fichier (codage de canal) 
BOOL Client:: CodageHamming(void){
// copier le code fournis dans la fonction CompressionFichierLZ77() pour les boites de dialogue
/**********************************************
	VOTRE ALGORITHME DE CODAGE   (TP4)
**********************************************/
	return TRUE;
}

// Fonction appelée du GUI pour décoder un fichier (décodage de canal)
BOOL Client:: DecodageHamming(void){
// copier le code fournis dans la fonction CompressionFichierLZ77() pour afficher les boites de dialogue
/**********************************************
	VOTRE ALGORITHME DE DÉCODAGE  (TP4)
**********************************************/	
	return TRUE;
}

// Fonction appelée du GUI pour calculer localement le CRC d'un fichier
BOOL Client::CalculCRC(void) {	
/**********************************************
	     NE RIEN FAIRE DANS CETTE FCT
**********************************************/	
	return TRUE;
}

// Fonction appelée du GUI pour envoyer un message chat et pour l'afficher dans
// la fenêtre principale
BOOL Client::SendChatMsg(CString txt) {
	CString msg;
	msg+=my_alias+":  ";
	msg+=txt;
	// ce message sera affiche dans la fenetre superieure
	DisplayMsg(msg);
	//on envoie le paquet contenant ce message au client B
	SendPacket(CHAT_DATA,(unsigned char)dest_id, (unsigned char)my_id, (unsigned char) txt.GetLength()+1, (unsigned char *)txt.GetString());
	return TRUE;
}

// Fonction vous permettant d'afficher les messages d'erreur provenant du serveur où bien du client 
// complet disponible sur le site web 
BOOL Client::HandleErrorMsg(unsigned long error_code) {
	switch(error_code) {
		case UNKNOWN_COMMAND:
			DisplayMsg(_T("Error message received: UNKNOWN_COMMAND"));
			break;
		case UNEXPECTED_COMMAND:
			DisplayMsg(_T("Error message received: UNEXPECTED_COMMAND"));
			break;
		case ERROR_IN_MSG_FORMAT:
			DisplayMsg(_T("Error message received: ERROR_IN_MSG_FORMAT"));
			break;
		case ALIAS_ALREADY_EXIST:
			DisplayMsg(_T("Error message received: You cannot use already existing alias"));
			break;
		case ALIAS_TOO_LONG:
			DisplayMsg(_T("Error message received: ALIAS_TOO_LONG"));
			break;
		case ALIAS_TOO_SHORT:
			DisplayMsg(_T("Error message received: ALIAS_TOO_SHORT"));
			break;
		case CLIENT_LIST_FULL:
			DisplayMsg(_T("Error message received: CLIENT_LIST_FULL"));
			break;
		case INVALID_PARAMETERS:
			DisplayMsg(_T("Error message received: INVALID_PARAMETERS"));
			break;
		case CONN_REQ_REJECT:
			DisplayMsg(_T("Error message received: Connection request denied"));
			break;
		case CONN_REQ_REJECT_BUSY:
			DisplayMsg(_T("Error message received: This client is already in session"));
			break;
		case ROUTING_ERROR:
			DisplayMsg(_T("Error message received: ROUTING_ERROR"));
			break;
		case FILE_TRANSFER_REJECT:
			DisplayMsg(_T("Error message received: File transfer denied"));
			break;
		case FILE_TRANSFER_REJECT_BUSY:
			DisplayMsg(_T("Error message received: This client is already transferring a file"));
			break;
		case FILE_TRANSFER_CRC_ERROR:
			DisplayMsg(_T("Error message received: FILE_TRANSFER_CRC_ERROR"));
			break;
		case FILE_TRANSFER_STOP:
			DisplayMsg(_T("Error message received: FILE_TRANSFER_STOP"));
			break;
		case FILE_TRANSFER_ERROR:
			DisplayMsg(_T("Error message received: FILE_TRANSFER_ERROR"));
			break;
		default:
			DisplayMsg(_T("Unknown error code received"));
			break;
	}
	return TRUE;
}


// Fonction du socket: vous ne devriez rien avoir à changer ici
void Client::OnReceive(int nErrorCode) 
{	
	CSocket::OnReceive(nErrorCode);

	ReceivePacket();
}

BOOL Client::ReceivePacket(void) {	
	unsigned long N;

	if(status==CLIENT_NOT_CONNECTED)
		return FALSE;

	while(1) {
		if(!IOCtl(FIONREAD,&N)) {
			return FALSE;
		}
		if(!N) {
			return FALSE;
		}
		
// on recoit un paquet
		unsigned char* paquetRecu = new unsigned char[N];
		Receive(paquetRecu,N);
// et on en extrait les champs de donnees
		unsigned char cmde = paquetRecu[0];
		unsigned char idDest = paquetRecu[1]>>2;
		unsigned char idSrc = ((paquetRecu[1]& 0x03)<< 4 |(paquetRecu[2]& 0xf0)>>4);
		unsigned short longueur = (((unsigned short)paquetRecu[2] & 0x0f)<< 4 )|(unsigned short) paquetRecu[3];
		unsigned char* donnees;
		if (longueur == 0) {
			donnees = NULL;
		}
		else {
			donnees = new unsigned char [longueur];
			memcpy(donnees,&paquetRecu[4], longueur);
		}
		delete [] paquetRecu;
// le string du message affiche
		CString msg;
// a partir du champ de la commande on execute une action commandee
		switch(cmde) {
/***************************************************************************************************************/
// a l'obtention d'une requete de connection
			case REQ_CONNECTION:
			// si on est en session de communication avec un autre client
				if(status == CLIENT_BUSY) {
			// le message d'erreur correspondant sera envoye
					SignalError (idSrc, idDest, CONN_REQ_REJECT_BUSY);
				}
				else {
					CString Alias(&donnees[0]);
			// si on est libre, le message demandant si on veut accepter la connexion proposee
					msg.Format("Do you want to accept a connection request from %s?", Alias);
					//AfxMessageBox(msg);
			// si la reponse est positive, on ajuste les attributs de destination d'usager
					if(AfxMessageBox(msg,MB_YESNO)==IDYES){
						dest_id = (int) idSrc;
						dest_alias = Alias;
			//on envoie le paquet confirmant l'acception de la demande de la connexion au client source
						SendPacket(IND_CONNECTION_ACCEPT,idSrc, idDest, 0, NULL);						 
			// on change le statut
						status = CLIENT_BUSY;
						ActivateChatView(true);
			// et on envoie le msg au serveur demandant de mettre a jour le statut
						unsigned char a=dest_id|0x40;
						SendPacket(REQ_CLIENT_UPDATE_STATUS, 0, idDest, 1, &(a));
						DisplayMsg("Connection established");
					}
			//si on ne veut pas accepter l'invitation
					else {
			// on envoie le message d'erreur approprie
						SignalError (idSrc, idDest, CONN_REQ_REJECT);
					}
				}
				break;
/***************************************************************************************************************/
// a l'obtention d'une demande du transfert d'un fichier
			case REQ_FILE_TRANSFER:
			// si on est en train d'envoyer un autre fichier,
				if(file_status == BUSY_FLAG){
			// le message d'erreur approprie sera envoye
					SignalError (idSrc, idDest, FILE_TRANSFER_REJECT_BUSY);
				}
			// sinon,
				else {
					// on recupere la longueur du fichier a recevoir et son nom
					unsigned long file_size = ((unsigned long )donnees[0] << 24 ) | (unsigned long)donnees[1] << 16 | (unsigned long)donnees[2] << 8 | (unsigned long)donnees[3];
					CString file_name(&donnees[4]);
					// et on demande si l'utilisateur veut accepter le fichier
					msg.Format("Acceptez-vous le transfert du fichier %s (Size: %d Bytes)?", file_name, file_size);
					AfxMessageBox(msg);
					// si l'usager refuse d'accepter le fichier,
					if(AfxMessageBox(msg,MB_YESNO)!=IDYES) {
						// le message d'erreur approprie sera envoye a l'autre client
						SignalError (idSrc, idDest, FILE_TRANSFER_REJECT);
					}
					// si l'usager  decide d'accepter le fichier,
					else {
						OPENFILENAME	wr_file;							//permet d'afficher une boite de dialogue pour récupérer 
														//le nom du fichier pour l'ÉCRITURE
						_TCHAR file_filter2[256]=_T("All Files\0*.*\0\0");
						_TCHAR filename2[256]=_T("\0");
						_TCHAR filetitle2[256]=_T("\0");
						_TCHAR strTitle2[50] = _T("Nom du fichier à sauvegarder");

						wr_file.lStructSize = sizeof (OPENFILENAME);
						wr_file.hwndOwner = NULL;
						wr_file.lpstrFilter = file_filter2;
						wr_file.lpstrCustomFilter = NULL;
						wr_file.nMaxCustFilter = 0L;
						wr_file.nFilterIndex = 1L;
						wr_file.lpstrFile = filename2;  // with entire path
						wr_file.nMaxFile = 256;
						wr_file.lpstrFileTitle = filetitle2;  // w/o path
						wr_file.nMaxFileTitle = 256;
						wr_file.lpstrInitialDir = NULL;
						wr_file.lpstrTitle = strTitle2;
						wr_file.Flags = 0;
						wr_file.lpstrDefExt = NULL;

						if (GetOpenFileName(&wr_file) == 0) {
							if (CommDlgExtendedError() == FNERR_INVALIDFILENAME)
								AfxMessageBox(_T("Fichier à transférrer invalide"));
						return FALSE;
						}

						file = new CFile;
	
						if(!file->Open(wr_file.lpstrFile,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
							AfxMessageBox(_T("Incapable d'ouvrir le fichier pour écriture"));
							delete file;
							return FALSE;
						}
						else {
							SendPacket(IND_FILE_TRANSFER_ACCEPT,idSrc, idDest, 0, NULL);
							file_status = BUSY_FLAG;
							DisplayMsg(" File transfer was accepted ");
						}
					}
				}
				break;
/***************************************************************************************************************/
// a l'obtention d'une requete de fermeture de la session de communication 
			case REQ_CONNECTION_CLOSE:
				if(status == CLIENT_BUSY)
				{
					status = CLIENT_REGISTERED;
					dest_id=-1;
					dest_alias = "";
					unsigned char a = 0;
					SendPacket(REQ_CLIENT_UPDATE_STATUS, (unsigned char) 0, (unsigned char) my_id, 1, &(a));
					DisplayMsg("Connexion shut down");
					ActivateChatView(false);
				}
				break;
/***************************************************************************************************************/
// a l'obtention d'un paquet contenant un message a afficher comme chat
			case CHAT_DATA:
				msg.Format( "%s: %s",dest_alias,donnees);
				DisplayMsg(msg);
				break;
/***************************************************************************************************************/
// a l'obtention d'un paquet contenant un segment du fichier en transfert
			case FILE_DATA:
			// on effectue l'ecriture du data extrait dans le fichier d'ecriture
				file->Write(donnees,longueur);
			// et on envoie un paquet avec l'accusee de reception
				SendPacket(ACK_FILE_DATA,idSrc, idDest, 0, NULL);
				file_status = BUSY_FLAG;
				//DisplayMsg(" Transmission du fichier en cours ");
				break;
/***************************************************************************************************************/
// a l'obtention du paquet signalant qu'une erreur est arrivee
			case IND_CLIENT_ERROR:
				unsigned long error_code;
				error_code = ((unsigned long) donnees[0]<<24 ) | (unsigned long)donnees[1] << 16 | (unsigned long) donnees[2] << 8 | (unsigned long) donnees[3];
				HandleErrorMsg(error_code);
	// si le message d'erreur est venu de la part du serveur, on ferme la connexion avec le serveur 
				if ((int) idSrc == 0){
					FermetureClient();
				}
				else {
		// si le msg d'erreur est en provenance du client avec qui on a une session de communication,
		// cette session sera ferme et les stats seront ajustes					
					FermetureConnection();
				}
				break;
/***************************************************************************************************************/
// a l'obtention de la liste des clients connectes au serveur
			case IND_CLIENT_LIST:
				AfficherListeClients(donnees);
				break;
/***************************************************************************************************************/
// a l'obtention d'une reponse positive a la demande d'etablissement d'une session de communication avec un autre client
			case IND_CONNECTION_ACCEPT:
			// on change le status du client, les attributs de destination (dest_if et dest_alias)
			//et envoie une demende de mise-a-jour de status au serveur
				status = CLIENT_BUSY;
				dest_id = (int) idSrc;
				for(unsigned int i=0; i<listeClient.size(); i++){
					if(listeClient.at(i).getId()==dest_id){
						dest_alias = listeClient.at(i).getAlias();
					}
				}
				unsigned char a;
				a = (dest_id | 0x40);
				SendPacket(REQ_CLIENT_UPDATE_STATUS, (unsigned char ) 0, (unsigned char) my_id, 1, &(a));
				ActivateChatView(true);
				DisplayMsg("Connection request accepted");
				break;
/***************************************************************************************************************/
// a l'obtention d'une reponse positive a la demande de transfert, le premier segment du fichier sera envoye
			case IND_FILE_TRANSFER_ACCEPT:
				unsigned char* buffer;
				buffer = new unsigned char[5];
				int N;
				N = file->Read(buffer,5);			
				SendPacket(FILE_DATA, idSrc, idDest, N, buffer);
				DisplayMsg("File transfer began");
				file_status  = BUSY_FLAG;
				delete [] buffer;
				break;
/***************************************************************************************************************/
// a l'obtention d'une accusee de reception du segment precedent, le segment suivant du fichier sera envoye
			case ACK_FILE_DATA:
				buffer = new unsigned char[5];
				N = file->Read(buffer,5);
				if(N == 0)	{
					SendPacket(IND_FILE_TRANSFER_CLOSE, idSrc, idDest, 0, NULL);
					DisplayMsg("File was transfered completely");
					file_status  = IDLE_FLAG;
					file->Close();
					delete file;
				}
				else {					
					SendPacket(FILE_DATA, idSrc, idDest, N, buffer);
					file_status  = BUSY_FLAG;
				}
				delete [] buffer;
				break;
/***************************************************************************************************************/
// a l'obtention d'une reponse positive a la demande d'enregistrement aupres su serveur
			case IND_REG_ACCEPT:
			// on sauvegarde notre ID et met a jour le status
				my_id = idDest;
				status = CLIENT_REGISTERED;
				DisplayMsg("Registration complete");
				break;
/***************************************************************************************************************/
// a l'obtention d'un paquet signalant la fin du fichier transfere 
			case IND_FILE_TRANSFER_CLOSE:
			// on affiche que la transmission de fichier est terminee est met a jour la statistique
				DisplayMsg("File was succesfully transferred");
				file_status  = IDLE_FLAG;
				file->Close();
				delete file;
				break;
/***************************************************************************************************************/
// a l'obtention de toute autre commande
			default:
				DisplayMsg("Unknown command");		
				break;
		}

	}
	
	return TRUE;
}

// Fonction du socket: vous ne devriez rien avoir à changer ici
void Client::OnClose(int nErrorCode) 
{
	CSocket::OnClose(nErrorCode);

	DeconnecterClient();
}


// Fonction pour fermer gracieusement le socket
BOOL Client::DeconnecterClient(void) {
	ShutDown();
	while(ReceivePacket());
	Close();
	DisplayMsg(_T("Fermeture du socket"));
	// Ajoutez les fonctions que vous jugez nécessaires pour mettre à jour vos variables internes
	status=CLIENT_NOT_CONNECTED;
	my_id=-1;
	dest_id=-1;
	chat_status=IDLE;
	file_status=IDLE;
	my_alias=_T("");
	dest_alias=_T("");

	ActivateChatView(FALSE);

	return TRUE;
}
// Fonction permettant d'afficher du texte dans la fenêtre principale: ne rien changer	
void Client::DisplayMsg(LPCTSTR lpszText) {
	m_pdocument->DisplayMsg(lpszText);
}

// Fonction permettant d'activer ou désactiver la zone d'écriture pour le chat
void Client::ActivateChatView(BOOL state) {
	m_pdocument->ActivateChatView(state);
}

// Codes pour les dialogues: ne rien changer

/////////////////////////////////////////////////////////////////////////////
// CDialog_Info_Connect dialog


CDialog_Info_Connect::CDialog_Info_Connect(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Info_Connect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Info_Connect)
	m_port = 1500;
	m_alias = _T("ele3705");
	m_ipaddress = _T("127.0.0.1");
	//}}AFX_DATA_INIT
}


void CDialog_Info_Connect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Info_Connect)
	DDX_Text(pDX, IDC_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 0, 5000);
	DDX_Text(pDX, IDC_ALIAS, m_alias);
	DDV_MaxChars(pDX, m_alias, 64);
	DDX_Text(pDX, IDC_IPADDRESS, m_ipaddress);
	DDV_MaxChars(pDX, m_ipaddress, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Info_Connect, CDialog)
	//{{AFX_MSG_MAP(CDialog_Info_Connect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Info_Connect message handlers

/////////////////////////////////////////////////////////////////////////////
// DialogInfo dialog


DialogInfo::DialogInfo(CWnd* pParent /*=NULL*/)
	: CDialog(DialogInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogInfo)
	m_client_alias = _T("");
	m_client_id = _T("");
	m_client_ip = _T("");
	m_client_port = _T("");
	m_connect_alias = _T("");
	m_conn_id = _T("");
	m_conn_status = _T("");
	m_serveur_ip = _T("");
	m_serveur_port = _T("");
	m_statut = _T("");
	//}}AFX_DATA_INIT
}


void DialogInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogInfo)
	DDX_Text(pDX, IDC_CLIENT_ALIAS, m_client_alias);
	DDX_Text(pDX, IDC_CLIENT_ID, m_client_id);
	DDX_Text(pDX, IDC_CLIENT_IP, m_client_ip);
	DDX_Text(pDX, IDC_CLIENT_PORT, m_client_port);
	DDX_Text(pDX, IDC_CONN_ALIAS, m_connect_alias);
	DDX_Text(pDX, IDC_CONN_ID, m_conn_id);
	DDX_Text(pDX, IDC_CONN_STATUS, m_conn_status);
	DDX_Text(pDX, IDC_SERVEUR_IP, m_serveur_ip);
	DDX_Text(pDX, IDC_SERVEUR_PORT, m_serveur_port);
	DDX_Text(pDX, IDC_STATUT, m_statut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogInfo, CDialog)
	//{{AFX_MSG_MAP(DialogInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogInfo message handlers


/////////////////////////////////////////////////////////////////////////////
// CDialogConnectClient dialog


CDialogConnectClient::CDialogConnectClient(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConnectClient::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConnectClient)
	m_dest_alias = _T("");
	//}}AFX_DATA_INIT
}


void CDialogConnectClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConnectClient)
	DDX_Text(pDX, IDC_CONNECT_CLIENT_ALIAS, m_dest_alias);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConnectClient, CDialog)
	//{{AFX_MSG_MAP(CDialogConnectClient)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConnectClient message handlers

bool Client::SendPacket(unsigned char cmd, unsigned char dest, unsigned char src, unsigned short length, unsigned char* data) {
	unsigned char* paquet = new unsigned char [4+length];
	paquet[0]= cmd;	// le premier octet de paquet, c'est la commande
	paquet[1] = (dest&0x3f)<< 2 | ((src & 0x30) >> 4); // le deuxieme octet, c'est 6 bits de destination +2 bits de source
	paquet[2] = ((src&(0x0f))<< 4) | ((length & 0xf00) >> 8); // le 3e octet, c'est 4 bits restant de la source et 4 bits de longueur
	paquet[3] = (length & 0x00ff); // le 4e octet, c'est 8 bit restant de la longueur
	memcpy(&paquet[4], data, length);
	
	Send (paquet, 4+length);
	return true;
}
//*****************************************************************************************************//
//*****************************************************************************************************//
//*					FONCTION PERMETTANT AFFICHER LA LISTE DES CLIENTS								*//
//*****************************************************************************************************//
//*****************************************************************************************************//
bool Client::AfficherListeClients(unsigned char* donnees){

// les variables pour recuperer les attributs des clients dans la liste
	int status;
	int id;
	//CString Alias = "";
// la longueur d'un alidias a recuperer
	unsigned short AliasLength = 0;
// le numero d'octets parcourus
	unsigned short ByteNbr = 1;
	CString msg;
	msg.Format("%d clients are now connected",(int) donnees[0]);
	DisplayMsg(msg);
	DisplayMsg("List of clients");
	for(unsigned short i=0; i<(unsigned short)donnees[0]; i++) {
		AliasLength = (unsigned short) donnees[ByteNbr];
		ByteNbr++;
		status = (int)((donnees[ByteNbr]>>6)&0x01);
		id = (int) donnees[ByteNbr]&0x3f;
		ByteNbr++;		
		CString Alias(&donnees[ByteNbr]);
		ByteNbr=ByteNbr+AliasLength;
		listeClient.push_back(RegistredClient(status, id, Alias));
		DisplayMsg(listeClient[i].PrintClient());
	}
	return true;
}
/*****************************************************************************************************/
//*****************************************************************************************************//
//*					FONCTION PERMETTANT D'ENVOYER UN MSG D'ERREUR AU SERVEUR						*//
//*****************************************************************************************************//
//*****************************************************************************************************//
bool Client::SignalError (unsigned char dest, unsigned char src, int error){
	unsigned char* CodeError = new unsigned char [4];
	CodeError[0] = (unsigned long)error >> 24;
	CodeError[1] = (unsigned long)error >>16;
	CodeError[2] = (unsigned long)error >> 8;
	CodeError[3] = (unsigned long)error;
	SendPacket(IND_CLIENT_ERROR, dest, src, 4, CodeError);
	delete [] CodeError;
	return true;
}
/*********************************************************************************************************************/
/**			Fonction SendToken utilisee lors de codage LZ77													**/
/** Cree un jeton  representant une sequence et envoie ce jeton au fichier d'ecriture								**/
/** PARAMETRES:	unsigned short Indice: indice de decalage de la sequence											**/
/**				unsigned short Longueur: la longueur de sequence													**/
/**				char Car: octet suivant de la sequence dans la fenetre de lecture									**/
/**				unsigned short& ptr_lec: pointeur de debut de lecture, sera incremente a chaque envoie d'un jeton	**/
/**				unsigned short& ptr_rech: pointeur de debut de fenetre de recherche, est incremente a l'envoi		**/
/**											d'un jeton si le tableau de compression T est rempli					**/
/**				unsigned short& T_rech: la largeur de la fenetre de recherche (ne doit jamais exceder 2047)			**/
/**				unsigned short& N: nombre des caractere compresses et sauvegardes									**/
/**				CFile& Fichier:	le fichier de dictionnaire															**/
/** VALEUR DE RETOUR		: aucune																				**/
/***********************************************************************************************************************/
void Client::SendToken (CFile& Fichier, unsigned short Indice, unsigned short Longueur, unsigned char Car, 
						unsigned short& ptr_lec, unsigned short& ptr_rech, unsigned short& T_rech, unsigned short& N)
{	
	unsigned char Jeton [3];
	Jeton[0]=(Indice>>3)&0xff;
	Jeton[1]=((Indice<<5)&0xe0)|(Longueur&(0x1f));
	Jeton[2]=Car;
	Fichier.Write(Jeton,3);
	N = N+3;
	ptr_lec = (ptr_lec+Longueur+1)%2079;
	if (T_rech+Longueur+1>2047){
		ptr_rech = (ptr_rech+(T_rech+Longueur+1-2047))%2079;
		T_rech = 2047;
	}
	else
		T_rech= T_rech+Longueur+1;
}
/*********************************************************************************************************************/
/**			Fonction ReadCharacter utilisee lors de codage LZ77														**/
/** Effectue la lecture dans le fichier en comresser, met le caractere lu dans le tableau et ajuste le pointeur		**/
/** PARAMETRES:	unsigned char tableau[]: tableau de lecture qu'on rempli avec des caracteres lus					**/
/**				unsigned short& ptr: pointeur de la fin de la fenetre de lecture									**/
/**				unsigned short& number: nombre total des caracteres lus du fichier a compresser						**/
/**				CFile& fichier:	le fichier a compresser																**/
/** VALEUR DE RETOUR		: aucune																				**/
/***********************************************************************************************************************/
bool Client::ReadCharacter (CFile& fichier, unsigned char tableau[], unsigned short& ptr) {
	unsigned short L;
	unsigned char* M = new unsigned char;

	L=fichier.Read(M,1);
	if (L!=0){
		tableau[ptr] = *M;
		ptr = (ptr+1)%2079;
		delete M;
		return true;
	}
	else {
		delete M;
		return false;
	}
}
/***********************************************************************************************************************/
/**			Fonction FindSequence utilisee lors de codage LZ77												**/
/** Dans un tableau de recherche elle recherche une sequence de caracteres correspondente a la sequence de caracteres au **/
/** debut de la fenetre de lecture																					**/
/** PARAMETRES:	unsigned short& deb_n_seq: a l'appel a cette fonction cette variable defenit le pointeur de debut	**/
/**								de recherche d'une sequence dans la fenetre de recherche; une fois une telle		**/
/**								sequence trouvee, la variable prend la valeur du pointeur de debut de la			**/
/**								sequence trouvee																	**/
/**				int deb_lec:	le pointeur de debut de fenetre de lecture et, consequemment, de la fin de la fenetre**/
/**								de recherche																		**/
/**				int fin_lec		: le pointeur indiquant la fin de la fenetre de lecture								**/
/**				char T [2079]:	le tebleau contenant les fenetres de lecture et de recherche						**/
/** VALEUR DE RETOUR		: unsigned short, indique la longueur de la sequence trouvee (0 si aucune recurrence	**/
/**								n'a ete trouvee)																	**/
/***********************************************************************************************************************/
unsigned short Client::FindSequence (unsigned char T [], unsigned short& ptr, unsigned short deb_lec, unsigned short fin_lec)
{
	unsigned short longueur_seq= 0;
	while (ptr!=deb_lec){
		if (T[ptr]==T[deb_lec]){
			longueur_seq = 1;
			break;
		}
		ptr = (ptr+1)%2079;
	}
	if (longueur_seq == 1){
		while (((deb_lec+longueur_seq)%2079 != fin_lec) && (T[(ptr+longueur_seq)%2079]==T[(deb_lec+longueur_seq)%2079])){
			longueur_seq++;
		}
	}
	return longueur_seq;
}/*********************************************************************************************************************/
/**			Fonction RestituerCaractere utilisee lors de decodage LZ77												**/
/**				Restitue le contenu du jeton (indice de decalage, longueur de sequence, caractere)					**/
/** PARAMETRES:	unsigned short& indice: indice de decalage de la sequence											**/
/**				char* jeton:			le pointeur vers le jeton lu (3 octets)										**/
/**				char& caractere:		caractere qui suivi la sequence compressee									**/
/** VALEUR DE RETOUR		: unsigned short (retourne la longueur de la sequence compressee)						**/
/***********************************************************************************************************************/
unsigned short Client::RestituerSequence (unsigned char* jeton, unsigned short& indice, unsigned char& caractere) {
	unsigned short longueur;
	//caractere sauvegarde dans le jeton
	caractere = jeton[2];
	// Indice de decalage (11 bits)
	indice = ((short)(jeton[1]&0xE0)  >> 5 |(short)(jeton[0] &0x00FF) << 3);
	// Longeur de la sequence (5 bits)
	longueur = (jeton[1] & (0x1F));
	return longueur;
}
