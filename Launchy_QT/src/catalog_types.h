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

#pragma once

#include "catalog.h"


/** This class does not pertain to plugins */
class Catalog
{
public:
	Catalog() {}
	virtual ~Catalog() {}
	bool load(const QString& filename);
	bool save(const QString& filename);
	virtual void addItem(const CatItem& item) = 0;
	virtual int count() = 0;
	virtual const CatItem& getItem(int) = 0;
	static bool matches(CatItem* item, const QString& match);
	static QString decorateText(const QString& text, const QString& match, bool outputRichText = false);

	void searchCatalogs(const QString&, QList<CatItem>&);
	virtual void incrementUsage(const CatItem& item) = 0;
	virtual void clearUsage(const CatItem& item) = 0;
	virtual int getUsage(const QString& path) = 0;
	void promoteRecentlyUsedItems(const QString& text, QList<CatItem> & list);

private:	
	virtual QList<CatItem*> search(const QString&) = 0;
};


/** This class does not pertain to plugins */
// The fast catalog searches quickly but 
// addition of items is slow and uses a lot of memory
class FastCatalog : public Catalog
{
public:
	FastCatalog() : Catalog() {}
	void addItem(const CatItem& item);
	QList<CatItem*> search(const QString&);
	int count() { return catList.count(); }
	const CatItem& getItem(int i) { return catList[i]; }
	void incrementUsage(const CatItem& item);
	void clearUsage(const CatItem& item);
	int getUsage(const QString& path);

private:
	QVector<CatItem> catList;
	QHash<QChar, QList<CatItem*> > catIndex;
};


/** This class does not pertain to plugins */
// The slow catalog searches slowly but
// adding items is fast and uses less memory
// than FastCatalog
class SlowCatalog : public Catalog
{
public:
	SlowCatalog() : Catalog() {}
	void addItem(const CatItem& item);
	QList<CatItem*> search(const QString&);
	int count() { return catList.count(); }
	const CatItem& getItem(int i) { return catList[i]; }
	void incrementUsage(const CatItem& item);
	void clearUsage(const CatItem& item);
	int getUsage(const QString& path);

private:
	QVector<CatItem> catList;
};
