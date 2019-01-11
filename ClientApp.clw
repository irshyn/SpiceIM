; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ClientApp.h"
LastPage=0

ClassCount=10
Class1=CClientAppApp
Class2=CClientAppDoc
Class3=CClientAppView
Class4=CMainFrame

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX (French (France))
Class6=CChatView
Resource4=IDR_MAINFRAME (French (France))
Class7=Client
Class8=CDialog_Info_Connect
Resource5=IDD_DIALOG_INFO_CONNECT
Resource6=IDD_DIALOG_CONNECT
Class9=DialogInfo
Resource7=IDD_DIALOG_INFO
Class10=CDialogConnectClient
Resource8=IDD_DIALOG_CONNECT_CLIENT

[CLS:CClientAppApp]
Type=0
HeaderFile=ClientApp.h
ImplementationFile=ClientApp.cpp
Filter=N

[CLS:CClientAppDoc]
Type=0
HeaderFile=ClientAppDoc.h
ImplementationFile=ClientAppDoc.cpp
Filter=N
LastObject=CClientAppDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CClientAppView]
Type=0
HeaderFile=ClientAppView.h
ImplementationFile=ClientAppView.cpp
Filter=C
BaseClass=CEditView
VirtualFilter=VWC
LastObject=ID_EDITION_NETTOYER


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_EDITION_MONITOR




[CLS:CAboutDlg]
Type=0
HeaderFile=ClientApp.cpp
ImplementationFile=ClientApp.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_APP_ABOUT
CommandCount=11
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[MNU:IDR_MAINFRAME (French (France))]
Type=1
Class=CMainFrame
Command1=ID_SERVEUR_CONNECTER
Command2=ID_SERVEUR_INFORMATION
Command3=ID_SERVEUR_LISTE_CLIENTS
Command4=ID_SERVEUR_DECONNECTER
Command5=ID_APP_EXIT
Command6=ID_CLIENT_DEMANDEDECONNECTION
Command7=ID_CLIENT_TRANSFERTDEFICHIER
Command8=ID_CLIENT_FERMETURECONNECTION
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDITION_NETTOYER
Command14=ID_EDITION_MONITOR
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME (French (France))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_ABOUTBOX (French (France))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308480

[CLS:CChatView]
Type=0
HeaderFile=ChatView.h
ImplementationFile=ChatView.cpp
BaseClass=CEditView
Filter=C
LastObject=CChatView
VirtualFilter=VWC

[DLG:IDD_DIALOG_INFO_CONNECT]
Type=1
Class=CDialog_Info_Connect
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PORT,edit,1350631552
Control7=IDC_ALIAS,edit,1350631552
Control8=IDC_IPADDRESS,edit,1350631552

[CLS:Client]
Type=0
HeaderFile=Client.h
ImplementationFile=Client.cpp
BaseClass=CSocket
Filter=N
LastObject=Client
VirtualFilter=uq

[CLS:CDialog_Info_Connect]
Type=0
HeaderFile=client.h
ImplementationFile=client.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialog_Info_Connect
VirtualFilter=dWC

[DLG:IDD_DIALOG_INFO]
Type=1
Class=DialogInfo
ControlCount=24
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATUT,static,1342308352
Control16=IDC_CLIENT_IP,static,1342308352
Control17=IDC_CLIENT_PORT,static,1342308352
Control18=IDC_SERVEUR_IP,static,1342308352
Control19=IDC_SERVEUR_PORT,static,1342308352
Control20=IDC_CLIENT_ALIAS,static,1342308352
Control21=IDC_CLIENT_ID,static,1342308352
Control22=IDC_CONN_STATUS,static,1342308352
Control23=IDC_CONN_ALIAS,static,1342308352
Control24=IDC_CONN_ID,static,1342308352

[DLG:IDD_DIALOG_CONNECT]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_PORT,edit,1350631552
Control7=IDC_ALIAS,edit,1350631552
Control8=IDC_IPADDRESS,edit,1350631552

[CLS:DialogInfo]
Type=0
HeaderFile=client.h
ImplementationFile=client.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_CLIENT_ALIAS

[DLG:IDD_DIALOG_CONNECT_CLIENT]
Type=1
Class=CDialogConnectClient
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CONNECT_CLIENT_ALIAS,edit,1350631552

[CLS:CDialogConnectClient]
Type=0
HeaderFile=client.h
ImplementationFile=client.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogConnectClient
VirtualFilter=dWC

