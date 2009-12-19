/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin

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
#include "catalog_builder.h"
#include "globals.h"
#include "main.h"
#include "Directory.h"


CatalogBuilder::CatalogBuilder(PluginHandler* plugs) :
	plugins(plugs)
{
	catalog.reset((Catalog*)createCatalog());
}


CatalogBuilder::CatalogBuilder(PluginHandler* plugs, shared_ptr<Catalog> currentCatalog) :
	plugins(plugs),
	currentCatalog(currentCatalog)
{
	catalog.reset((Catalog*)createCatalog());
}


Catalog* CatalogBuilder::createCatalog()
{
	if (gSettings->value("GenOps/fastindexer",false).toBool())
		return new FastCatalog();
	return new SlowCatalog();
}


void CatalogBuilder::run()
{
	buildCatalog();
	emit catalogFinished();
}


void CatalogBuilder::buildCatalog()
{
	QList<Directory> memDirs;
	int size = gSettings->beginReadArray("directories");
	for (int i = 0; i < size; ++i)
	{
		gSettings->setArrayIndex(i);
		Directory tmp;
		tmp.name = gSettings->value("name").toString();
		tmp.types = gSettings->value("types").toStringList();
		tmp.indexDirs = gSettings->value("indexDirs", false).toBool();
		tmp.indexExe = gSettings->value("indexExes", false).toBool();
		tmp.depth = gSettings->value("depth", 100).toInt();
		memDirs.append(tmp);
	}
	gSettings->endArray();

	if (memDirs.count() == 0)
	{
		memDirs = platform->getDefaultCatalogDirectories();
	}

	for (int i = 0; i < memDirs.count(); ++i)
	{
		emit(catalogIncrement(100.0f * (float)(i+1) / (float) memDirs.count()));
		QString cur = platform->expandEnvironmentVars(memDirs[i].name);
		indexDirectory(cur, memDirs[i].types, memDirs[i].indexDirs, memDirs[i].indexExe, memDirs[i].depth);
	}

	QList<CatItem> items;
	plugins->getCatalogs(&items);
	foreach(CatItem item, items)
	{
		if (currentCatalog != NULL)
		{
			item.usage = currentCatalog->getUsage(item.fullPath);
		}
		catalog->addItem(item);
	}

	emit(catalogIncrement(0.0));
}


void CatalogBuilder::indexDirectory(const QString& directory, const QStringList& filters, bool fdirs, bool fbin, int depth)
{
	QString dir = QDir::toNativeSeparators(directory);
	QDir qd(dir);
	dir = qd.absolutePath();
	QStringList dirs = qd.entryList(QDir::AllDirs);

	if (depth > 0)
	{
		for (int i = 0; i < dirs.count(); ++i)
		{
			if (!dirs[i].startsWith("."))
			{
				QString cur = dirs[i];
				if (!cur.contains(".lnk"))
				{
					indexDirectory(dir + "/" + dirs[i], filters, fdirs, fbin, depth-1);
				}
			}
		}
	}

	if (fdirs)
	{
		for (int i = 0; i < dirs.count(); ++i)
		{
			if (!dirs[i].startsWith(".") && !indexed.contains(dir + "/" + dirs[i]))
			{
				bool isShortcut = dirs[i].endsWith(".lnk", Qt::CaseInsensitive);

				CatItem item(dir + "/" + dirs[i], !isShortcut);
				if (currentCatalog != NULL)
					item.usage = currentCatalog->getUsage(item.fullPath);
				catalog->addItem(item);
				indexed[dir + "/" + dirs[i]] = true;
			}
		}
	}
	else
	{
		// Grab any shortcut directories 
		// This is to work around a QT weirdness that treats shortcuts to directories as actual directories
		for (int i = 0; i < dirs.count(); ++i)
		{
			if (!dirs[i].startsWith(".") && dirs[i].endsWith(".lnk",Qt::CaseInsensitive))
			{
				if (!indexed.contains(dir + "/" + dirs[i]))
				{
					CatItem item(dir + "/" + dirs[i], true);
					if (currentCatalog != NULL)
						item.usage = currentCatalog->getUsage(item.fullPath);
					catalog->addItem(item);
					indexed[dir + "/" + dirs[i]] = true;
				}
			}
		}
	}

	if (fbin)
	{
		QStringList bins = qd.entryList(QDir::Files | QDir::Executable);
		for (int i = 0; i < bins.count(); ++i)
		{
			if (!indexed.contains(dir + "/" + bins[i]))
			{
				CatItem item(dir + "/" + bins[i]);
				if (currentCatalog != NULL)
					item.usage = currentCatalog->getUsage(item.fullPath);
				catalog->addItem(item);
				indexed[dir + "/" + bins[i]] = true;
			}
		}
	}

	// Don't want a null file filter, that matches everything..
	if (filters.count() == 0)
		return;

	QStringList files = qd.entryList(filters, QDir::Files | QDir::System, QDir::Unsorted );
	for (int i = 0; i < files.count(); ++i)
	{
		if (!indexed.contains(dir + "/" + files[i]))
		{
			CatItem item(dir + "/" + files[i]);
			if (currentCatalog != NULL)
				item.usage = currentCatalog->getUsage(item.fullPath);
			platform->alterItem(&item);
			catalog->addItem(item);
			indexed[dir + "/" + files[i]] = true;
		}
	}
}
