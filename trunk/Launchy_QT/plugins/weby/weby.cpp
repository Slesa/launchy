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
#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QTextCodec>

#ifdef Q_WS_WIN
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>

#endif

#include "weby.h"
#include "gui.h"

WebyPlugin* gWebyInstance = NULL;

void WebyPlugin::init()
{
	if (gWebyInstance == NULL)
		gWebyInstance = this;



	QSettings* set = *settings;
	if ( set->value("weby/version", 0.0).toDouble() == 0.0 ) {
		set->beginWriteArray("weby/sites");
		set->setArrayIndex(0);
		set->setValue("name", "Google");
		set->setValue("base", "http://www.google.com/");
		set->setValue("query", "search?q=%s");

		set->setArrayIndex(1);
		set->setValue("name", "Live Search");
		set->setValue("base", "http://search.live.com/");
		set->setValue("query", "results.aspx?q=%s");

		set->setArrayIndex(2);
		set->setValue("name", "Yahoo");
		set->setValue("base", "http://search.yahoo.com/");
		set->setValue("query", "search?p=%s");

		set->setArrayIndex(3);
		set->setValue("name", "MSN");
		set->setValue("base", "http://search.msn.com/");
		set->setValue("query", "results.aspx?q=%s");

		set->setArrayIndex(4);
		set->setValue("name", "Weather");
		set->setValue("base", "http://www.weather.com/");
		set->setValue("query", "weather/local/%s");	

		set->setArrayIndex(5);
		set->setValue("name", "Amazon");
		set->setValue("base", "http://www.amazon.com/");
		set->setValue("query", "gp/search/?keywords=%s");

		set->setArrayIndex(6);
		set->setValue("name", "YouTube");
		set->setValue("base", "http://www.youtube.com/");
		set->setValue("query", "results?search_query=%s");

		set->setArrayIndex(7);
		set->setValue("name", "Wikipedia");
		set->setValue("base", "http://www.wikipedia.com/");
		set->setValue("query", "wiki/Special:Search?search=%s&go=Go");

		set->setArrayIndex(8);
		set->setValue("name", "Dictionary");
		set->setValue("base", "http://www.dictionary.com/");
		set->setValue("query", "browse/%s");		

		set->setArrayIndex(9);
		set->setValue("name", "Thesaurus");
		set->setValue("base", "http://www.thesaurus.com/");
		set->setValue("query", "browse/%s");		

		set->setArrayIndex(10);
		set->setValue("name", "Netflix");
		set->setValue("base", "http://www.netflix.com/");
		set->setValue("query", "Search?v1=%s");		

		set->setArrayIndex(11);
		set->setValue("name", "MSDN");
		set->setValue("base", "http://search.msdn.microsoft.com/");
		set->setValue("query", "search/default.aspx?siteId=0&tab=0&query=%s");

		set->setArrayIndex(11);
		set->setValue("name", "E-Mail");
		set->setValue("base", "mailto:");
		set->setValue("query", "%s");

		set->setArrayIndex(12);
		set->setValue("name", "IMDB");
		set->setValue("base", "http://www.imdb.com/");
		set->setValue("query", "find?s=all&q=%s");

		set->setArrayIndex(13);
		set->setValue("name", "Maps");
		set->setValue("base", "http://maps.google.com/");
		set->setValue("maps?f=q&hl=en&geocode=&q=%s&ie=UTF8&z=12&iwloc=addr&om=1");
		set->endArray();
	}
	set->setValue("weby/version", 2.0);

	// Read in the array of websites
	sites.clear();
	int count = set->beginReadArray("weby/sites");
	for(int i = 0; i < count; ++i) {
		set->setArrayIndex(i);
		WebySite s;
		s.base = set->value("base").toString();
		s.name = set->value("name").toString();
		s.query = set->value("query").toString();
		sites.push_back(s);
	}
	set->endArray();
}

void WebyPlugin::getID(uint* id)
{
	*id = HASH_WEBY;
}

void WebyPlugin::getName(QString* str)
{
	*str = "Weby";
}

