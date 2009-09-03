#include "catalog_types.h"
#include "globals.h"


void FastCatalog::addItem(CatItem item)
{
	catList.push_back(item);
	int index = catList.count() - 1;
	CatItem* pCatItem = &catList[index];
	foreach(QChar c, item.lowName)
	{
		if (catIndex[c].count() == 0 || catIndex[c].last() != pCatItem)
			catIndex[c].push_back(pCatItem);
	}
}


int FastCatalog::getUsage(const QString& path)
{
	for (int i = 0; i < catList.size(); ++i)
	{
		if (path == catList[i].fullPath)
		{
			return catList[i].usage;
		}
	}	
	return 0;
}


void FastCatalog::incrementUsage(const CatItem & item)
{
	for (int i = 0; i < catList.size(); ++i)
	{
		if (item == catList[i])
		{
			catList[i].usage += 1;
			break;
		}
	}
}


int SlowCatalog::getUsage(const QString& path)
{
	for (int i = 0; i < catList.size(); ++i)
	{
		if (path == catList[i].fullPath)
		{
			return catList[i].usage;
		}
	}	
	return 0;
}


void SlowCatalog::incrementUsage(const CatItem & item)
{
	for (int i = 0; i < catList.size(); ++i)
	{
		if (item == catList[i])
		{
			catList[i].usage += 1;
			break;
		}
	}
}


void SlowCatalog::addItem(CatItem item)
{
	catList.push_back(item);
}


bool Catalog::matches(CatItem* item, const QString& match)
{
	int matchLength = match.count();
	int curChar = 0;

	foreach(QChar c, item->lowName)
	{
		if (c == match[curChar])
		{
			++curChar;
			if (curChar >= matchLength)
			{
				return true;
			}
		} 
	}

	return false;
}


QString Catalog::decorateText(const QString& text, const QString& match, bool outputRichText)
{
	QString decoratedText;
	int matchLength = match.count();
	int curChar = 0;

	foreach(QChar c, text)
	{
		if (curChar < matchLength && c.toLower() == match[curChar].toLower())
		{
			decoratedText += outputRichText ? QString("<u>") + c + QString("</u>") : QString("&") + c;
			++curChar;
		}
		else
		{
			decoratedText += c;
		}
	}

	return decoratedText;
}


void Catalog::searchCatalogs(QString txt, QList<CatItem> & out)
{
	txt = txt.toLower();
	QList<CatItem*> catMatches = search(txt);
	
	// Now prioritize the catalog items
	gSearchTxt = txt;
	qSort(catMatches.begin(), catMatches.end(), CatLess);
	
	// Check for history matches
	QString location = "History/" + txt;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if(hist.count() == 2)
	{
		for (int i = 0; i < catMatches.count(); i++)
		{
			if (catMatches[i]->lowName == hist[0] &&
				catMatches[i]->fullPath == hist[1])
			{
				CatItem* tmp = catMatches[i];
				catMatches.removeAt(i);
				catMatches.push_front(tmp);
			}
		}
	}

	// Load up the results
	int max = gSettings->value("GenOps/numresults", 10).toInt();
	for (int i = 0; i < max && i < catMatches.count(); i++)
	{
		out.push_back(*catMatches[i]);
	}
}


void Catalog::checkHistory(QString txt, QList<CatItem> & list)
{
	// Check for history matches
	QString location = "History/" + txt;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if(hist.count() == 2)
	{
		for (int i = 0; i < list.count(); i++)
		{
			if (list[i].lowName == hist[0] &&
				list[i].fullPath == hist[1])
			{
				CatItem tmp = list[i];
				list.removeAt(i);
				list.push_front(tmp);
			}
		}
	}
}


QList<CatItem*> FastCatalog::search(QString searchTxt)
{
	QList<CatItem*> ret;
	if (searchTxt == "")
		return ret;


	// Find the smallest char list
	QChar leastCommon = -1;
	foreach(QChar c, searchTxt)
	{
		if (leastCommon == -1 || catIndex[c].count() < leastCommon)
			leastCommon = c;
	}
	
	if (catIndex[leastCommon].count() == 0)
		return ret;

	// Search the list
	for (int i = 0; i < catIndex[leastCommon].count(); ++i)
	{
		if (matches(catIndex[leastCommon][i], searchTxt))
		{
			ret.push_back(catIndex[leastCommon][i]);
		}
	}
	return ret;
}


QList<CatItem*> SlowCatalog::search(QString searchTxt)
{
	QList<CatItem*> ret;
	if (searchTxt == "")
		return ret;

	QString lowSearch = searchTxt.toLower();

	for (int i = 0; i < catList.count(); ++i)
	{
		if (matches(&catList[i], lowSearch))
		{
			ret.push_back(&catList[i]);
		}
	}
	return ret;
}
