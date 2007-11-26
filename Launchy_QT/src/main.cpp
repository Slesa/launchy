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

#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QWidget>
#include <QPalette>
#include <QLineEdit>
#include <QPixmap>
#include <QBitmap>
#include <QLabel>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDir>
#include <QMenu>
#include <QSettings>
#include <QTimer>
#include <QDateTime>
#include <QDesktopWidget>

#include "icon_delegate.h"
#include "main.h"
#include "globals.h"
#include "options.h"
#include "dsingleapplication.h"
#include "plugin_interface.h"




MyWidget::MyWidget(QWidget *parent)
:  updateTimer(NULL), dropTimer(NULL), alternatives(NULL), QWidget(parent,Qt::Tool | Qt::FramelessWindowHint /*,Qt::Popup*/)
{
//	setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_AlwaysShowToolTips);
	if (platform.isAlreadyRunning())
		exit(1);

	fader = new Fader(this);
	connect(fader, SIGNAL(fadeLevel(double)), this, SLOT(setFadeLevel(double)));
	connect(fader, SIGNAL(finishedFade(double)), this, SLOT(finishedFade(double)));

	gMainWidget = this;
	menuOpen = false;
	gSearchTxt = "";
	gBuilder = NULL;
	catalog = NULL;

	setFocusPolicy(Qt::ClickFocus);

	alwaysShowLaunchy = false;


//	hideLaunchy();
	label = new QLabel(this);

	opsButton = new QPushButton(label);
	opsButton->setObjectName("opsButton");
	opsButton->setToolTip(tr("Launchy Options"));
	connect(opsButton, SIGNAL(pressed()), this, SLOT(menuOptions()));

	closeButton = new QPushButton(label);
	closeButton->setObjectName("closeButton");
	closeButton->setToolTip(tr("Close Launchy"));
	connect(closeButton, SIGNAL(pressed()), this, SLOT(close()));


	output = new QLineEditMenu(label);
	output->setAlignment(Qt::AlignHCenter);
	output->setReadOnly(true);
	output->setObjectName("output");
	connect(output, SIGNAL(menuEvent(QContextMenuEvent*)), this, SLOT(menuEvent(QContextMenuEvent*)));

	input = new QCharLineEdit(label);
	input->setObjectName("input");
	connect(input, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(inputKeyPressEvent(QKeyEvent*)));
	connect(input, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));




	licon = new QLabel(label);




	// Load settings
	if (QFile::exists(qApp->applicationDirPath() + "/config.ini")) 
		gSettings = new QSettings(qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat); 
	else
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");

	// If this is the first time running or a new version, call updateVersion
	bool showLaunchyFirstTime = false;
	if (gSettings->value("version", 0).toInt() != LAUNCHY_VERSION) {
		updateVersion(gSettings->value("version", 0).toInt());
		showLaunchyFirstTime = true;
	}

	alternatives = new QCharListWidget(this);
	listDelegate = new IconDelegate(this);
	defaultDelegate = alternatives->itemDelegate();
	setCondensed(gSettings->value("GenOps/condensedView", false).toBool());
	alternatives->setObjectName("alternatives");
	alternatives->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	alternatives->setTextElideMode(Qt::ElideLeft);
	alternatives->setWindowFlags(Qt::Window | Qt::Tool | Qt::FramelessWindowHint);
	altScroll = alternatives->verticalScrollBar();
	altScroll->setObjectName("altScroll");
