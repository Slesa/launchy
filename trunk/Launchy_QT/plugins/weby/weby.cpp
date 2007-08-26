#include "weby.h"
#include <QtCore>
#include <QUrl>


void runProgram(QString file, QString args) {
#ifdef WIN32
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (LPCTSTR) (file).utf16();
	if (args != "") {
		ShExecInfo.lpParameters = (LPCTSTR) args.utf16();
	} else {
		ShExecInfo.lpParameters = NULL;
	}
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&ShExecInfo);	
#endif

#ifdef OSX_BUILD

#endif

#ifdef KDE_BUILD

#endif

}


void WebyPlugin::getID(uint* id)
{
	*id = HASH_WEBY;
}

void WebyPlugin::getLabels(QList<InputData>* id)
{
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
		results->push_front(CatItem(text + ".weby", text, HASH_WEBY));
	}

	if (id->count() > 1 && id->first().getTopResult().id == HASH_WEBY) {
		QString & text = id->last().getText();
		// This is user search text, create an entry for it
		results->push_front(CatItem(text + ".weby", text, HASH_WEBY));
	}
}

void WebyPlugin::getCatalog(QList<CatItem>* items)
{
	items->push_back(CatItem("Google.weby", "Google", HASH_WEBY));
	items->push_back(CatItem("Live Search.weby", "Live Search", HASH_WEBY));
	items->push_back(CatItem("MSN.weby", "MSN", HASH_WEBY));
	items->push_back(CatItem("Yahoo.weby", "Yahoo", HASH_WEBY));
	items->push_back(CatItem("Weather.weby", "Weather", HASH_WEBY));
	items->push_back(CatItem("Amazon.weby", "Amazon", HASH_WEBY));
	items->push_back(CatItem("YouTube.weby", "YouTube", HASH_WEBY));
	items->push_back(CatItem("Wikipedia.weby", "Wikipedia", HASH_WEBY));
	items->push_back(CatItem("Dictionary.weby", "Dictionary", HASH_WEBY));
	items->push_back(CatItem("Thesaurus.weby", "Thesaurus", HASH_WEBY));
	items->push_back(CatItem("IMDB.weby", "IMDB", HASH_WEBY));
	items->push_back(CatItem("Netflix.weby", "Netflix", HASH_WEBY));
	items->push_back(CatItem("MSDN.weby", "MSDN", HASH_WEBY));
	items->push_back(CatItem("E-Mail.weby", "E-Mail", HASH_WEBY));
}

void WebyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	QString file = "";
	QString args = "";


	if (id->count() == 2) {
		args = id->last().getText();
		item = &id->first().getTopResult();
	}
	if (item->shortName == "Google") {
		file = "http://www.google.com/";
		if (args != "")
			file += "search?q=" + args;
	}
	else if (item->shortName == "Live Search") {
		file = "http://search.live.com/";
		if (args != "")
			file += "results.aspx?q=" + args;
	}
	else if (item->shortName == "Yahoo") {
		file = "http://search.yahoo.com/";
		if (args != "")
			file += "search?p=" + args;
	}
	else if (item->shortName == "MSN") {
		file = "http://search.msn.com/";
		if (args != "")
			file += "results.aspx?q=" + args;
	}
	else if (item->shortName == "Weather") {
		file = "http://www.weather.com/";
		if (args != "")
			file += "weather/local/" + args;
	}
	else if (item->shortName == "Amazon") {
		file = "http://www.amazon.com/";
		if (args != "")
			file += "gp/search/?keywords=" + args;
	}
	else if (item->shortName == "YouTube") {
		file = "http://www.youtube.com/";
		if (args != "")
			file += "results?search_query=" + args;
	}
	else if (item->shortName == "Wikipedia") {
		file = "http://www.wikipedia.com/";
		if (args != "")
			file += "wiki/Special:Search?search=" + args;
	}
	else if (item->shortName == "Dictionary") {
		file = "http://www.dictionary.com/";
		if (args != "")
			file += "browse/" + args;
	}
	else if (item->shortName == "Thesaurus") {
		file = "http://www.thesaurus.com/";
		if (args != "")
			file += "browse/" + args;
	}
	else if (item->shortName == "IMDB") {
		file = "http://www.imdb.com/";
		if (args != "")
			file += "find?q=" + args;
	}
	else if (item->shortName == "Netflix") {
		file = "http://www.netflix.com/";
		if (args != "")
			file += "Search?v1=" + args;
	}
	else if (item->shortName == "MSDN") {
		file = "http://search.msdn.microsoft.com/";
		if (args != "")
			file += "search/default.aspx?siteId=0&tab=0&query=" + args;
	}
	else if (item->shortName == "E-Mail")
		file = "mailto:" + args;
	else  {
		file = item->shortName;	
	}

	QUrl url(file);
	runProgram(url.toEncoded(), "");
}


bool WebyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{
		case MSG_GET_LABELS:
			getLabels((QList<InputData>*) wParam);
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
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
		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(weby, WebyPlugin) 