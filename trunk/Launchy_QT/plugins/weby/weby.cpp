#include "weby.h"
#include <QtGui>
#include <QUrl>


void WebyPlugin::init()
{
	QSettings* set = *settings;
	if ( set->value("weby/version", 0.0).toDouble() == 0.0 ) {
		set->beginWriteArray("weby/sites");
		set->setArrayIndex(0);
		set->setValue("name", "Google");
		set->setValue("base", "http://www.google.com/");
		set->setValue("query", "search?q=");

		set->setArrayIndex(1);
		set->setValue("name", "Live Search");
		set->setValue("base", "http://search.live.com/");
		set->setValue("query", "results.aspx?q=");

		set->setArrayIndex(2);
		set->setValue("name", "Yahoo");
		set->setValue("base", "http://search.yahoo.com/");
		set->setValue("query", "search?p=");

		set->setArrayIndex(3);
		set->setValue("name", "MSN");
		set->setValue("base", "http://search.msn.com/");
		set->setValue("query", "results.aspx?q=");

		set->setArrayIndex(4);
		set->setValue("name", "Weather");
		set->setValue("base", "http://www.weather.com/");
		set->setValue("query", "weather/local/");	

		set->setArrayIndex(5);
		set->setValue("name", "Amazon");
		set->setValue("base", "http://www.amazon.com/");
		set->setValue("query", "gp/search/?keywords=");

		set->setArrayIndex(6);
		set->setValue("name", "YouTube");
		set->setValue("base", "http://www.youtube.com/");
		set->setValue("query", "results?search_query=");

		set->setArrayIndex(7);
		set->setValue("name", "Wikipedia");
		set->setValue("base", "http://www.wikipedia.com/");
		set->setValue("query", "wiki/Special:Search?search=");

		set->setArrayIndex(8);
		set->setValue("name", "Dictionary");
		set->setValue("base", "http://www.dictionary.com/");
		set->setValue("query", "browse/");		

		set->setArrayIndex(9);
		set->setValue("name", "Thesaurus");
		set->setValue("base", "http://www.thesaurus.com/");
		set->setValue("query", "browse/");		

		set->setArrayIndex(10);
		set->setValue("name", "Netflix");
		set->setValue("base", "http://www.netflix.com/");
		set->setValue("query", "Search?v1=");		

		set->setArrayIndex(11);
		set->setValue("name", "MSDN");
		set->setValue("base", "http://search.msdn.microsoft.com/");
		set->setValue("query", "search/default.aspx?siteId=0&tab=0&query=");

		set->setArrayIndex(11);
		set->setValue("name", "E-Mail");
		set->setValue("base", "mailto:");
		set->setValue("query", "");

		set->endArray();
	}
	set->setValue("weby/version", 2.0);

	// Read in the array of websites
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
	foreach(WebySite site, sites) {
		items->push_back(CatItem(site.name + ".weby", site.name, HASH_WEBY));
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
	bool found = false;
	foreach(WebySite site, sites) {
		if (item->shortName == site.name) {
			found = true;
			file = site.base;
			if (args != "")
				file += site.query + args;
			break;
		}
	}

	if (!found)
		file = item->shortName;	

	QUrl url(file);
	runProgram(url.toEncoded(), "");
}

void WebyPlugin::doDialog(QWidget* parent) {

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
			doDialog((QWidget*) wParam);

		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(weby, WebyPlugin) 