//	combo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
	connect(alternatives, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(altKeyPressEvent(QKeyEvent*)));
	connect(alternatives, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));


	// Load the plugins
	plugins.loadPlugins();

	// Load the skin
	applySkin(qApp->applicationDirPath() + "/Skins/" + gSettings->value("GenOps/skin", "Default").toString());

	// Move to saved position
	QPoint x = loadPosition(); //gSettings->value("Display/relpos", QPoint(0,0)).toPoint();
	move(x);
	platform.MoveAlphaBorder(x);

	// Set the general options
	setAlwaysShow(gSettings->value("GenOps/alwaysshow", false).toBool());
	setAlwaysTop(gSettings->value("GenOps/alwaystop", false).toBool());
	setPortable(gSettings->value("GenOps/isportable", false).toBool());
	setUpdateCheck(gSettings->value("GenOps/updatecheck", true).toBool());
	setUpdateTimer(gSettings->value("GenOps/updatetimer", "10").toInt());
	setNumResults(gSettings->value("GenOps/numresults", "10").toInt());


	// Check for udpates?
	if (gSettings->value("GenOps/updatecheck", true).toBool()) {
		checkForUpdate();
	}


	// Set the hotkey
	int curMeta = gSettings->value("GenOps/hotkeyModifier", Qt::AltModifier).toInt();
	int curAction = gSettings->value("GenOps/hotkeyAction", Qt::Key_Space).toInt();
	setHotkey(curMeta, curAction);


	// Set the timers
     updateTimer = new QTimer(this);
	 dropTimer = new QTimer(this);
	 dropTimer->setSingleShot(true);
     connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	 connect(dropTimer, SIGNAL(timeout()), this, SLOT(dropTimeout()));
	 if (gSettings->value("indexTime", 10).toInt() != 0)
		 updateTimer->start(60000);



	// Load the catalog
	gBuilder = new CatBuilder(true, &plugins);
	connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
	gBuilder->start();

	//	setTabOrder(combo, combo);
	
	if (showLaunchyFirstTime)
		showLaunchy();
	else
		hideLaunchy();
//	hideLaunchy();
}

void MyWidget::setCondensed(int condensed) {
	if (alternatives == NULL || listDelegate == NULL || defaultDelegate == NULL)
		return;
	if (condensed) 
		alternatives->setItemDelegate(defaultDelegate);
	else 
		alternatives->setItemDelegate(listDelegate);
		
}
void MyWidget::setHotkey(int meta, int key) {
	QKeySequence keys = QKeySequence(meta + key);
	platform.SetHotkey(keys, this, SLOT(onHotKey()));
}

void MyWidget::altIndexChanged(int index) {

}

void MyWidget::menuEvent(QContextMenuEvent* evt) {
	contextMenuEvent(evt);
}

void MyWidget::showAlternatives(bool show) {
	if (searchResults.size() <= 1)
		return;
	QRect n = altRect;
	n.translate(pos());

	alternatives->setGeometry(n);

	if (show) {
		//alternatives->clear();
		int num = alternatives->count();
		for(int i = 0; i < num; ++i) {
			QListWidgetItem * item = alternatives->takeItem(0);
			if (item != NULL)
				delete item;
		}
		

		for(int i = 0; i < searchResults.size(); ++i) {
			QFileInfo fileInfo(searchResults[i].fullPath);

			QIcon icon = getIcon(searchResults[i]);
			QListWidgetItem * item = new QListWidgetItem(icon, QDir::toNativeSeparators(searchResults[i].fullPath), alternatives);
//			QListWidgetItem *item = new QListWidgetItem(alternatives);
			item->setData(ROLE_FULL, QDir::toNativeSeparators(searchResults[i].fullPath));
			item->setData(ROLE_SHORT, searchResults[i].shortName);
			item->setData(ROLE_ICON, icon);
			item->setToolTip(QDir::toNativeSeparators(searchResults[i].fullPath));
			alternatives->addItem(item);
			alternatives->setFocus();
		}
		if (alternatives->count() > 0) {
			int numViewable = gSettings->value("GenOps/numviewable", "4").toInt();
			//QRect r = alternatives->geometry();
			int min = alternatives->count() < numViewable ? alternatives->count() : numViewable;
			n.setHeight(min * alternatives->sizeHintForRow(0));

			altRect.setHeight(n.height());

			// Is there room for the dropdown box?
			if (n.y() + n.height() > qApp->desktop()->height()) {
				n.moveTop(pos().y() + input->pos().y() - n.height());
			}
			alternatives->setGeometry(n);
		}
		double opaqueness = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
		opaqueness /= 100.0;
		alternatives->setWindowOpacity(opaqueness);
		alternatives->show();

	}
}


void MyWidget::launchObject(int obj) {
	CatItem res = inputData[0].getTopResult();
	if (res.id == HASH_LAUNCHY) {
		QString args = "";
		if (inputData.count() > 1)
			for(int i = 1; i < inputData.count(); ++i)
				args += inputData[i].getText() + " ";
		runProgram(res.fullPath, args);
	}
	else {
		int ops = plugins.execute(&inputData, &res);
		if (ops > 1) {
			switch (ops)
			{
				case MSG_CONTROL_EXIT:
					close();
					break;
				case MSG_CONTROL_OPTIONS:
					menuOpen = true;
					menuOptions();
					menuOpen = false;
					break;
				case MSG_CONTROL_REBUILD:
					// Perform the database update
					if (gBuilder == NULL) {
						gBuilder = new CatBuilder(false, &plugins);
						gBuilder->setPreviousCatalog(catalog);
						connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
						gBuilder->start(QThread::IdlePriority);
					}
					break;
				default:
					break;
			}
		}
	}
	catalog->incrementUsage(res);
}

