#include "catalog.h"
#include "catalog_builder.h"
#include "globals.h"
#include "main.h"
#include "options.h"

#include <QFile>
#include <QDataStream>

CatBuilder::CatBuilder(bool fromArchive) :
  buildFromStorage(fromArchive), curcat(NULL)
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
		memDirs = main->platform.GetInitialDirs();
	}

	for(int i = 0; i < memDirs.count(); ++i) {
		emit(catalogIncrement(100.0 * (float)i / (float) memDirs.count()));
		QString cur = memDirs[i].name;
		indexDirectory(memDirs[i].name, memDirs[i].types, memDirs[i].indexDirs, memDirs[i].indexExe, memDirs[i].depth);
	}
	emit(catalogIncrement(0.0));
}

void CatBuilder::indexDirectory(QString dir, QStringList filters, bool fdirs, bool fbin, int depth)
{
//	dir = QDir::toNativeSeparators(dir);
	QDir qd(dir);
	dir = qd.absolutePath();
	QStringList dirs = qd.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);

	if (depth > 0) {
		for (int i = 0; i < dirs.count(); ++i) {
			QString cur = dirs[i];
			if (cur.contains(".lnk")) continue;
			indexDirectory(dir + "/" + dirs[i], filters, fdirs, fbin, depth-1);
		}
	}
	int lStore = gTypes->getType("store");
	if (fdirs) {
		int dInt = gTypes->getType("directory");
		for(int i = 0; i < dirs.count(); ++i) {
			if (!indexed.contains(dir + "/" + dirs[i])) {
				CatItem item(dir + "/" + dirs[i]);
				item.setLabel(dInt, true);
				item.setLabel(lStore, true);
				if (curcat != NULL)
					item.usage = curcat->getUsage(item.fullPath);
				cat->addItem(item);
				indexed[dir + "/" + dirs[i]] = true;
			}
		}
	}
	if (fbin) {
		QStringList bins = qd.entryList(QDir::Files | QDir::Executable);
		int bInt = gTypes->getType("executable");
		for(int i = 0; i < bins.count(); ++i) {
			if (!indexed.contains(dir + "/" + bins[i])) {
				CatItem item(dir + "/" + bins[i]);
				item.setLabel(bInt, true);
				item.setLabel(lStore, true);
				if (curcat != NULL)
					item.usage = curcat->getUsage(item.fullPath);
				cat->addItem(item);
				indexed[dir + "/" + bins[i]] = true;
			}
		}
	}
	QStringList files = qd.entryList(filters, QDir::Files, QDir::Unsorted);
	for(int i = 0; i < files.count(); ++i) {
		if (!indexed.contains(dir + "/" + files[i])) {
			CatItem item(dir + "/" + files[i]);
			item.setLabel(lStore, true);
			if (curcat != NULL)
				item.usage = curcat->getUsage(item.fullPath);
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

	int storeLabel = gTypes->getType("store");
	for(int i = 0; i < this->cat->count(); i++) {
		CatItem item = cat->getItem(i);
		if (item.hasLabel(storeLabel))
			out << item;
	}

	// Zip the archive
	QFile file(dest);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Could not open database for writing";
	}
	file.write(qCompress(ba));
}