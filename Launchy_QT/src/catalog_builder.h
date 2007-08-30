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