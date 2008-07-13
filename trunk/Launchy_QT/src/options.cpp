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

#include "options.h"
#include "main.h"
#include "globals.h"
#include "plugin_handler.h"
#include <QSettings>
#include <QDir>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QFileDialog>
#include <QTextStream>

OptionsDlg::OptionsDlg(QWidget * parent)
    : QDialog(parent) 
{
    setupUi(this);
    curPlugin = -1;
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    // Load General Options
    genAlwaysShow->setChecked(gSettings->value("GenOps/alwaysshow", false).toBool());
    genAlwaysTop->setChecked(gSettings->value("GenOps/alwaystop", false).toBool());
    genPortable->setChecked(gSettings->value("GenOps/isportable", false).toBool());
    genHideFocus->setChecked(gSettings->value("GenOps/hideiflostfocus", false).toBool());
    genCenter->setChecked(gSettings->value("GenOps/alwayscenter", false).toBool());
    genFastIndex->setChecked(gSettings->value("GenOps/fastindexer",false).toBool());
    genUpdateCheck->setChecked(gSettings->value("GenOps/updatecheck", true).toBool());
    genShowHidden->setChecked(gSettings->value("GenOps/showHiddenFiles", false).toBool());
    genCondensed->setChecked(gSettings->value("GenOps/condensedView",false).toBool());
    genUpMinutes->setText(gSettings->value("GenOps/updatetimer", "10").toString());
    genMaxViewable->setText(gSettings->value("GenOps/numviewable", "4").toString());
    genNumResults->setText(gSettings->value("GenOps/numresults", "10").toString());
    genOpaqueness->setValue(gSettings->value("GenOps/opaqueness", 100).toInt());
    genFadeIn->setValue(gSettings->value("GenOps/fadein", 0).toInt());
    genFadeOut->setValue(gSettings->value("GenOps/fadeout", 20).toInt());
    connect(genOpaqueness, SIGNAL(sliderMoved(int)), main, SLOT(setOpaqueness(int)));
    metaKeys << tr("Alt") << tr("Win") << tr("Shift") << tr("Control");
    iMetaKeys << Qt::AltModifier << Qt::MetaModifier << Qt::ShiftModifier << Qt::ControlModifier;
    
    actionKeys << tr("Space") << tr("Tab") << tr("Backspace") << tr("Enter") << tr("Esc") << tr("Home") << 
	tr("End") << tr("Pause") << tr("Print") << tr("Up") << tr("Down") << tr("Left") << tr("Right") << tr("F1") <<
	tr("F2") << tr("F3") << tr("F4") << tr("F5") << tr("F6") << tr("F7") << tr("F8") << tr("F9") << tr("F10") <<
	tr("F11") << tr("F12") << tr("F13");
    
    
    for(int i = 'A'; i <= 'Z'; i++) 
	actionKeys << QString(QChar(i));
    iActionKeys << Qt::Key_Space << Qt::Key_Tab << Qt::Key_Backspace << Qt::Key_Enter << Qt::Key_Escape << Qt::Key_Home <<
	Qt::Key_End << Qt::Key_Pause << Qt::Key_Print << Qt::Key_Up << Qt::Key_Down << Qt::Key_Left << Qt::Key_Right << Qt::Key_F1 <<
	Qt::Key_F2 << Qt::Key_F3 << Qt::Key_F4 << Qt::Key_F5 << Qt::Key_F6 << Qt::Key_F7 << Qt::Key_F8 << Qt::Key_F9 << Qt::Key_F10 <<
	Qt::Key_F11 << Qt::Key_F12 << Qt::Key_F13;
    
    for(int i = 'A'; i <= 'Z'; i++) 
	iActionKeys << i;
    
    
    // Find the current hotkey
    QKeySequence keys = gSettings->value("Options/hotkey", QKeySequence(Qt::ControlModifier +  Qt::Key_Space)).value<QKeySequence>();
    int curMeta = gSettings->value("GenOps/hotkeyModifier", Qt::AltModifier).toInt();
    int curAction = gSettings->value("GenOps/hotkeyAction", Qt::Key_Space).toInt();
    for(int i = 0; i < metaKeys.count(); ++i) {
	genModifierBox->addItem(metaKeys[i]);
	if (iMetaKeys[i] == curMeta) 
	    genModifierBox->setCurrentIndex(i);
    }
    for(int i = 0; i < actionKeys.count(); ++i) {
	genKeyBox->addItem(actionKeys[i]);
	if (iActionKeys[i] == curAction) 
	    genKeyBox->setCurrentIndex(i);
    }
    
    
    // Load up the skins list
    QString skinName = gSettings->value("GenOps/skin", main->dirs["defSkin"][0]).toString();

    connect(skinList, SIGNAL(currentTextChanged(const QString)), this, SLOT(skinChanged(const QString)));

    int skinRow = 0;

    foreach(QString szDir, main->dirs["skins"]) {
	QDir dir(szDir);
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

	foreach(QString d, dirs) {

	    QFile f(dir.absolutePath() + "/" + d + "/misc.txt");
	    // Only look for 2.0+ skins
	    if (!f.exists()) continue;
	    
	    QListWidgetItem * item = new QListWidgetItem(d);
	    item->setData(Qt::UserRole, szDir);
	    skinList->addItem(item);
	    
	    if (skinName.compare(szDir + "/" + d, Qt::CaseInsensitive) == 0)
		skinRow = skinList->count() - 1;
	}
    }

    skinList->setCurrentRow(skinRow);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    
    // Load the directories and types
    connect(catDirectories, SIGNAL(currentRowChanged(int)), this, SLOT(dirChanged(int)));
    connect(catDirPlus, SIGNAL(clicked(bool)), this, SLOT(catDirPlusClicked(bool)));
    connect(catDirMinus, SIGNAL(clicked(bool)), this, SLOT(catDirMinusClicked(bool)));
    connect(catTypesPlus, SIGNAL(clicked(bool)), this, SLOT(catTypesPlusClicked(bool)));
    connect(catTypesMinus, SIGNAL(clicked(bool)), this, SLOT(catTypesMinusClicked(bool)));
    connect(catCheckDirs, SIGNAL(stateChanged(int)), this, SLOT(catTypesDirChanged(int)));
    connect(catCheckBinaries, SIGNAL(stateChanged(int)), this, SLOT(catTypesExeChanged(int)));
    connect(catDepth, SIGNAL(valueChanged(int)),this, SLOT(catDepthChanged(int)));
    connect(catRescan, SIGNAL(clicked(bool)), this, SLOT(catRescanClicked(bool)));
    connect(catDirectories, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(catDirTextChanged(QListWidgetItem* )));
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
	catDirectories->addItem(memDirs[i].name);
	QListWidgetItem* it = catDirectories->item(i);
	it->setFlags(it->flags() | Qt::ItemIsEditable);
    }
    if (catDirectories->count() > 0)
	catDirectories->setCurrentRow(0);
    
    genOpaqueness->setRange(15,100);

    if (main->catalog != NULL) {
	QString txt = tr("Index has ");
	txt += QString::number(main->catalog->count());
	txt += tr(" items");
	catSize->setText(txt);
    }
    if (gBuilder != NULL) {
	connect(gBuilder, SIGNAL(catalogIncrement(float)), this, SLOT(catProgressUpdated(float)));
	connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
    }
    // Load up the plugins		
    connect(plugList, SIGNAL(currentRowChanged(int)), this, SLOT(pluginChanged(int)));
    connect(plugList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(pluginItemChanged(QListWidgetItem*)));
    main->plugins.loadPlugins();
    foreach(PluginInfo info, main->plugins.getPlugins()) {
	plugList->addItem(info.name);
	QListWidgetItem* item = plugList->item(plugList->count()-1);
	item->setData(Qt::UserRole, info.id);
	item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	if (info.loaded)
	    item->setCheckState(Qt::Checked);
	else
	    item->setCheckState(Qt::Unchecked);
    }
    if (plugList->count() > 0) {
	plugList->setCurrentRow(0);
    }
    aboutVer->setText(QString(tr("This is Launchy version ")) + QString(LAUNCHY_VERSION_STRING));
    
    tabWidget->setCurrentIndex(0);
}
OptionsDlg::~OptionsDlg() {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    
    // Close any current plugin dialogs
    //	if (curPlugin >= 0) {
    //		QListWidgetItem* item = plugList->item(curPlugin);
    //		main->plugins.endDialog(item->data(3).toUInt());
    //	}
}