void MyWidget::focusOutEvent ( QFocusEvent * evt) {
	if (evt->reason() == Qt::ActiveWindowFocusReason) {
		if (gSettings->value("GenOps/hideiflostfocus", true).toBool())
			if (!this->isActiveWindow() && !alternatives->isActiveWindow() && !menuOpen)
				hideLaunchy();
	}
}


void MyWidget::altKeyPressEvent(QKeyEvent* key) {
	if (key->key() == Qt::Key_Escape) {
		alternatives->hide();
	}
	if (key->key() == Qt::Key_Up) {key->ignore();}
	else if (key->key() == Qt::Key_Down) {key->ignore();}
	else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Tab) {
		
		if (searchResults.count() > 0) {
			int row = alternatives->currentRow();
			if (row > -1) {
				QString location = "History/" + input->text();
				QStringList hist;
				hist << searchResults[row].lowName << 
					searchResults[row].fullPath;
				gSettings->setValue(location, hist);

				CatItem tmp = searchResults[row];
				searchResults[row] = searchResults[0];
				searchResults[0] = tmp;

				updateDisplay();

				inputData.last().setText(searchResults[0].fullPath);
				input->setText(printInput() + searchResults[0].fullPath);

				alternatives->hide();


				if (key->key() == Qt::Key_Tab) {
					doTab();
					parseInput(input->text());
					searchOnInput();
					updateDisplay();
					dropTimer->stop();
					dropTimer->start(1000);
				} else {

					doEnter();
				}
			}
		}
	}
	else {
		alternatives->hide();
		this->activateWindow();
		input->setFocus();
		key->ignore();
	}
}





void MyWidget::inputKeyPressEvent(QKeyEvent* key) {
	if (key->key() == Qt::Key_Tab) {
		keyPressEvent(key);
	} 	
	else {
		key->ignore();
	}
}

void MyWidget::parseInput(QString text) {
//	QStringList spl = text.split(" | ");
	QStringList spl = text.split(QString(" ") + sepChar() + QString(" "));
	if (spl.count() < inputData.count()) {
		inputData = inputData.mid(0, spl.count());
	}
	for(int i = 0; i < inputData.size(); i++) {
		if (inputData[i].getText() != spl[i]) {
			inputData = inputData.mid(0, i);
			break;
		}
	}
	for(int i = inputData.count(); i < spl.count(); i++) {
		InputData data(spl[i]);
		inputData.push_back(data);
	}
}

// Print all of the input up to the last entry
QString MyWidget::printInput() {
	QString res = "";
	for(int i = 0; i < inputData.count()-1; ++i) {
		res += inputData[i].getText();
		if (i+2 >= inputData.count())
			res += QString(" ") + sepChar() + QString(" ");
	}
	return res;
}

void MyWidget::doTab() 
{
	if (inputData.count() > 0 && searchResults.count() > 0) {
		// If it's an incomplete file or dir, complete it
		QFileInfo info(searchResults[0].fullPath);

		if ((inputData.last().hasLabel(LABEL_FILE) || info.isDir())
		)//	&& input->text().compare(QDir::toNativeSeparators(searchResults[0].fullPath), Qt::CaseInsensitive) != 0)
		{
			QString path;
			if (info.isSymLink())
				path = info.symLinkTarget() + QDir::toNativeSeparators("/");
			else
				path = searchResults[0].fullPath;
			input->setText(printInput() + QDir::toNativeSeparators(path));
		} else {
			// Looking for a plugin
			input->setText(input->text() + " " + sepChar() + " ");
			inputData.last().setText(searchResults[0].shortName);
			input->setText(printInput() + searchResults[0].shortName + " " + sepChar() + " ");
		}
	}
}

void MyWidget::doEnter()
{
	if (dropTimer->isActive())
		dropTimer->stop();
	if (searchResults.count() > 0 || inputData.count() > 1) 
		launchObject(0);
	hideLaunchy();

}

