; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTranProDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ssmApp.h"

ClassCount=4
Class1=CSsmAppApp
Class2=CSsmAppDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_SSMAPP_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CTranProDlg
Resource4=IDD_TRANPRO_DIALOG

[CLS:CSsmAppApp]
Type=0
HeaderFile=ssmApp.h
ImplementationFile=ssmApp.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CSsmAppApp

[CLS:CSsmAppDlg]
Type=0
HeaderFile=ssmAppDlg.h
ImplementationFile=ssmAppDlg.cpp
Filter=D
LastObject=CSsmAppDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ssmAppDlg.h
ImplementationFile=ssmAppDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SSMAPP_DIALOG]
Type=1
Class=CSsmAppDlg
ControlCount=2
Control1=IDC_SHOCKWAVEFLASH1,{D27CDB6E-AE6D-11CF-96B8-444553540000},1342242816
Control2=IDC_SPIN1,msctls_updown32,1342177312

[DLG:IDD_TRANPRO_DIALOG]
Type=1
Class=CTranProDlg
ControlCount=3
Control1=IDC_STMDEVICECTRL1,{48463046-5067-11D4-8316-0080C88EE29F},1073807360
Control2=IDC_ACTZT598CTRL1,{3AC0F674-5099-4754-BD85-99FB02A842D6},1073807360
Control3=IDC_EXPLORER1,{8856F961-340A-11D0-A96B-00C04FD705A2},1342242816

[CLS:CTranProDlg]
Type=0
HeaderFile=TranProDlg.h
ImplementationFile=TranProDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EXPLORER1
VirtualFilter=dWC