void OptionsDlg::accept() {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    if (gSettings == NULL) return;
    
    // Save General Options
    gSettings->setValue("GenOps/alwaysshow", genAlwaysShow->isChecked());
    gSettings->setValue("GenOps/alwaystop", genAlwaysTop->isChecked());
    gSettings->setValue("GenOps/isportable", genPortable->isChecked());
    gSettings->setValue("GenOps/updatecheck", genUpdateCheck->isChecked());
    gSettings->setValue("GenOps/hideiflostfocus", genHideFocus->isChecked());
    gSettings->setValue("GenOps/alwayscenter", genCenter->isChecked());
    gSettings->setValue("GenOps/fastindexer", genFastIndex->isChecked());
    gSettings->setValue("GenOps/showHiddenFiles", genShowHidden->isChecked());
    gSettings->setValue("GenOps/condensedView", genCondensed->isChecked());
    gSettings->setValue("GenOps/updatetimer", genUpMinutes->text());
    gSettings->setValue("GenOps/numviewable", genMaxViewable->text());
    gSettings->setValue("GenOps/numresults", genNumResults->text());
    gSettings->setValue("GenOps/hotkeyModifier", iMetaKeys[genModifierBox->currentIndex()]);
    gSettings->setValue("GenOps/hotkeyAction", iActionKeys[genKeyBox->currentIndex()]);
    gSettings->setValue("GenOps/opaqueness", genOpaqueness->value());
    gSettings->setValue("GenOps/fadein", genFadeIn->value());
    gSettings->setValue("GenOps/fadeout", genFadeOut->value());
    
    // Apply General Options	
    main->setAlwaysShow(genAlwaysShow->isChecked());
    main->setAlwaysTop(genAlwaysTop->isChecked());
    main->setPortable(genPortable->isChecked());
    main->setHotkey(iMetaKeys[genModifierBox->currentIndex()], iActionKeys[genKeyBox->currentIndex()]);
    main->setCondensed(genCondensed->isChecked());
    main->setOpaqueness(genOpaqueness->value());
    
    
    // Apply Skin Options
    QString prevSkinName = gSettings->value("GenOps/skin", main->dirs["defSkin"][0]).toString();
    
    if (skinList->currentRow() >= 0 && skinList->currentItem()->text() != prevSkinName) {
	QString path = skinList->currentItem()->data(Qt::UserRole).toString();
	gSettings->setValue("GenOps/skin", path + "/" + skinList->currentItem()->text());
	main->setSkin(path, skinList->currentItem()->text());
    }
    
    // Apply Directory Options
    gSettings->beginWriteArray("directories");
    for(int i = 0; i < memDirs.count(); ++i) {
	gSettings->setArrayIndex(i);
	gSettings->setValue("name", memDirs[i].name);
	gSettings->setValue("types", memDirs[i].types);
	gSettings->setValue("indexDirs", memDirs[i].indexDirs);
	gSettings->setValue("indexExes", memDirs[i].indexExe);
	gSettings->setValue("depth", memDirs[i].depth);
    }
    
    gSettings->endArray();
    
    
    // Compare the checks to the plugins
    QHash<uint, PluginInfo> plugins = main->plugins.getPlugins();
    bool changed = false;
    
    
    if (changed) {
	main->plugins.loadPlugins();
    }
    
    if (curPlugin >= 0) {
	QListWidgetItem* item = plugList->item(curPlugin);
	main->plugins.endDialog(item->data(Qt::UserRole).toUInt(), true);
    }
    
    gSettings->sync();
    QDialog::accept();
}