void MyWidget::keyPressEvent(QKeyEvent* key) {
	
	if (key->key() == Qt::Key_Escape) {
		if (alternatives->isVisible())
			alternatives->hide();
		else
			hideLaunchy();
	}

	else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter) {
		doEnter();			
	}

	else if (key->key() == Qt::Key_Down) {
		if (!alternatives->isVisible()) {
			dropTimer->stop();
			showAlternatives();
		}
		if (alternatives->isVisible() && this->isActiveWindow()) {
			alternatives->setFocus();
			if (alternatives->count() > 0) {
				alternatives->setCurrentRow(0);
			}

			alternatives->activateWindow();

		}
	}

	else if (key->key() == Qt::Key_Up) {
		// Prevent alternatives from being hidden on up key
	}


	else {
		if (key->key() == Qt::Key_Tab) {
			doTab();
		} else {
			key->ignore();	
		}

		alternatives->hide();
		dropTimer->stop();
		dropTimer->start(1000);

		parseInput(input->text());

		searchOnInput();
		updateDisplay();
	}	
}

void MyWidget::searchOnInput() {
	if (catalog == NULL) return;
	if (inputData.count() == 0) return;
	
	gSearchTxt = inputData.last().getText();
	searchResults.clear();

	if (catalog != NULL) {
		if (inputData.count() <= 1)
			catalog->searchCatalogs(gSearchTxt, searchResults);
	}

	if (searchResults.count() != 0)
		inputData.last().setTopResult(searchResults[0]);

	plugins.getLabels(&inputData);
	plugins.getResults(&inputData, &searchResults);
	qSort(searchResults.begin(), searchResults.end(), CatLessNoPtr);

	// Is it a file?
	if (gSearchTxt.contains("\\") || gSearchTxt.contains("/")) {
		searchFiles(gSearchTxt, searchResults);
		inputData.last().setLabel(LABEL_FILE);
	}
	catalog->checkHistory(gSearchTxt, searchResults);
}

void MyWidget::updateDisplay() {
	if (searchResults.count() > 0) {
		QIcon icon = getIcon(searchResults[0]);
		licon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
		output->setText(searchResults[0].shortName);

		// Did the plugin take control of the input?
		if (inputData.last().getID() != 0)
			searchResults[0].id = inputData.last().getID();

		inputData.last().setTopResult(searchResults[0]);

	} else {
		licon->clear();
		output->clear();
	}	
}

QIcon MyWidget::getIcon(CatItem & item) {

	if (item.icon.isNull()) {
		QDir dir(item.fullPath);
		if (dir.exists())
			return platform.icons->icon(QFileIconProvider::Folder);
		return platform.icons->icon(QDir::toNativeSeparators(item.fullPath));
	}
	else
		return platform.icons->icon(QDir::toNativeSeparators(item.icon));
}

void MyWidget::searchFiles(const QString & input, QList<CatItem>& searchResults) {
	// Split the string on the last slash
	QString path = QDir::fromNativeSeparators(gSearchTxt);

	// Network searches are too slow
	if (path.startsWith("//")) return;

	QString dir, file;
	dir = path.mid(0,path.lastIndexOf("/"));
	file = path.mid(path.lastIndexOf("/")+1);
	QFileInfo info(dir);
	if (!info.isDir()) return;

	if (file == "") {
		CatItem item(QDir::toNativeSeparators(dir + "/"));
		searchResults.push_back(item);
	}	


	// Okay, we have a directory, find files that match "file"
	QDir qd(dir);
	QStringList ilist;
	if (gSettings->value("GenOps/showHiddenFiles", false).toBool())
		ilist = qd.entryList(QStringList(file + "*"), QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);
	else
		ilist = qd.entryList(QStringList(file + "*"), QDir::AllDirs | QDir::Files , QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);
	foreach(QString inf, ilist) {
		if (inf.startsWith(".")) continue;
		if (inf.mid(0, file.count()).compare(file,  Qt::CaseInsensitive) == 0) {
			QString fp = qd.absolutePath() + "/" + inf;
			QFileInfo in(fp);
			if (in.isDir())
				fp += "/";
			CatItem item(QDir::toNativeSeparators(fp), inf);
			searchResults.push_back(item);
		}
	}
	return;
}


