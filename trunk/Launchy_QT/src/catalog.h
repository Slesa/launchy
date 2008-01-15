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

#ifndef CATALOG_H
#define CATALOG_H

#include <QString>
#include <QStringList>
#include <QBitArray>
#include <QIcon>
#include <QHash>
#include <QDataStream>
#include <QDir>
#include <QSet>

class CatItem {
public:
    
	QString fullPath;
	QString shortName;
	QString lowName;
	QString icon;
	int usage;
	void* data;
	int id;

	CatItem() {}


	CatItem(QString full, bool isDir = false) 
		: fullPath(full) {
			int last = fullPath.lastIndexOf("/");
			if (last == -1) {
				shortName = fullPath;

			} else {
				shortName = fullPath.mid(last+1);
				if (!isDir)
					shortName = shortName.mid(0,shortName.lastIndexOf("."));
			}

			lowName = shortName.toLower();
			data = NULL;
			usage = 0;
			id = 0;
	}


	CatItem(QString full, QString shortN) 
		: fullPath(full), shortName(shortN) 
	{
		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
		id = 0;
	}

	CatItem(QString full, QString shortN, uint i_d) 
		: id(i_d), fullPath(full), shortName(shortN)
	{
		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
	}

	CatItem(QString full, QString shortN, uint i_d, QString iconPath) 
		: id(i_d), fullPath(full), shortName(shortN), icon(iconPath)
	{
		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
	}

	CatItem(const CatItem &s) {
		fullPath = s.fullPath;
		shortName = s.shortName;
		lowName = s.lowName;
		icon = s.icon;
		usage = s.usage;
		data = s.data;
		id = s.id;
	}

	CatItem& operator=( const CatItem &s ) {
		fullPath = s.fullPath;
		shortName = s.shortName;
		lowName = s.lowName;
		icon = s.icon;
		usage = s.usage;
		data = s.data;
		id = s.id;
		return *this;
	}

	bool operator==(const CatItem& other) const{
		if (fullPath == other.fullPath)
			return true;
		return false;
	}

};



class InputData 
{
private:
	QString text;
	QSet<uint> labels;
	CatItem topResult;
	uint id;
public:
	QSet<uint>  getLabels() { return labels; }
	void setLabel(uint l) { labels.insert(l); }
	bool hasLabel(uint l) { return labels.contains(l); }

	void setID(uint i) { id = i; }
	uint getID() { return id; }
	QString  getText() { return text; }
	void setText(QString t) { text = t; }
	CatItem&  getTopResult() { return topResult; }
	void setTopResult(CatItem sr) { topResult = sr; }
	InputData() { id = 0; }
	InputData(QString str) : text(str) { id = 0;}
};

bool CatLess (CatItem* left, CatItem* right); 
bool CatLessNoPtr (CatItem & a, CatItem & b);

inline QDataStream &operator<<(QDataStream &out, const CatItem &item) {
	out << item.fullPath;
	out << item.shortName;
	out << item.lowName;
	out << item.icon;
	out << item.usage;
	out << item.id;
	return out;
}

inline QDataStream &operator>>(QDataStream &in, CatItem &item) {
	in >> item.fullPath;
	in >> item.shortName;
	in >> item.lowName;
	in >> item.icon;
	in >> item.usage;
	in >> item.id;
	return in;
}

class Catalog {
public:
	Catalog() {}
	virtual ~Catalog() {}
	virtual void addItem(CatItem item) = 0;
	virtual int count() = 0;
	virtual const CatItem & getItem(int) = 0;
	static bool matches(CatItem* item, QString& txt);

	void searchCatalogs(QString, QList<CatItem> & );
	virtual void incrementUsage(const CatItem& item) = 0;
	virtual int getUsage(const QString& path) = 0;
	void checkHistory(QString txt, QList<CatItem> & list);

private:	
	virtual QList<CatItem*> search(QString) = 0;

};



// The fast catalog searches quickly but 
// addition of items is slow and uses a lot of memory
class FastCatalog : public Catalog {
private:
	QList<CatItem> catList;
	QHash<QChar, QList<CatItem*> > catIndex;
public:
	FastCatalog() : Catalog() {}
	void addItem(CatItem item);
	QList<CatItem*> search(QString);
	int count() { return catList.count(); }
	const CatItem & getItem(int i) { return catList[i]; }
	void incrementUsage(const CatItem& item);
	int getUsage(const QString& path);
};

// The slow catalog searches slowly but
// adding items is fast and uses less memory
// than FastCatalog
class SlowCatalog : public Catalog {
private:
	QList<CatItem> catList;
public:
	SlowCatalog() : Catalog() {}
	void addItem(CatItem item);
	QList<CatItem*> search(QString);
	int count() { return catList.count(); }
	const CatItem & getItem(int i) { return catList[i]; }
	void incrementUsage(const CatItem& item);
	int getUsage(const QString& path);
};

#endif