void OptionsDlg::tabChanged(int tab) {
    tab = tab; // Compiler warning
    // Redraw the current skin
    // (necessary because of dialog resizing issues)
    if (tabWidget->currentWidget()->objectName() == "Skins") {
	int row = skinList->currentRow();
	skinList->setCurrentRow(-1);
	skinList->setCurrentRow(row);
    }
}

void OptionsDlg::pluginChanged(int row) {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    
    if (plugBox->layout() != NULL)
	for(int i = 1; i < plugBox->layout()->count(); i++) 
	    plugBox->layout()->removeItem(plugBox->layout()->itemAt(i));
    
    
    // Close any current plugin dialogs
    if (curPlugin >= 0) {
	QListWidgetItem* item = plugList->item(curPlugin);
	main->plugins.endDialog(item->data(Qt::UserRole).toUInt(), true);
    }
    
    // Open the new plugin dialog
    curPlugin = row;
    if (row < 0) return;	
    QListWidgetItem* item = plugList->item(row);
    QWidget* win = main->plugins.doDialog(plugBox, item->data(Qt::UserRole).toUInt());
    
    if (win != NULL) {
	if (plugBox->layout() != NULL)
	    plugBox->layout()->addWidget(win);
	win->show();
    }
}

void OptionsDlg::pluginItemChanged(QListWidgetItem* iz) {
    
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    int row = plugList->currentRow();
    if (row == -1) return;
    
    // Close any current plugin dialogs
    if (curPlugin >= 0) {
	QListWidgetItem* item = plugList->item(curPlugin);
	main->plugins.endDialog(item->data(Qt::UserRole).toUInt(), true);
    }
    
    // Write out the new config
    gSettings->beginWriteArray("plugins");
    for(int i = 0; i < plugList->count(); i++) {
	QListWidgetItem* item = plugList->item(i);
	gSettings->setArrayIndex(i);
	gSettings->setValue("id", item->data(Qt::UserRole).toUInt());
	if (item->checkState() == Qt::Checked) {
	    gSettings->setValue("load", true);
	}
	else {
	    gSettings->setValue("load", false);
	}
    }
    gSettings->endArray();
    
    // Reload the plugins
    main->plugins.loadPlugins();
    
    // If enabled, reload the dialog
    if (iz->checkState() == Qt::Checked) {
	main->plugins.doDialog(plugBox, iz->data(Qt::UserRole).toUInt());
    }
    
}

