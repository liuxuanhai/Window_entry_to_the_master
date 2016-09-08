; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAddDirDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FileSystem.h"

ClassCount=8
Class1=CFileSystemApp
Class2=CFileSystemDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_FILESYSTEM_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ADD_USER
Resource4=IDR_OP_MENU
Class4=CAddDirDlg
Resource5=IDD_LOGGING
Class5=CLogDlg
Resource6=IDD_ABOUTBOX
Class6=CFileDlg
Resource7=IDD_FILE
Class7=CAddUserDlg
Resource8=IDD_SHOW_USER
Class8=CShowUserDlg
Resource9=IDD_ADD_DIR

[CLS:CFileSystemApp]
Type=0
HeaderFile=FileSystem.h
ImplementationFile=FileSystem.cpp
Filter=N

[CLS:CFileSystemDlg]
Type=0
HeaderFile=FileSystemDlg.h
ImplementationFile=FileSystemDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_HIGH_FORMAT

[CLS:CAboutDlg]
Type=0
HeaderFile=FileSystemDlg.h
ImplementationFile=FileSystemDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDC_STATIC,static,1342308353
Control2=IDC_STATIC,static,1342308352

[DLG:IDD_FILESYSTEM_DIALOG]
Type=1
Class=CFileSystemDlg
ControlCount=12
Control1=IDC_CUR_PATH,button,1342177287
Control2=IDC_FILE_LIST,SysListView32,1350664193
Control3=IDC_STATIC,button,1342177287
Control4=IDC_LOG,button,1342242816
Control5=IDC_LOW_FORMAT,button,1342242816
Control6=IDC_HIGH_FORMAT,button,1342242816
Control7=IDC_CREATE_DIR,button,1342242816
Control8=IDC_DELETE_DIR,button,1342242816
Control9=IDC_CREATE_FILE,button,1342242816
Control10=IDC_DELETE_FILE,button,1342242816
Control11=IDC_UPDATE,button,1342242816
Control12=IDC_AUTHOR,button,1342242816

[MNU:IDR_OP_MENU]
Type=1
Class=CFileSystemDlg
Command1=ID_LOG
Command2=ID_CREATE_USER
Command3=ID_SHOW_USER
Command4=ID_CREATE_FILE
Command5=ID_DELETE_FILE
Command6=ID_CREATE_DIR
Command7=ID_DELETE_DIR
Command8=ID_STALL
Command9=ID_LOW_FORMAT
Command10=ID_HIGH_FORMAT
CommandCount=10

[DLG:IDD_ADD_DIR]
Type=1
Class=CAddDirDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_SUB_DIR_NAME,edit,1350631552

[CLS:CAddDirDlg]
Type=0
HeaderFile=AddDirDlg.h
ImplementationFile=AddDirDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_LOG

[DLG:IDD_LOGGING]
Type=1
Class=CLogDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_USER_NAME,edit,1350631552
Control6=IDC_USER_PWD,edit,1350631552

[CLS:CLogDlg]
Type=0
HeaderFile=LogDlg.h
ImplementationFile=LogDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_FILE]
Type=1
Class=CFileDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TITLE,edit,1350631552
Control5=IDC_TEXT,edit,1352728644
Control6=IDC_STATIC,static,1342308352

[CLS:CFileDlg]
Type=0
HeaderFile=FileDlg.h
ImplementationFile=FileDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[DLG:IDD_ADD_USER]
Type=1
Class=CAddUserDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_USER_NAME,edit,1350631552
Control6=IDC_USER_PWD,edit,1350631552

[CLS:CAddUserDlg]
Type=0
HeaderFile=AddUserDlg.h
ImplementationFile=AddUserDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_USER_PWD

[DLG:IDD_SHOW_USER]
Type=1
Class=CShowUserDlg
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_DELETE_USER,button,1342242816
Control3=IDCANCEL,button,1342242816
Control4=IDC_USER_LIST,SysListView32,1350631425

[CLS:CShowUserDlg]
Type=0
HeaderFile=ShowUserDlg.h
ImplementationFile=ShowUserDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CShowUserDlg