void MyWidget::catalogBuilt() {
	if (catalog != NULL) {
		delete catalog;
	}
	catalog = gBuilder->getCatalog();
	gBuilder->wait();
	delete gBuilder;
	gBuilder = NULL;
//	qDebug() << "The catalog is built, need to re-search input text" << catalog->count();
	//todo Do a search here of the current input text
	searchOnInput();
	updateDisplay();
}

void MyWidget::checkForUpdate() {
    http = new QHttp(this);
    verBuffer = new QBuffer(this);
	counterBuffer = new QBuffer(this);
	verBuffer->open(QIODevice::ReadWrite);
	counterBuffer->open(QIODevice::ReadWrite);


    connect(http, SIGNAL(done( bool)), this, SLOT(httpGetFinished( bool)));
	http->setHost("www.launchy.net");	
	http->get("http://www.launchy.net/version.html", verBuffer);	

	/*
	QHttpRequestHeader header("GET", "/n?id=AEJV3A4l/cDSX3qBPvhGeIRGerIg");
	header.setValue("Host", "m1.webstats.motigo.com");
	header.setValue("Referer", "http://www.launchy.net/stats.html");
	header.setContentType("image/gif, text/plain, text/html, text/htm");
	http->setHost("m1.webstats.motigo.com");
	http->request(header, NULL, counterBuffer);
	*/
}

void MyWidget::httpGetFinished(bool error) {

	if (!error) {
		QString str(verBuffer->data());
		int ver = str.toInt();
		if (ver > LAUNCHY_VERSION) {
			QMessageBox::information(this, tr("A new version of Launchy is available"), 
				tr("A new version of Launchy is available.\n\nYou can download it at http://www.launchy.net/"));
		}
		if (http != NULL)
			delete http;
		http = NULL;
	}
	verBuffer->close();
	counterBuffer->close();
	delete verBuffer;
	delete counterBuffer;
}

void MyWidget::setSkin(QString name) {
	bool wasShowing = isVisible();
	QPoint p = pos();
	hideLaunchy(true);
	applySkin(qApp->applicationDirPath() + "/Skins/" + name);
	move(p);
	platform.MoveAlphaBorder(p);
	platform.ShowAlphaBorder();
	if (wasShowing)
		showLaunchy(true);
}

void MyWidget::updateVersion(int oldVersion) {
	if (oldVersion < 200) {
		// We've completely changed the database and ini between 1.25 and 2.0
		// Erase all of the old information
		QString origFile = gSettings->fileName();
		delete gSettings;

		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");
		QString permFile = gSettings->fileName();
		delete gSettings;

		QFile oldIniPerm(permFile);
		oldIniPerm.remove();
		oldIniPerm.close();

		QDir d(permFile);
		d.cdUp();
		QFile oldDbPerm(d.absoluteFilePath("Launchy.db"));
		oldDbPerm.remove();
		oldDbPerm.close();

		QFile oldDB(qApp->applicationDirPath() + "/Launchy.db");
		oldDB.remove();
		oldDB.close();

		QFile oldIni(qApp->applicationDirPath() + "/Launchy.ini");
		oldIni.remove();
		oldIni.close();

		gSettings = new QSettings(origFile, QSettings::IniFormat);
	}

	if (oldVersion < LAUNCHY_VERSION) {
		gSettings->setValue("donateTime", QDateTime::currentDateTime().addDays(21));
		gSettings->setValue("version", LAUNCHY_VERSION);
	}
}

/*
QPair<double,double> MyWidget::relativePos() {
	QPoint p = pos();
	QPair<double,double> relPos;
	relPos.first = (double) p.x() / (double) qApp->desktop()->width();
	relPos.second = (double) p.y() / (double) qApp->desktop()->height();
	return relPos;
}

QPoint MyWidget::absolutePos(QPair<double,double> relPos) {
	QPoint absPos;
	absPos.setX(relPos.first * (double) qApp->desktop()->width());
	absPos.setY(relPos.second * (double) qApp->desktop()->height());
	return absPos;
}

QPoint MyWidget::loadPosition() {
	QPair<double,double> rpos;
	rpos.first = gSettings->value("Display/rposX", 0.0).toDouble();
	rpos.second = gSettings->value("Display/rposY", 0.0).toDouble();
	return absolutePos(rpos);
}

void MyWidget::savePosition() {
	QPair<double,double> rpos = relativePos();
	gSettings->setValue("Display/rposX", rpos.first);
	gSettings->setValue("Display/rposY", rpos.second);
}
*/