void WebyPlugin::getLabels(QList<InputData>* id)
{
	if (id->count() > 1)
		return;

	// Apply a "website" label if we think it's a website
	QString & text = id->last().getText();

	if (text.contains("http://", Qt::CaseInsensitive))
		id->last().setLabel(HASH_WEBSITE);
	else if (text.contains("https://", Qt::CaseInsensitive)) 
		id->last().setLabel(HASH_WEBSITE);
	else if (text.contains(".com", Qt::CaseInsensitive)) 
		id->last().setLabel(HASH_WEBSITE);
	else if (text.contains(".net", Qt::CaseInsensitive))
		id->last().setLabel(HASH_WEBSITE);
	else if (text.contains(".org", Qt::CaseInsensitive))
		id->last().setLabel(HASH_WEBSITE);
	else if (text.contains("www.", Qt::CaseInsensitive))
		id->last().setLabel(HASH_WEBSITE);
}

void WebyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	if (id->last().hasLabel(HASH_WEBSITE)) {
		QString & text = id->last().getText();
		// This is a website, create an entry for it
		results->push_front(CatItem(text + ".weby", text, HASH_WEBY, getIcon()));
	}

	if (id->count() > 1 && id->first().getTopResult().id == HASH_WEBY) {
		QString & text = id->last().getText();
		// This is user search text, create an entry for it
		results->push_front(CatItem(text + ".weby", text, HASH_WEBY, getIcon()));
	}
}

#ifdef Q_WS_WIN
BOOL GetShellDir(int iType, QString& szPath)
{
	QString tmpp = "shell32.dll";
	HINSTANCE hInst = ::LoadLibrary( (LPCTSTR) tmpp.utf16() );
	if ( NULL == hInst )
	{
		return FALSE;
	}

	HRESULT (__stdcall *pfnSHGetFolderPath)( HWND, int, HANDLE, DWORD, LPWSTR );


	pfnSHGetFolderPath = reinterpret_cast<HRESULT (__stdcall *)( HWND, int, HANDLE, DWORD, LPWSTR )>( GetProcAddress( hInst,"SHGetFolderPathW" ) );

	if ( NULL == pfnSHGetFolderPath )
	{
		FreeLibrary( hInst ); // <-- here
		return FALSE;
	}

	TCHAR tmp[_MAX_PATH];
	HRESULT hRet = pfnSHGetFolderPath( NULL, iType, NULL, 0, tmp );
	szPath = QString::fromUtf16((const ushort*)tmp);
	FreeLibrary( hInst ); // <-- and here
	return TRUE;
	return 0;
}

void WebyPlugin::indexIE(QString path, QList<CatItem>* items)
{
	QDir qd(path);
	QString dir = qd.absolutePath();
	QStringList dirs = qd.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

	for (int i = 0; i < dirs.count(); ++i) {
		QString cur = dirs[i];
		if (cur.contains(".lnk")) continue;
		indexIE(dir + "/" + dirs[i],items);
	}	

	QStringList files = qd.entryList(QStringList("*.url"), QDir::Files, QDir::Unsorted);
	for(int i = 0; i < files.count(); ++i) {
		items->push_back(CatItem(dir + "/" + files[i], files[i].mid(0,files[i].size()-4)));
	}
}
#endif

