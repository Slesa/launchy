#include "weby.h"
#include <QtCore>

void WebyPlugin::getName(QString* name)
{
	*name = "Weby";
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
	if (!id->last().hasLabel(HASH_WEBSITE))
		return;

	QString & text = id->last().getText();
	// This is a website, create an entry for it
	CatItem item(text + ".weby", text, HASH_WEBY);
	results->push_back(item);
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
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;
		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(weby, WebyPlugin) 