void MyWidget::updateTimeout() {
	// Save the settings periodically
	savePosition();
	gSettings->sync();

	// Perform the database update
	if (gBuilder == NULL) {
		gBuilder = new CatBuilder(false, &plugins);
		gBuilder->setPreviousCatalog(catalog);
		connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
		gBuilder->start(QThread::IdlePriority);
	}

	int time = gSettings->value("indexTime", 10).toInt();
	updateTimer->stop();
	if (time != 0)
		updateTimer->start(time * 60000);
}

void MyWidget::dropTimeout() {
	if (input->text() != "")
		showAlternatives();
}

void MyWidget::onHotKey() {
	if (menuOpen) {
		showLaunchy();
		return;
	} 
	if (isVisible()) {
		hideLaunchy();
	}
	else {
		showLaunchy();
	}
}

void MyWidget::closeEvent(QCloseEvent *event) {
//	gSettings->setValue("Display/pos", relativePosition());
	savePosition();
	gSettings->sync();


	QDir dest(gSettings->fileName());
	dest.cdUp();
	CatBuilder builder(catalog, &plugins);
	builder.storeCatalog(dest.absoluteFilePath("Launchy.db"));
	event->accept();
	qApp->quit();
}

MyWidget::~MyWidget() {
	delete updateTimer;
	delete dropTimer;
}

void MyWidget::MoveFromAlpha(QPoint pos) {
	move(pos);
	if(alternatives)
		alternatives->hide();
}

void MyWidget::setAlwaysShow(bool alwaysShow) {
	alwaysShowLaunchy = alwaysShow;
	if (!isVisible()  && alwaysShow)
		showLaunchy();
}

void MyWidget::setAlwaysTop(bool alwaysTop) {
	if (alwaysTop)	{
//		setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);
	}
	else {
		if ((windowFlags() & Qt::WindowStaysOnTopHint) != 0)
			setWindowFlags( windowFlags() & ~Qt::WindowStaysOnTopHint);
	}
		
}
void MyWidget::setPortable(bool portable) {
	if (portable && gSettings->fileName().compare(qApp->applicationDirPath() + "/config.ini", Qt::CaseInsensitive) != 0) {
		QString oldName = gSettings->fileName();
		delete gSettings;

		// Copy the old settings
		QFile oldSet(oldName);
		oldSet.copy(qApp->applicationDirPath() + "/config.ini");
		oldSet.close();

		QFile oldDB(oldName.mid(0,oldName.lastIndexOf("/")) + "/Launchy.db");
		oldDB.copy(qApp->applicationDirPath() + "/Launchy.db");
		oldDB.close();

		gSettings = new QSettings(qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat); 

	}
	else if (!portable && gSettings->fileName().compare(qApp->applicationDirPath() + "/config.ini", Qt::CaseInsensitive) == 0) {
		delete gSettings;
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");
		QString newName = gSettings->fileName();
		delete gSettings;

		// Remove the ini file we're going to copy to so that copy can work
		QFile newF(newName);
		newF.remove();
		newF.close();

		// Copy the local ini + db files to the users section
		QFile oldSet(qApp->applicationDirPath() + "/config.ini");
		oldSet.copy(newName);
		oldSet.remove();
		oldSet.close();

		QFile oldDB(qApp->applicationDirPath() + "/Launchy.db");
		oldDB.copy(newName.mid(0,newName.lastIndexOf("/")) + "/Launchy.db");
		oldDB.remove();
		oldDB.close();

		// Load up the user section ini file
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");

	}
}

void MyWidget::setUpdateCheck(bool) {

}
void MyWidget::setUpdateTimer(int) {

}
void MyWidget::setNumResults(int) {

}

void MyWidget::setFadeTimes(int in, int out)
{
	
}

void MyWidget::setNumViewable(int val)
{
}

void MyWidget::setOpaqueness(int val)
{
	double max = (double) val;
	max /= 100.0;
	this->setWindowOpacity(max);
	platform.SetAlphaOpacity(max);
}