QString WebyPlugin::getFirefoxPath()
{
	QString path;
	QString iniPath;
	QString appData;

#ifdef Q_WS_WIN
	GetShellDir(CSIDL_APPDATA, appData);
	iniPath = appData + "/Mozilla/Firefox/profiles.ini";
#endif

	QFile file(iniPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";
	bool isRel = false;
	while (!file.atEnd()) {
		QString line = file.readLine();
		if (line.contains("IsRelative")) {
			QStringList spl = line.split("=");
			isRel = spl[1].toInt();
		}
		if (line.contains("Path")) {
			QStringList spl = line.split("=");
			if (isRel)
				path = appData + "/Mozilla/Firefox/" + spl[1].mid(0,spl[1].count()-1) + "/bookmarks.html" ;
			else
				path = spl[1].mid(0,spl[1].count()-1);
			break;
		}
	} 	

	return path;
}

QString WebyPlugin::getIcon()
{
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/plugins/icons/weby.ico";
#endif
}

void WebyPlugin::indexFirefox(QString path, QList<CatItem>* items)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	marks.clear();

	QRegExp regex_url("<a href=\"([^\"]*)\"", Qt::CaseInsensitive);
	QRegExp regex_urlname("\">([^<]*)</A>", Qt::CaseInsensitive);
	QRegExp regex_shortcut("SHORTCUTURL=\"([^\"]*)\"");
	QRegExp regex_postdata("POST_DATA", Qt::CaseInsensitive);

	while (!file.atEnd()) {
		QString line = QString::fromUtf8(file.readLine());
		if (regex_url.indexIn(line) != -1) {
			Bookmark mark;
			mark.url = regex_url.cap(1);

			if (regex_urlname.indexIn(line) != -1) {
				mark.name = regex_urlname.cap(1);

				if (regex_postdata.indexIn(line) != -1) continue;
				if (regex_shortcut.indexIn(line) != -1) {
					mark.shortcut = regex_shortcut.cap(1);
					marks.push_back(mark);
					items->push_back(CatItem(mark.url + ".shortcut", mark.shortcut, HASH_WEBY, getIcon()));
				} else {
					items->push_back(CatItem(mark.url, mark.name, 0, getIcon()));
				}	
			}			
		}
	}
}


/*
QString string = fileName();//fileName() returns a chinese filename
                QTextCodec* gbk_codec = QTextCodec::codecForName("GB2312");
                QString gbk_string = gbk_codec->toUnicode(string);
                QLabel *label = new QLabel(gbk_string);
*/

void WebyPlugin::getCatalog(QList<CatItem>* items)
{
	foreach(WebySite site, sites) {
		items->push_back(CatItem(site.name + ".weby", site.name, HASH_WEBY, getIcon()));
	}

#ifdef Q_WS_WIN
	if ((*settings)->value("weby/ie", true).toBool()) {
		QString path;
		GetShellDir(CSIDL_FAVORITES, path);
		indexIE(path, items);
	}
#endif
	if ((*settings)->value("weby/firefox", true).toBool()) {
		QString path = getFirefoxPath();
		indexFirefox(path, items);
	}
}

void WebyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	QString file = "";
	QString args = "";


	if (id->count() == 2) {
		args = id->last().getText();
		item = &id->first().getTopResult();
	}

	// Is it a Firefox shortcut?
	if (item->fullPath.contains(".shortcut")) {
		file = item->fullPath.mid(0, item->fullPath.count()-9);
		file.replace("%s", args);
	}
	else { // It's a user-specific site
		bool found = false;
		foreach(WebySite site, sites) {
			if (item->shortName == site.name) {
				found = true;
				file = site.base;
				if (args != "") {
					QString tmp = site.query;
					tmp.replace("%s", args);
					file += tmp;
				}
				break;
			}
		}

		if (!found) {
			file = item->shortName;	
			if (!file.contains("http://")) {
				file = "http://" + file;
			}
		}
	}
	QUrl url(file);
	runProgram(url.toEncoded(), "");
}

void WebyPlugin::doDialog(QWidget* parent, QWidget** newDlg) {
	if (gui != NULL) return;
	gui = new Gui(parent);
	*newDlg = gui;
}

void WebyPlugin::endDialog(bool accept) {
	if (accept) {
		gui->writeOptions();
		init();
	}
	if (gui != NULL) 
		delete gui;
	
	gui = NULL;
}

bool WebyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_LABELS:
			getLabels((QList<InputData>*) wParam);
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
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = true;
			break;
		case MSG_LAUNCH_ITEM:
			launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
			handled = true;
			break;
		case MSG_HAS_DIALOG:
			handled = true;
			break;
		case MSG_DO_DIALOG:
			doDialog((QWidget*) wParam, (QWidget**) lParam);
			break;
		case MSG_END_DIALOG:
			endDialog((bool) wParam);
			break;

		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(weby, WebyPlugin) 