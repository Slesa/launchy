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
	QBitArray labels;
	QIcon icon;
	int usage;
	void* data;
	int id;

	CatItem() {}

	
	CatItem(QString full) 
		: fullPath(full) {

		int last = fullPath.lastIndexOf("/");
		if (last == -1) {
				shortName = fullPath;
		
		} else {
			shortName = fullPath.mid(last+1);
			shortName = shortName.mid(0,shortName.lastIndexOf("."));
		}

		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
	}
	

	CatItem(QString full, QString shortN) 
		: fullPath(full), shortName(shortN) 
	{
		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
	}

	CatItem(QString full, QString shortN, uint i_d) 
		: id(i_d), fullPath(full), shortName(shortN)
	{
		lowName = shortName.toLower();
		data = NULL;
		usage = 0;
	}

	CatItem(const CatItem &s) {
		fullPath = s.fullPath;
		shortName = s.shortName;
		lowName = s.lowName;
		labels = s.labels;
		icon = s.icon;
		usage = s.usage;
		data = s.data;
	}

	CatItem& operator=( const CatItem &s ) {
		fullPath = s.fullPath;
		shortName = s.shortName;
		lowName = s.lowName;
		labels = s.labels;
		icon = s.icon;
		usage = s.usage;
		data = s.data;
		return *this;
	}

	bool operator==(const CatItem& other) const{
		if (fullPath == other.fullPath)
			return true;
		return false;
	}

	void setLabel(int num, bool val) {
		if (num+1 > labels.size()) {
			labels.resize(num+1);
		}
		labels.setBit(num, val);
	}

	bool hasLabel(int num) {
		if (num > labels.size())
			return false;
		return labels.at(num);
	}
};


class InputData 
{
private:
	QString text;
	QSet<uint> labels;
	CatItem topResult;
public:
	QSet<uint> & getLabels() { return labels; }
	void setLabel(uint l) { labels.insert(l); }
	bool hasLabel(uint l) { return labels.contains(l); }
	QString & getText() { return text; }
	void setText(QString t) { text = t; }
	CatItem & getTopResult() { return topResult; }
	void setTopResult(CatItem sr) { topResult = sr; }
	InputData() {}
	InputData(QString str) : text(str) {}
};

bool CatLess (CatItem* left, CatItem* right); 

inline QDataStream &operator<<(QDataStream &out, const CatItem &item) {
	out << item.fullPath;
	out << item.shortName;
	out << item.lowName;
	out << item.labels;
	out << item.icon;
	out << item.usage;
	return out;
}

inline QDataStream &operator>>(QDataStream &in, CatItem &item) {
	in >> item.fullPath;
	in >> item.shortName;
	in >> item.lowName;
	in >> item.labels;
	in >> item.icon;
	in >> item.usage;
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