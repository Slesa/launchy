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
//#include <QFile>
//#include <QTextStream>
//#include <QTime>

#ifdef Q_WS_WIN
#include <shellapi.h>
#include <assert.h>
#include <cpl.h>
#endif


/*
void log(QString str) {
	return;
	QFile file("debug.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	QTextStream out(&file);
	out << "[" << QTime::currentTime().toString() << "] ";
	out << str;
	file.close();
}
*/

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
	int a = 0;
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

		if (cache.count(file) > 0) {
			if (cache[file] != "") 
				items->push_back(CatItem(path, cache[file], 0, getIcon()));
			continue;
		}
		union { 
			NEWCPLINFOA NewCplInfoA;
			NEWCPLINFOW NewCplInfoW; 
		} Newcpl;
	    
		HINSTANCE hLib; // Library Handle to *.cpl file
		APPLET_PROC CplCall; // Pointer to CPlApplet() function
		LONG i;


		hLib = LoadLibrary((LPCTSTR) path.utf16());
		if (hLib) {
			CplCall=(APPLET_PROC)GetProcAddress(hLib,"CPlApplet");
			if (CplCall) {
				if (CplCall(NULL, CPL_INIT,0,0)) {
					
					for (i=0;i<CplCall(NULL,CPL_GETCOUNT,0,0);i++)
					{	        
						Newcpl.NewCplInfoA.dwSize = 0;
						Newcpl.NewCplInfoA.dwFlags = 0;
						a = CplCall(NULL,CPL_INQUIRE,i,(long)&Newcpl);

						if (Newcpl.NewCplInfoA.dwSize == sizeof(NEWCPLINFOW))
						{
							// Case #1, CPL_INQUIRE has returned an Unicode String
							items->push_back(CatItem(path, QString::fromUtf16((const ushort*)Newcpl.NewCplInfoW.szName), 0, getIcon()));
							cache[file] = QString::fromUtf16((const ushort*)Newcpl.NewCplInfoW.szName);
						}
						else 
						{
							// Case #2, CPL_NEWINQUIRE has returned an ANSI String
							if (Newcpl.NewCplInfoA.dwSize != sizeof(NEWCPLINFOA))
							{
								// Case #3, CPL_NEWINQUIRE failed to return a string
								//    Get the string from the *.cpl Resource instead
								CPLINFO CInfo;
								a = CplCall(NULL,CPL_INQUIRE,i,(long)&CInfo);

								LoadStringA(hLib,CInfo.idName,
									Newcpl.NewCplInfoA.szName,32);
							}
			//				wchar_t	tmpString[32];
			//				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Newcpl.NewCplInfoA.szName, 32, tmpString, 32);
							items->push_back(CatItem(path, QString(Newcpl.NewCplInfoA.szName), 0, getIcon()));
							cache[file] = QString(Newcpl.NewCplInfoA.szName);
						}
					} // for
					CplCall(NULL,CPL_EXIT,0,0);
				}
				
			}
			if (hLib != NULL)
				FreeLibrary(hLib);
		} 
	}
}
#endif



void controlyPlugin::getCatalog(QList<CatItem>* items) {
	getApps(items);
	CatItem tmp = CatItem("Launchy.controly", "Launchy", HASH_controly, getIcon());
	tmp.usage = 5000;
	items->push_back(tmp); 
}

void controlyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	if (id->count() < 2) return;
	if (id->first().getTopResult().id == HASH_controly) {
		QSettings* set = *settings;
		if (set == NULL) return;

		CatItem tmp = CatItem("Launchy.options", "Options", HASH_controly, getIcon());
//		tmp.usage = set->value("controly/OptionsCount",0).toInt();
		results->push_back(tmp);

		tmp = CatItem("Launchy.reindex", "Rebuild Index", HASH_controly, getIcon());
//		tmp.usage = set->value("controly/RebuildCount",0).toInt();
		results->push_back(tmp);

		tmp = CatItem("Launchy.exit", "Exit", HASH_controly, getIcon());
//		tmp.usage = set->value("controly/ExitCount",0).toInt();
		results->push_back(tmp);
	}	
}

int controlyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	if (id->count() < 2) return 1;
	CatItem last = id->last().getTopResult();
	QSettings* set = *settings;
	if (set == NULL) return 1;
	if (last.shortName == "Options") {
//		set->setValue("controly/OptionsCount", set->value("controly/OptionsCount",0).toInt() + 1);
		return MSG_CONTROL_OPTIONS;
	}
	else if (last.shortName == "Rebuild Index") {
//		set->setValue("controly/RebuildCount", set->value("controly/RebuildCount",0).toInt() + 1);
		return MSG_CONTROL_REBUILD;
	}
	else if (last.shortName == "Exit") {
//		set->setValue("controly/ExitCount", set->value("controly/ExitCount",0).toInt() + 1);
		return MSG_CONTROL_EXIT;
	}
	return 1;

}
int controlyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	int handled = 0;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = 1;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = 1;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = 1;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = 1;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = 1;
			break;
		case MSG_LAUNCH_ITEM:
			handled = launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
			break;
		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(controly, controlyPlugin) 