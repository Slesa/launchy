
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
#include "catalog_builder.h"
#include "globals.h"
#include "main.h"
#include "options.h"
#include <QDebug>

#include <QFile>
#include <QDataStream>

CatBuilder::CatBuilder(bool fromArchive, PluginHandler* plugs) :
  buildFromStorage(fromArchive), curcat(NULL), plugins(plugs)
{
	if (gSettings->value("GenOps/fastindexer",false).toBool())
		cat = (Catalog*) new FastCatalog();
	else
		cat = (Catalog*) new SlowCatalog();
}

void CatBuilder::run() {
	QString dest = gSettings->fileName();
	int lastSlash = dest.lastIndexOf(QLatin1Char('/'));
	if (lastSlash == -1)
		return;
	dest = dest.mid(0, lastSlash);
	dest += "/Launchy.db";


	if (buildFromStorage && loadCatalog(dest)) {
	} else {
		buildCatalog();
		storeCatalog(dest);
	}
	emit catalogFinished();
}

void CatBuilder::buildCatalog() {
	MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
	if (main == NULL) return;
	QList<Directory> memDirs;
	int size = gSettings->beginReadArray("directories");
	for(int i = 0; i < size; ++i) {
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

	if (memDirs.count() == 0) {
		memDirs = main->platform->GetInitialDirs();
	}

	for(int i = 0; i < memDirs.count(); ++i) {
		emit(catalogIncrement(100.0 * (float)(i+1) / (float) memDirs.count()));
		QString cur = main->platform->expandEnvironmentVars(memDirs[i].name);
		indexDirectory(cur, memDirs[i].types, memDirs[i].indexDirs, memDirs[i].indexExe, memDirs[i].depth);
	}
	
	QList<CatItem> pitems;
	plugins->getCatalogs(&pitems);
	foreach(CatItem item, pitems) {
		if (curcat != NULL) {
			item.usage = curcat->getUsage(item.fullPath);
		}
		cat->addItem(item);
	}

	emit(catalogIncrement(0.0));
}

void CatBuilder::indexDirectory(QString dir, QStringList filters, bool fdirs, bool fbin, int depth)
{
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (!main) return;
    dir = QDir::toNativeSeparators(dir);
    
	QDir qd(dir);
	dir = qd.absolutePath();
	QStringList dirs = qd.entryList(QDir::AllDirs);

	if (depth > 0) {
		for (int i = 0; i < dirs.count(); ++i) {
			if (dirs[i].startsWith(".")) continue;
			QString cur = dirs[i];
			if (cur.contains(".lnk")) continue;
			indexDirectory(dir + "/" + dirs[i], filters, fdirs, fbin, depth-1);
		}
	}

	if (fdirs) {
		for(int i = 0; i < dirs.count(); ++i) {
			if (dirs[i].startsWith(".")) continue;
			if (!indexed.contains(dir + "/" + dirs[i])) {
				bool isShortcut = dirs[i].endsWith(".lnk", Qt::CaseInsensitive);

					
				CatItem item(dir + "/" + dirs[i], !isShortcut);
				if (curcat != NULL)
					item.usage = curcat->getUsage(item.fullPath);
				cat->addItem(item);
				indexed[dir + "/" + dirs[i]] = true;
			}
		}
	} else {
		// Grab any shortcut directories 
		// This is to work around a QT weirdness that treats shortcuts to directories as actual directories
		for(int i = 0; i < dirs.count(); ++i) {
			if (dirs[i].startsWith(".")) continue;
			if (dirs[i].endsWith(".lnk",Qt::CaseInsensitive)) {
				if (!indexed.contains(dir + "/" + dirs[i])) {
					CatItem item(dir + "/" + dirs[i], true);
					if (curcat != NULL)
						item.usage = curcat->getUsage(item.fullPath);
					cat->addItem(item);
					indexed[dir + "/" + dirs[i]] = true;
				}
			}
		}
	}

	

	if (fbin) {
		QStringList bins = qd.entryList(QDir::Files | QDir::Executable);
		for(int i = 0; i < bins.count(); ++i) {
			if (!indexed.contains(dir + "/" + bins[i])) {
				CatItem item(dir + "/" + bins[i]);
				if (curcat != NULL)
					item.usage = curcat->getUsage(item.fullPath);
				cat->addItem(item);
				indexed[dir + "/" + bins[i]] = true;
			}
		}
	}
	
	// Don't want a null file filter, that matches everything..
	if (filters.count() == 0)
		return;

	QStringList files = qd.entryList(filters, QDir::Files | QDir::System, QDir::Unsorted );
	for(int i = 0; i < files.count(); ++i) {
		if (!indexed.contains(dir + "/" + files[i])) {
			CatItem item(dir + "/" + files[i]);
			if (curcat != NULL)
				item.usage = curcat->getUsage(item.fullPath);
			main->platform->alterItem(&item);
			cat->addItem(item);
			indexed[dir + "/" + files[i]] = true;
		}
	}
}


bool CatBuilder::loadCatalog(QString dest) {

    QFile inFile(dest);
	if (!inFile.open(QIODevice::ReadOnly)) {
		return false;
	}
    QByteArray ba = inFile.readAll();

	QByteArray unzipped = qUncompress(ba);

	QDataStream in(&unzipped, QIODevice::ReadOnly);
	in.setVersion(QDataStream::Qt_4_2);

	while(!in.atEnd()) {
		CatItem item;
		in >> item;
		cat->addItem(item);
	}
	

	return true;
}

void CatBuilder::storeCatalog(QString dest) {

	QByteArray ba;
	QDataStream out(&ba, QIODevice::ReadWrite); 
	out.setVersion(QDataStream::Qt_4_2);

	for(int i = 0; i < this->cat->count(); i++) {
		CatItem item = cat->getItem(i);
		out << item;
	}

	// Zip the archive
	QFile file(dest);
	if (!file.open(QIODevice::WriteOnly)) {
	    //		qDebug() << "Could not open database for writing";
	}
	file.write(qCompress(ba));
}