void MyWidget::applySkin(QString directory) {

	// Hide the buttons by default
	closeButton->hide();
	opsButton->hide();

	if (listDelegate == NULL) return;

	// Use default skin if this one doesn't exist
	if (!QFile::exists(directory + "/misc.txt")) 
		directory = qApp->applicationDirPath() + "/Skins/Default/";
	

	// Set positions
	if (QFile::exists(directory + "/misc.txt")) {
		QFile file(directory + "/misc.txt");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
			QTextStream in(&file);
			while (!in.atEnd()) {
				QString line = in.readLine();
				if (line.startsWith(";")) continue;
				QStringList spl = line.split("=");
				if (spl.size() == 2) {
					QStringList sizes = spl.at(1).trimmed().split(",");
					QRect rect;
					if (sizes.size() == 4) {
						rect.setRect(sizes[0].toInt(), sizes[1].toInt(), sizes[2].toInt(), sizes[3].toInt());
					} 

					if (spl.at(0).trimmed().compare("input", Qt::CaseInsensitive) == 0) 
						input->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("output", Qt::CaseInsensitive) == 0) 
						output->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("alternatives", Qt::CaseInsensitive) == 0)
						altRect = rect;
					else if (spl.at(0).trimmed().compare("boundary", Qt::CaseInsensitive) == 0) {
						setGeometry(rect);
						label->setGeometry(rect);	
					}
					else if (spl.at(0).trimmed().compare("icon", Qt::CaseInsensitive) == 0) 
						licon->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("optionsbutton", Qt::CaseInsensitive) == 0) {
						opsButton->setGeometry(rect);
						opsButton->show();
					}
					else if (spl.at(0).trimmed().compare("closebutton", Qt::CaseInsensitive) == 0) {
						closeButton->setGeometry(rect);
						closeButton->show();
					}
					else if (spl.at(0).trimmed().compare("dropPathColor", Qt::CaseInsensitive) == 0)
						listDelegate->setColor(spl.at(1));
					else if (spl.at(0).trimmed().compare("dropPathSelColor", Qt::CaseInsensitive) == 0)
						listDelegate->setColor(spl.at(1),true);
					else if (spl.at(0).trimmed().compare("dropPathFamily", Qt::CaseInsensitive) == 0)
						listDelegate->setFamily(spl.at(1));
					else if (spl.at(0).trimmed().compare("dropPathSize", Qt::CaseInsensitive) == 0)
						listDelegate->setSize(spl.at(1).toInt());
					else if (spl.at(0).trimmed().compare("dropPathWeight", Qt::CaseInsensitive) == 0)
						listDelegate->setWeight(spl.at(1).toInt());
					else if (spl.at(0).trimmed().compare("dropPathItalics", Qt::CaseInsensitive) == 0)
						listDelegate->setItalics(spl.at(1).toInt());
				}
				
			}			
			file.close();
		}
	}

	// Load the style sheet
	if (QFile::exists(directory + "/style.qss")) {		
		QFile file(directory + "/style.qss");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QString styleSheet = QLatin1String(file.readAll());
			qApp->setStyleSheet(styleSheet);
			file.close();
		}
	}
	
	// Set the background image
	if (QFile::exists(directory + "/background.png")) {
		QPixmap image(directory + "/background.png");
		label->setPixmap(image);
	}

	// Set the background mask
	if (QFile::exists(directory + "/mask.png")) {
		QPixmap image(directory + "/mask.png");
		setMask(image);
	}

	// Set the alpha background
	if (QFile::exists(directory + "/alpha.png")) {
		platform.CreateAlphaBorder(this, directory + "/alpha.png");
		platform.MoveAlphaBorder(pos());
	}
	
}



void MyWidget::mousePressEvent(QMouseEvent *e)
{
	moveStartPoint = e->pos();
}

void MyWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPoint p = e->globalPos();
	p -= moveStartPoint;
	move(p);
	platform.MoveAlphaBorder(p);
	showAlternatives(false);
	input->setFocus();

}



void MyWidget::contextMenuEvent(QContextMenuEvent *event) {
	QMenu menu(this);
	QAction* actOptions = menu.addAction(tr("Options"));
	connect(actOptions, SIGNAL(triggered()), this, SLOT(menuOptions()));
	QAction* actExit = menu.addAction(tr("Exit"));
	connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
	menuOpen = true;
	menu.exec(event->globalPos());
	menuOpen = false;
}


