#pragma once

#include "catalog.h"


/** This class does not pertain to plugins */
class Catalog
{
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


/** This class does not pertain to plugins */
// The fast catalog searches quickly but 
// addition of items is slow and uses a lot of memory
class FastCatalog : public Catalog
{
private:
	QVector<CatItem> catList;
//	QList<CatItem> catList;
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


/** This class does not pertain to plugins */
// The slow catalog searches slowly but
// adding items is fast and uses less memory
// than FastCatalog
class SlowCatalog : public Catalog
{
private:
	QVector<CatItem> catList;
//	QList<CatItem> catList;
public:
	SlowCatalog() : Catalog() {}
	void addItem(CatItem item);
	QList<CatItem*> search(QString);
	int count() { return catList.count(); }
	const CatItem & getItem(int i) { return catList[i]; }
	void incrementUsage(const CatItem& item);
	int getUsage(const QString& path);
};
