/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin, Simon Capewell

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

#include "precompiled.h"
#include "catalog_types.h"
#include "globals.h"


bool Catalog::load(const QString& filename)
{
	QFile inFile(filename);
	if (!inFile.open(QIODevice::ReadOnly))
		return false;

	QByteArray ba = inFile.readAll();
	QByteArray unzipped = qUncompress(ba);
	QDataStream in(&unzipped, QIODevice::ReadOnly);
	in.setVersion(QDataStream::Qt_4_2);

	while (!in.atEnd())
	{
		CatItem item;
		in >> item;
		addItem(item);
	}

	return true;
}


bool Catalog::save(const QString& filename)
{
	QByteArray ba;
	QDataStream out(&ba, QIODevice::ReadWrite); 
	out.setVersion(QDataStream::Qt_4_2);

	for (int i = 0; i < count(); i++)
	{
		CatItem item = getItem(i);
		out << item;
	}

	// Zip the archive
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		qDebug() << "Could not open database for writing";
		return false;
	}
	file.write(qCompress(ba));
	return true;
}


void FastCatalog::addItem(const CatItem& item)
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


void FastCatalog::incrementUsage(const CatItem& item)
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


void SlowCatalog::incrementUsage(const CatItem& item)
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


void SlowCatalog::addItem(const CatItem& item)
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
	if (!gSettings->value("GenOps/decoratetext", false).toBool())
		return text;
	QString decoratedText;
	int matchLength = match.count();
	int curChar = 0;

	int index = text.toLower().indexOf(match);
	if (index > 0)
		decoratedText = text.left(index);
	else
		index = 0;
	bool highlighted = false;
	for (; index < text.count(); ++index)
	{
		QChar c = text[index];
		// prefix based rendering is buggy with lots of underlines limit it to 15
		// until we get round to replacing the list widget delegate with a rich text delegate
		if (curChar < matchLength && c.toLower() == match[curChar].toLower() &&
			(outputRichText || curChar < 15))
		{
			if (outputRichText)
			{
				if (!highlighted)
				{
					decoratedText += "<u>";
					highlighted = true;
				}
				decoratedText += c;
			}
			else
				decoratedText += QString("&") + c;
			++curChar;
		}
		else
		{
			if (outputRichText && highlighted)
			{
				decoratedText += "</u>";
				highlighted = false;
			}
			decoratedText += c;
		}
	}

	if (outputRichText && highlighted)
	{
		decoratedText += "</u>";
		highlighted = false;
	}

	return decoratedText;
}


void Catalog::searchCatalogs(const QString& text, QList<CatItem> & out)
{
	gSearchText = text.toLower();
	QList<CatItem*> catMatches = search(gSearchText);
	
	// Now prioritize the catalog items
	qSort(catMatches.begin(), catMatches.end(), CatLess);

	// Check for history matches
	QString location = "History/" + gSearchText;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if (hist.count() == 2)
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


void Catalog::promoteRecentlyUsedItems(const QString& text, QList<CatItem> & list)
{
	// Check for history matches
	QString location = "History/" + text;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if (hist.count() == 2)
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


QList<CatItem*> FastCatalog::search(const QString& searchText)
{
	QList<CatItem*> result;

	if (searchText.count() > 0)
	{
		// Find the smallest char list
		QChar leastCommon = -1;
		foreach(QChar c, searchText)
		{
			if (leastCommon == -1 || catIndex[c].count() < leastCommon)
				leastCommon = c;
		}
		
		if (catIndex[leastCommon].count() > 0)
		{
			// Search the list
			for (int i = 0; i < catIndex[leastCommon].count(); ++i)
			{
				if (matches(catIndex[leastCommon][i], searchText))
				{
					result.push_back(catIndex[leastCommon][i]);
				}
			}
		}
	}

	return result;
}


QList<CatItem*> SlowCatalog::search(const QString& searchText)
{
	QList<CatItem*> result;

	if (searchText.count() > 0)
	{
		QString lowSearch = searchText.toLower();

		for (int i = 0; i < catList.count(); ++i)
		{
			if (matches(&catList[i], lowSearch))
			{
				result.push_back(&catList[i]);
			}
		}
	}

	return result;
}
