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


#include <QThread>
#include <boost/shared_ptr.hpp>
#include "catalog_types.h"
#include "plugin_handler.h"


using namespace boost;


class CatalogBuilder : public QThread
{
	Q_OBJECT

public:
	CatalogBuilder(bool fromArchive, PluginHandler * plugs);
	CatalogBuilder(shared_ptr<Catalog> catalog, PluginHandler * plugs) : plugins(plugs), buildFromStorage(false), catalog(catalog) {}
	bool loadCatalog(const QString& filename);
	void storeCatalog(const QString& filename);
	void buildCatalog();
	void indexDirectory(const QString& dir, const QStringList& filters, bool fdirs, bool fbin, int depth);
	void setPreviousCatalog(shared_ptr<Catalog> cata)
	{
		currentCatalog = cata;
	}

	shared_ptr<Catalog> getCatalog()
	{
		return catalog;
	}
	void run();

signals:
	void catalogFinished();
	void catalogIncrement(float);

private:
	shared_ptr<Catalog> currentCatalog;
	shared_ptr<Catalog> catalog;
	PluginHandler* plugins;
	bool buildFromStorage;
	QHash<QString, bool> indexed;
};


#endif