void OptionsDlg::catProgressUpdated(float val) {
    catProgress->setValue((int) val);
}
void OptionsDlg::catalogBuilt() {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main->catalog != NULL)
	catSize->setText(tr("Index has ") + QString::number(main->catalog->count()) + tr(" items"));
}
void OptionsDlg::catRescanClicked(bool val) {
    val = val; // Compiler warning
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    
    // Apply Directory Options
    gSettings->beginWriteArray("directories");
    for(int i = 0; i < memDirs.count(); ++i) {
	gSettings->setArrayIndex(i);
	gSettings->setValue("name", memDirs[i].name);
	gSettings->setValue("types", memDirs[i].types);
	gSettings->setValue("indexDirs", memDirs[i].indexDirs);
	gSettings->setValue("indexExes", memDirs[i].indexExe);
	gSettings->setValue("depth", memDirs[i].depth);
    }
    
    gSettings->endArray();
    
    if (gBuilder == NULL) {
	gBuilder = new CatBuilder(false, &main->plugins);
	gBuilder->setPreviousCatalog(main->catalog);
	connect(gBuilder, SIGNAL(catalogFinished()), main, SLOT(catalogBuilt()));
	connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
	connect(gBuilder, SIGNAL(catalogIncrement(float)), this, SLOT(catProgressUpdated(float)));
	gBuilder->start(QThread::IdlePriority);
    }
}

void OptionsDlg::catTypesDirChanged(int state) {
    state = state; // Compiler warning
    int row = catDirectories->currentRow();
    if (row == -1) return;
    memDirs[row].indexDirs = catCheckDirs->isChecked();
}

void OptionsDlg::catTypesExeChanged(int state) {
    state = state; // Compiler warning
    int row = catDirectories->currentRow();
    if (row == -1) return;
    memDirs[row].indexExe = catCheckBinaries->isChecked();
}

void OptionsDlg::catDirTextChanged( QListWidgetItem * item ) {
    int row = catDirectories->currentRow();
    if (row == -1) return;
    if (item != catDirectories->item(row))
	return;		
    memDirs[row].name = item->text();
}

void OptionsDlg::dirChanged(int row) {
    if (row == -1) return;
    
    catTypes->clear();
    foreach(QString str, memDirs[row].types) {
	catTypes->addItem(str);
    }	
    catCheckDirs->setChecked(memDirs[row].indexDirs);
    catCheckBinaries->setChecked(memDirs[row].indexExe);
    catDepth->setValue(memDirs[row].depth);
};

