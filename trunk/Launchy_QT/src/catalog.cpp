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

#include "catalog.h"
#include "globals.h"
#include <qdebug.h>

#include <QtAlgorithms>

bool CatLessNoPtr (CatItem & a, CatItem & b) {
	return CatLess(&a, &b);
}

bool CatLess (CatItem* a, CatItem* b)  {
/*
	if (a->isHistory) { return true; }
	if (b->isHistory) { return false; }
*/
	bool localEqual = a->lowName == gSearchTxt;
	bool otherEqual = b->lowName == gSearchTxt;

	if (localEqual && !otherEqual)
		return true;
	if (!localEqual && otherEqual)
		return false;


	if(a->usage > b->usage)
		return true;
	if (a->usage < b->usage)
		return false;

	bool localFind = a->lowName.contains(gSearchTxt);
	bool otherFind = b->lowName.contains(gSearchTxt);

	if (localFind  && !otherFind)
		return true;
	else if (!localFind && otherFind)
		return false;

	/*
	if (localFind != -1 && otherFind != -1) {
		if (localFind < otherFind)
			return true;
		else
			return false;
	}
*/
	int localLen = a->lowName.count();
	int otherLen = b->lowName.count();

	if (localLen < otherLen)
		return true;
	if (localLen > otherLen)
		return false;

	
	// Absolute tiebreaker to prevent loops
	if (a->fullPath < b->fullPath)
		return true;
	return false;
}


void FastCatalog::addItem(CatItem item) {
	catList.push_back(item);
	int index = catList.count() - 1;
	CatItem* pCatItem = &catList[index];
	foreach(QChar c, item.lowName) {
		catIndex[c].push_back(pCatItem);
	}
}

int FastCatalog::getUsage(const QString& path) {
	for(int i = 0; i < catList.size(); ++i) {
		if (path == catList[i].fullPath) {
			return catList[i].usage;
		}
	}	
	return 0;
}

void FastCatalog::incrementUsage(const CatItem & item) {
	for(int i = 0; i < catList.size(); ++i) {
		if (item == catList[i]) {
			catList[i].usage += 1;
			break;
		}
	}
}

int SlowCatalog::getUsage(const QString& path) {
	for(int i = 0; i < catList.size(); ++i) {
		if (path == catList[i].fullPath) {
			return catList[i].usage;
		}
	}	
	return 0;
}

void SlowCatalog::incrementUsage(const CatItem & item) {
	for(int i = 0; i < catList.size(); ++i) {
		if (item == catList[i]) {
			catList[i].usage += 1;
			break;
		}
	}
}

void SlowCatalog::addItem(CatItem item) {
	catList.push_back(item);
}

bool Catalog::matches(CatItem* item, QString& txt) {
	int size = item->lowName.count();
	int txtSize = txt.count();
	int curChar = 0;

	for(int i = 0; i < size; i++) {
		if (item->lowName[i] == txt[curChar]) {
			curChar++;
			if (curChar >= txtSize) {
				return true;
			}
		} 
	}
	return false;
}

void Catalog::searchCatalogs(QString txt, QList<CatItem> & out) {
	txt = txt.toLower();
	QList<CatItem*> catMatches = search(txt);
	
	// Now prioritize the catalog items
	gSearchTxt = txt;
	qSort(catMatches.begin(), catMatches.end(), CatLess);
	
	// Check for history matches
	QString location = "History/" + txt;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if(hist.count() == 2) {
		for(int i = 0; i < catMatches.count(); i++) {
			if (catMatches[i]->lowName == hist[0] &&
				catMatches[i]->fullPath == hist[1]) {
				CatItem* tmp = catMatches[i];
				catMatches.removeAt(i);
				catMatches.push_front(tmp);
			}
		}
	}

	// Load up the results
	int max = gSettings->value("GenOps/numresults", 10).toInt();
	for(int i = 0; i < max && i < catMatches.count(); i++) {
		out.push_back(*catMatches[i]);
	}
}


void Catalog::checkHistory(QString txt, QList<CatItem> & list)
{
	// Check for history matches
	QString location = "History/" + txt;
	QStringList hist;
	hist = gSettings->value(location, hist).toStringList();
	if(hist.count() == 2) {
		for(int i = 0; i < list.count(); i++) {
			if (list[i].lowName == hist[0] &&
				list[i].fullPath == hist[1]) {
				CatItem tmp = list[i];
				list.removeAt(i);
				list.push_front(tmp);
			}
		}
	}
}


QList<CatItem*> FastCatalog::search(QString searchTxt) {
	QList<CatItem*> ret;
	if (searchTxt == "") return ret;
//	QString lowSearch = searchTxt.toLower();
	
	// Find the smallest char list
	QChar leastCommon = -1;
	foreach(QChar c, searchTxt) {
		if (leastCommon == -1 || catIndex[c].count() < leastCommon)
			leastCommon = c;
	}
	
	if (catIndex[leastCommon].count() == 0)
		return ret;

	// Search the list
	for(int i = 0; i < catIndex[leastCommon].count(); ++i) {
		if (matches(catIndex[leastCommon][i], searchTxt)) {
			ret.push_back(catIndex[leastCommon][i]);
		}
	}
	return ret;
}

QList<CatItem*>  SlowCatalog::search(QString searchTxt) {
	QList<CatItem*> ret;
	if (searchTxt == "") return ret;
	QString lowSearch = searchTxt.toLower();

	for(int i = 0; i < catList.count(); ++i) {
		if (matches(&catList[i], lowSearch)) {
			ret.push_back(&catList[i]);
		}
	}
	return ret;
}
