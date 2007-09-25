/*
Launchy: Application Launcher
Copyright (C) 2007  Josh Karlin

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

#include <QtGui>
#include <QDir>
#include <QString>

#include "controly.h"

#ifdef Q_WS_WIN
#include <shellapi.h>
#include <assert.h>
#include <cpl.h>
#endif


void controlyPlugin::init()
{

}

void controlyPlugin::getID(uint* id)
{
	*id = HASH_controly;
}

void controlyPlugin::getName(QString* str)
{
	*str = "Controly";
}

QString controlyPlugin::getIcon()
{
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/plugins/icons/controly.ico";
#endif
}

#ifdef Q_WS_WIN


void controlyPlugin::getApps(QList<CatItem>* items) {

	HWND hwnd = NULL;
	// Get the control panel applications
	TCHAR  infoBuf[32767];
	if (!GetSystemDirectory(infoBuf, 32767)) {
		return;
	}
	QString buff = QString::fromUtf16((const ushort*) infoBuf);
	QDir qd(buff);

	QStringList files = qd.entryList(QStringList("*.cpl"), QDir::Files, QDir::Unsorted);
	foreach(QString file, files) {
		QString path = QDir::toNativeSeparators(qd.absoluteFilePath(file));

		union { 
			NEWCPLINFOA NewCplInfoA;
			NEWCPLINFOW NewCplInfoW; 
		} Newcpl;
	    
		HINSTANCE hLib; // Library Handle to *.cpl file
		APPLET_PROC CplCall; // Pointer to CPlApplet() function
		LONG i;
	    
		// -------------------
		if (!(hLib = LoadLibrary((LPCTSTR) path.utf16()))) 
			continue ;	
		if (!(CplCall=(APPLET_PROC)GetProcAddress(hLib,"CPlApplet")))
		{
			FreeLibrary(hLib);        
			continue ;
		}
	    
		// -------------------
		CplCall(NULL, CPL_INIT,0,0); // Init the *.cpl file
	    
		for (i=0;i<CplCall(NULL,CPL_GETCOUNT,0,0);i++)
		{	        
			Newcpl.NewCplInfoA.dwSize = 0;
			Newcpl.NewCplInfoA.dwFlags = 0;
			CplCall(NULL,CPL_NEWINQUIRE,i,(long)&Newcpl);
	        
			if (Newcpl.NewCplInfoA.dwSize == sizeof(NEWCPLINFOW))
			{   // Case #1, CPL_NEWINQUIRE has returned an Unicode String
				items->push_back(CatItem(path, QString::fromUtf16((const ushort*)Newcpl.NewCplInfoW.szName), 0, getIcon()));
			}
			else 
			{   // Case #2, CPL_NEWINQUIRE has returned an ANSI String
				if (Newcpl.NewCplInfoA.dwSize != sizeof(NEWCPLINFOA))
				{
					// Case #3, CPL_NEWINQUIRE failed to return a string
					//    Get the string from the *.cpl Resource instead
					CPLINFO CInfo;
	                
					CplCall(NULL,CPL_INQUIRE,i,(long)&CInfo);				
					LoadStringA(hLib,CInfo.idName,
						Newcpl.NewCplInfoA.szName,32);
				}
//				wchar_t	tmpString[32];
//				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Newcpl.NewCplInfoA.szName, 32, tmpString, 32);
				items->push_back(CatItem(path, QString(Newcpl.NewCplInfoA.szName), 0, getIcon()));
			}
		} // for
	    
		CplCall(NULL,CPL_EXIT,0,0);
	    
		// -------------------
		FreeLibrary(hLib);        
	}
}
#endif



void controlyPlugin::getCatalog(QList<CatItem>* items) {
	getApps(items);
}


bool controlyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = true;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = true;
			break;

		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(controly, controlyPlugin) 