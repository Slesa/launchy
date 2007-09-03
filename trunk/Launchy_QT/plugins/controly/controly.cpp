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
	QString dir = qd.absolutePath();

	QStringList files = qd.entryList(QStringList("*.cpl"), QDir::Files, QDir::Unsorted);
	foreach(QString file, files) {
		QString path = dir + "/" + file;
		HINSTANCE LoadMe;
		LoadMe = LoadLibrary((LPCTSTR) path.utf16());
		if (LoadMe == 0) continue;
//		QString cpl = "CPlApplet";
		APPLET_PROC cpla = (APPLET_PROC)GetProcAddress(LoadMe, "CPlApplet");
		if (cpla == NULL) {
			FreeLibrary(LoadMe);
			continue;
		}
		LONG nInitResult = cpla(hwnd, CPL_INIT, 0, 0); 
		if (nInitResult == 0) {
			cpla(hwnd,CPL_STOP, 0, 0);
			cpla(hwnd, CPL_EXIT, 0, 0);
			FreeLibrary(LoadMe);			
			continue;
		}

		LONG NumberOfApplets = cpla(hwnd, CPL_GETCOUNT, 0, 0);
		for(int j = 0; j < NumberOfApplets; j++) {
			CPLINFO info;
			if (cpla(hwnd, CPL_INQUIRE, j, (LPARAM) &info) != 0)
			{	
				break;
			}
			TCHAR CPName[32];
			if (0 == LoadString(LoadMe, info.idName, CPName, 32) )
			{
				break;	
			}			
			items->push_back(CatItem(path, QString::fromUtf16((const ushort*)CPName), 0, getIcon()));			
		}
		cpla(hwnd,CPL_STOP, 0, 0);
		cpla(hwnd, CPL_EXIT, 0, 0);
		FreeLibrary(LoadMe);
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