#ifndef CATALOG_BUILDER
#define CATALOG_BUILDER
#include "catalog.h"
#include "plugin_handler.h"
#include <QThread>

class CatBuilder : public QThread
{
	Q_OBJECT

private:
	bool buildFromStorage;
	Catalog* cat;
	Catalog* curcat;
	PluginHandler* plugins;
	QHash<QString, bool> indexed;

public:
	bool loadCatalog(QString);
	void storeCatalog(QString);
	void buildCatalog();
	void indexDirectory(QString dir, QStringList filters, bool fdirs, bool fbin, int depth);
	void setPreviousCatalog(Catalog* cata) {
		curcat = cata;
	}


	Catalog* getCatalog() { return cat; }
	CatBuilder(bool fromArchive, PluginHandler* plugs);
	CatBuilder(Catalog* catalog, PluginHandler* plugs) : plugins(plugs), buildFromStorage(false), cat(catalog) {}
	void run();

signals:
	void catalogFinished();
	void catalogIncrement(float);

};

#endif