void MyWidget::menuOptions() {
	
	alternatives->hide();
	OptionsDlg ops(this);
	ops.setObjectName("options");
	ops.exec();

	// Perform the database update
	if (gBuilder == NULL) {
		gBuilder = new CatBuilder(false, &plugins);
		gBuilder->setPreviousCatalog(catalog);
		connect(gBuilder, SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
		gBuilder->start(QThread::IdlePriority);
	}
	input->activateWindow();
	input->setFocus();	
}

void MyWidget::shouldDonate() {
	QDateTime time = QDateTime::currentDateTime();
	QDateTime donateTime = gSettings->value("donateTime",time.addDays(21)).toDateTime();
	if (donateTime.isNull()) return;
	gSettings->setValue("donateTime", donateTime);

	if (donateTime <= time) {
		runProgram("http://www.launchy.net/donate.html", "");
		QDateTime def;
		gSettings->setValue("donateTime", def);
	}
}

void Fader::fadeIn() {

	int time = gSettings->value("GenOps/fadein", 0).toInt();
	double end = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
	end /= 100.0;
	if (time != 0) {
		double delay = ((double) time) / (end / 0.05);

		for(double i = 0.0; i < end + 0.01 && keepRunning; i += 0.05) {
			emit fadeLevel(i);
			msleep(delay);
		}
	}
	emit fadeLevel(end);
	emit finishedFade(end);
	return;
}

void Fader::fadeOut() {
	int time = gSettings->value("GenOps/fadeout", 0).toInt();

	if(time != 0) {
		double start = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
		start /= 100.0;
		double delay = ((double) time) / (start / 0.05);


		for(double i = start; i > -0.01 && keepRunning; i -= 0.05) {
			emit fadeLevel(i);
			msleep(delay);
		}
	}
	emit fadeLevel(0.0);
	emit finishedFade(0.0);

	return;
}

void Fader::run() {
	keepRunning = true;
	if (fadeType)
		fadeIn();
	else
		fadeOut();
}

void MyWidget::setFadeLevel(double d) {
	this->setWindowOpacity(d);
	platform.SetAlphaOpacity(d);
}

void MyWidget::finishedFade(double d) {
	if (d == 0.0) {
		hide();
		platform.HideAlphaBorder();
	}
}

void MyWidget::fadeIn() {
	if (fader->isRunning())
		fader->stop();
	while(fader->isRunning()) {}
	fader->setFadeType(true);
	fader->start();
}

void MyWidget::fadeOut() {
	if (fader->isRunning())
		fader->stop();
	while(fader->isRunning()) {}
	fader->setFadeType(false);
	fader->start();
}



void MyWidget::showLaunchy(bool now) {
	shouldDonate();
	alternatives->hide();
	
	// This gets around the weird Vista bug
	// where the alpha border would dissappear
	// on sleep or user switch
	move(loadPosition());
	platform.CreateAlphaBorder(this, "");
	platform.MoveAlphaBorder(pos());

	setFadeLevel(0.0);
	this->show();
	platform.ShowAlphaBorder();

	if (!now) {
		fadeIn();
	} else {
		double end = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
		end /= 100.0;
		setFadeLevel(end);
	}

	input->activateWindow();
	input->selectAll();
	input->setFocus();

	// Let the plugins know
	plugins.showLaunchy();
}



void MyWidget::hideLaunchy(bool now) {
	if (!isVisible())
		return;
	savePosition();
	if (dropTimer != NULL && dropTimer->isActive())
		dropTimer->stop();
	if (alwaysShowLaunchy) return;
	
	if (alternatives != NULL)
		alternatives->hide();

	if (isVisible()) {
		if (!now)
			fadeOut();
		else {
			setFadeLevel(0.0);
			finishedFade(0.0);
		}
	}

	// let the plugins know
	plugins.hideLaunchy();
}

void MyWidget::printGeometry() {
	QPoint p = pos();
	qDebug() << p;
	QRect r = geometry();
	qDebug() << r;


}

QChar MyWidget::sepChar() {
	QFontMetrics met = input->fontMetrics();
	QChar arrow(0x25ba);
	if (met.inFont(arrow))
		return arrow;
	else
		return QChar('|');
}
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList args = qApp->arguments();

	QCoreApplication::setApplicationName("Launchy");
	QCoreApplication::setOrganizationDomain("Launchy");



	MyWidget widget;
	widget.setObjectName("main");

	return app.exec();
} 