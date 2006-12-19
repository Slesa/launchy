/*
Launchy: Application Launcher
Copyright (C) 2005  Josh Karlin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// Launchy.cpp : Defines the class behaviors for the application.
//
	
#include "stdafx.h"
//#include <vld.h>
#include "Launchy.h"
#include "LaunchyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLaunchyApp

BEGIN_MESSAGE_MAP(CLaunchyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLaunchyApp construction

CLaunchyApp::CLaunchyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}


// The one and only CLaunchyApp object

CLaunchyApp theApp;


CString searchTxt;
CStringArray SearchStrings;
int SearchPluginID;

// CLaunchyApp initialization

BOOL CLaunchyApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create application mutexes so that installer knows when
	// Launchy is running
	CreateMutex(NULL,0,_T("LaunchyMutex"));
	CreateMutex(NULL,0,_T("Global\\LaunchyMutex"));

/*	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
*/
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Launchy"));

	CLaunchyDlg dlg;
	m_pMainWnd = &dlg;



	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