void OptionsDlg::catDirPlusClicked(bool c) {
    c = c; // Compiler warning
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"),
						    lastDir,
						    QFileDialog::ShowDirsOnly );
    if (dir == "")
	return;
    lastDir = dir;
    
    QString nativeDir = QDir::toNativeSeparators(dir);
    Directory tmp;
    tmp.name = nativeDir;
    memDirs.append(tmp);
    
    catTypes->clear();
    catDirectories->addItem(nativeDir);
    QListWidgetItem* it = catDirectories->item(catDirectories->count()-1);
    it->setFlags(it->flags() | Qt::ItemIsEditable);
    catDirectories->setCurrentRow(catDirectories->count()-1);
}

void OptionsDlg::catTypesPlusClicked(bool c) {
    c = c; // Compiler warning
    if (catDirectories->currentRow() == -1) return;
    QString txt = catTypeEdit->text();
    if (txt == "") return;
    if (txt.startsWith("."))
	txt = "*" + txt;
    catTypes->addItem(txt);
    memDirs[catDirectories->currentRow()].types << txt;
    catTypeEdit->clear();
}

void OptionsDlg::catTypesMinusClicked(bool c) {
    c = c; // Compiler warning
    int row = catTypes->currentRow();
    int dirRow = catDirectories->currentRow();
    if (row == -1 || dirRow == -1) return;
    
    memDirs[dirRow].types.removeAt(row);
    catTypes->takeItem(row);
}

void OptionsDlg::catDepthChanged(int d) {
    int row = catDirectories->currentRow();
    if (row != -1)
	memDirs[row].depth = d;
}
void OptionsDlg::catDirMinusClicked(bool c) {
    c = c; // Compiler warning
    int row = catDirectories->currentRow();
    
    catDirectories->takeItem(row);
    catTypes->clear();
    
    memDirs.removeAt(row);
    
    
    if (catDirectories->count() > 0)
	catDirectories->setCurrentRow(catDirectories->count() - 1);
}

void OptionsDlg::skinChanged(const QString newSkin) {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    
    if (skinList->currentRow() == -1)
	return;

    QString path = skinList->currentItem()->data(Qt::UserRole).toString();  

    // Load up the author file
    QFile author(path + "/" + newSkin + "/author.txt"); 
    if (!author.open(QIODevice::ReadOnly)) {
	authorInfo->setText("");
	skinPreview->clear();
	return;
    }
    
    QTextStream in(&author);
    
    QString line, total;
    line = in.readLine();
    while (!line.isNull())
	{
	    total += line + "\n";
	    line = in.readLine();
	} 
    authorInfo->setText(total);
    author.close();
    
    // Set the image preview
    QPixmap pix;
    QString sDir(path + "/" + newSkin + "/");
        
    if (pix.load(sDir + "background.png")) {
	if (!main->platform->SupportsAlphaBorder() && QFile::exists(sDir + "background_nc.png"))
	    pix.load(sDir + "background_nc.png");
	if (!main->platform->SupportsAlphaBorder() && QFile::exists(sDir + "mask_nc.png"))
	    pix.setMask(QPixmap(sDir + "mask_nc.png"));
	else
	    pix.setMask(QPixmap(sDir + "mask.png"));
	
	
	if (main->platform->SupportsAlphaBorder()) {
	    // Compose the alpha image with the background
	    QImage sourceImage(pix.toImage());
	    QImage destinationImage(sDir + "alpha.png");
	    QImage resultImage(destinationImage.size(), QImage::Format_ARGB32_Premultiplied);
	    
	    QPainter painter(&resultImage);
	    painter.setCompositionMode(QPainter::CompositionMode_Source);
	    painter.fillRect(resultImage.rect(), Qt::transparent);
	    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	    painter.drawImage(0, 0, sourceImage);
	    painter.drawImage(0, 0, destinationImage);
	    
	    painter.end();
	    
	    pix = QPixmap::fromImage(resultImage);
	} 
	
	QPixmap scaled;
	scaled = pix.scaled(skinPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	skinPreview->setPixmap(scaled);
    } else {
	skinPreview->clear();
    }    
}

void OptionsDlg::reject() {
    MyWidget* main = qobject_cast<MyWidget*>(gMainWidget);
    if (main == NULL) return;
    
    if (curPlugin >= 0) {
	QListWidgetItem* item = plugList->item(curPlugin);
	main->plugins.endDialog(item->data(Qt::UserRole).toUInt(), false);
    }
    
    QDialog::reject();
}
