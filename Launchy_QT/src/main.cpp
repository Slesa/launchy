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

#include "main.h"
#include "globals.h"
#include "options.h"
#include "dsingleapplication.h"




MyWidget::MyWidget(QWidget *parent)
:  updateTimer(NULL), dropTimer(NULL), alternatives(NULL), QWidget(parent,Qt::Tool | Qt::FramelessWindowHint /*,Qt::Popup*/)
{
	gMainWidget = this;
	menuOpen = false;
	gSearchTxt = "";
	gBuilder = NULL;
	catalog = NULL;

	setFocusPolicy(Qt::ClickFocus);

	alwaysShowLaunchy = false;


	hideLaunchy();
	label = new QLabel(this);


	output = new QLineEdit(label);
	output->setAlignment(Qt::AlignHCenter);
	output->setReadOnly(true);
	output->setObjectName("output");

	input = new QCharLineEdit(label);
	input->setObjectName("input");
	connect(input, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(inputKeyPressEvent(QKeyEvent*)));
	connect(input, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));

	alternatives = new QCharListWidget(this);
	alternatives->setObjectName("alternatives");
	alternatives->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	alternatives->setTextElideMode(Qt::ElideLeft);
//	alternatives->setViewMode(QListView::IconMode);
	alternatives->setWindowFlags(Qt::Window | Qt::Tool | Qt::FramelessWindowHint);
	altScroll = alternatives->verticalScrollBar();
	altScroll->setObjectName("altScroll");

//	combo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
	connect(alternatives, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(altKeyPressEvent(QKeyEvent*)));
	connect(alternatives, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));
	licon = new QLabel(label);
//	QIcon icon = platform.GetIcon("vc70.idb");	
//	licon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));




	// Load settings
	if (QFile::exists(qApp->applicationDirPath() + "/config.ini")) 
		gSettings = new QSettings(qApp->applicationDirPath() + "/config.ini", QSettings::IniFormat); 
	else
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy.net", "Launchy");

	// Load the plugins
	plugins.loadPlugins();

	// Load the skin
	applySkin(qApp->applicationDirPath() + "/Skins/" + gSettings->value("GenOps/skin", "Default").toString());

	// Move to saved position
	QPoint x = gSettings->value("Display/pos", QPoint(0,0)).toPoint();
	move(x);
	platform.MoveAlphaBorder(x);

	// If this is the first time running or a new version, call updateVersion
	if (gSettings->value("version", 0).toInt() != LAUNCHY_VERSION)
	updateVersion(gSettings->value("version", 0).toInt());

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
	int curMeta = gSettings->value("GenOps/hotkeyModifier", Qt::ControlModifier).toInt();
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
	

	showLaunchy();
}


void MyWidget::setHotkey(int meta, int key) {
	QKeySequence keys = QKeySequence(meta + key);
	platform.SetHotkey(keys, this, SLOT(onHotKey()));
}

void MyWidget::altIndexChanged(int index) {

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
			QListWidgetItem *item=new QListWidgetItem(icon, QDir::toNativeSeparators(searchResults[i].fullPath),alternatives,0);
			qDebug () << searchResults[i].lowName;
			alternatives->addItem(item);
			alternatives->setFocus();
		}

			alternatives->show();

	}
}


void MyWidget::launchObject(int obj) {
	if (searchResults[obj].id == HASH_LAUNCHY)
		platform.Execute(searchResults[obj].fullPath, "");
	else
		plugins.execute(&inputData, &searchResults[obj]);
	catalog->incrementUsage(searchResults[obj]);
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
	/*
	else if (key->key() == Qt::Key_Backslash || key->key() == Qt::Key_Slash) {
		int row = alternatives->currentRow();
		CatItem tmp = searchResults[row];
		searchResults.clear();
		searchResults.push_back(tmp);
		QIcon icon = getIcon(searchResults[0]);

		licon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
		output->setText(searchResults[0].shortName);
		input->setText(QDir::toNativeSeparators(searchResults[0].fullPath + "/"));
		alternatives->hide();
		this->activateWindow();
		input->setFocus();
	}
	*/
	else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Tab) {
		
		if (searchResults.count() > 0) {
			int row = alternatives->currentRow();
			if (row > 0) {
				QString location = "History/" + input->text();
				QStringList hist;
				hist << searchResults[row].lowName << 
					searchResults[row].fullPath;
				gSettings->setValue(location, hist);

				CatItem tmp = searchResults[row];
				searchResults.clear();
				searchResults.push_back(tmp);
				QIcon icon = getIcon(searchResults[0]);

				input->clear();
				input->insert(QDir::toNativeSeparators(tmp.fullPath));
				licon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
				output->setText(searchResults[0].shortName);
				alternatives->hide();
			}

			//	hideLaunchy();
		//	launchObject(row);
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
	} else {
		key->ignore();
	}
}

void MyWidget::parseInput(QString text) {
	QStringList spl = text.split(" | ");
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

void MyWidget::keyPressEvent(QKeyEvent* key) {
	
	if (key->key() == Qt::Key_Escape) {
			hideLaunchy();
	}

	else if (key->key() == Qt::Key_Return) {
		if (dropTimer->isActive())
			dropTimer->stop();
		hideLaunchy();
		if (searchResults.count() > 0) {
			if (alternatives->isVisible()) {
				launchObject(alternatives->currentRow());
			} else {
				launchObject(0);
			}
		}
		
	}

	else if (key->key() == Qt::Key_Down) {
		if (!alternatives->isVisible()) {
			dropTimer->stop();
			showAlternatives();
		}
		if (alternatives->isVisible() && this->isActiveWindow()) {
			alternatives->activateWindow();

			alternatives->setFocus();
			if (alternatives->count() > 0) {
				alternatives->setCurrentRow(0);
			}
		}

	}
	/*
	else if (key->key() == Qt::Key_Left ||
		key->key() == Qt::Key_Right) {
		key->ignore();
	}
	*/
	else if (key->key() == Qt::Key_Up) {
		// Prevent alternatives from being hidden on up key
	}

	else {
		if (key->key() == Qt::Key_Tab || key->key() == Qt::Key_Backslash || key->key() == Qt::Key_Slash) {
			if (searchResults.count() > 0) {
				QString path = "";
				QFileInfo info(searchResults[0].fullPath);
//				if (!info.exists()) return;
				if (info.isSymLink()) {
					path = info.symLinkTarget();
					path += "/";
					path = QDir::toNativeSeparators(path);
				} else if (info.isDir()) {
					path = searchResults[0].fullPath;
					if (!path.endsWith("/") && !path.endsWith("\\"))
						path += "/";
					path = QDir::toNativeSeparators(path);
				} else if (info.isFile()) {
					path = searchResults[0].fullPath;
				} else {
					path = input->text();
					if (key->key() == Qt::Key_Tab) {
						path += " | ";	
					}
				}
				input->setText(path);
			} 
		} else {
			key->ignore();	
		}

		alternatives->hide();
		dropTimer->stop();
		dropTimer->start(1000);

		QString inText = input->text();
		parseInput(inText);

		if (input->text() != "") {
			if (inputData.count() == 0) return;
			gSearchTxt = inputData.last().getText();
			searchResults.clear();
			if (catalog != NULL) {
				catalog->searchCatalogs(gSearchTxt, searchResults);
			}
			if (searchResults.count() == 0) {
				// Is it a file?
				if (gSearchTxt.contains("\\") || gSearchTxt.contains("/")) {
					searchFiles(gSearchTxt, searchResults);
					inputData.last().setLabel(LABEL_FILE);
				}
			}

			plugins.getLabels(&inputData);
			plugins.getResults(&inputData, &searchResults);
//			sortResults(searchResults);


			input->clear();
			input->insert(inText);

			if (searchResults.count() > 0) {
//				qDebug() << searchResults[0].shortName;

				QIcon icon = getIcon(searchResults[0]);

				licon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
				output->setText(searchResults[0].shortName);
				inputData.last().setTopResult(searchResults[0]);

			} else {
				licon->clear();
				output->clear();
			}
		}
		
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
	QString dir, file;
	dir = path.mid(0,path.lastIndexOf("/"));
	file = path.mid(path.lastIndexOf("/")+1);
	QFileInfo info(dir);
	if (!info.isDir()) return;

	if (file == "") {
		CatItem item(dir + "/");
		searchResults.push_back(item);
	}	

	// Okay, we have a directory, find files that match "file"
	QDir qd(dir);
	qDebug() << dir;
	qDebug() << file;
	QStringList ilist = qd.entryList(QStringList(file + "*"), QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	foreach(QString inf, ilist) {
		if (inf.mid(0, file.count()).compare(file,  Qt::CaseInsensitive) == 0) {
			CatItem item(qd.absolutePath() + "/" +  inf);
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
	qDebug() << "The catalog is built, need to re-search input text" << catalog->count();
	//todo Do a search here of the current input text
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

	QHttpRequestHeader header("GET", "/n?id=AEJV3A4l/cDSX3qBPvhGeIRGerIg");
	header.setValue("Host", "m1.webstats.motigo.com");
	header.setValue("Referer", "http://www.launchy.net/stats.html");
	header.setContentType("image/gif, text/plain, text/html, text/htm");
	http->setHost("m1.webstats.motigo.com");
	http->request(header, NULL, counterBuffer);
}

void MyWidget::httpGetFinished(bool error) {
	if (!error) {
		QString str(verBuffer->data());
		int ver = str.toInt();
		if (ver > LAUNCHY_VERSION) {
			QMessageBox::information(this, tr("A new version of Launchy is available"), 
				tr("A new version of Launchy is available.\n\nYou can download it at http://www.launchy.net/"));
		}
	}
	verBuffer->close();
	counterBuffer->close();
	delete http;
	delete verBuffer;
	delete counterBuffer;
}

void MyWidget::setSkin(QString name) {
	applySkin(qApp->applicationDirPath() + "/Skins/" + name);
}

void MyWidget::updateVersion(int oldVersion) {
	if (oldVersion < LAUNCHY_VERSION) {
		gSettings->setValue("donateTime", QDateTime::currentDateTime().addDays(21));
		gSettings->setValue("version", LAUNCHY_VERSION);
	}
	if (oldVersion < 200) {
		QFile oldDB(qApp->applicationDirPath() + "/Launchy.db");
		oldDB.remove();
		oldDB.close();
	}
}

void MyWidget::updateTimeout() {
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
	if (visible)
		hideLaunchy();
	else {
		showLaunchy();
	}
}

void MyWidget::closeEvent(QCloseEvent *event) {
	gSettings->setValue("Display/pos", pos());

	qApp->quit();
	event->accept();
}

MyWidget::~MyWidget() {
	QString dest = gSettings->fileName();
	int lastSlash = dest.lastIndexOf(QLatin1Char('/'));
	if (lastSlash == -1)
		return;
	dest = dest.mid(0, lastSlash);
	dest += "/Launchy.db";

	CatBuilder builder(catalog, &plugins);
	builder.storeCatalog(dest);
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
	if (!visible  && alwaysShow)
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
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy.net", "Launchy");
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
		gSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Launchy.net", "Launchy");

	}
}
void MyWidget::setUpdateCheck(bool) {

}
void MyWidget::setUpdateTimer(int) {

}
void MyWidget::setNumResults(int) {

}



void MyWidget::applySkin(QString directory) {

	// Set positions
	if (QFile::exists(directory + "/pos.txt")) {
		QFile file(directory + "/pos.txt");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {	
			QTextStream in(&file);
			while (!in.atEnd()) {
				QString line = in.readLine();
				QStringList spl = line.split("=");
				if (spl.size() == 2) {
					QStringList sizes = spl.at(1).trimmed().split(",");
					QRect rect;
					if (sizes.size() == 4) {
						rect.setRect(sizes[0].toInt(), sizes[1].toInt(), sizes[2].toInt(), sizes[3].toInt());
					} else continue;

					if (spl.at(0).trimmed().compare("input", Qt::CaseInsensitive) == 0) 
						input->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("output", Qt::CaseInsensitive) == 0) 
						output->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("alternatives", Qt::CaseInsensitive) == 0)
						altRect = rect;
					else if (spl.at(0).trimmed().compare("boundary", Qt::CaseInsensitive) == 0) 
						label->setGeometry(rect);	
					else if (spl.at(0).trimmed().compare("icon", Qt::CaseInsensitive) == 0) 
						licon->setGeometry(rect);
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
	if (QFile::exists(directory + "/background.bmp")) {
		QPixmap image(directory + "/background.bmp");
		label->setPixmap(image);
	}

	// Set the background mask
	if (QFile::exists(directory + "/mask.bmp")) {
		QPixmap image(directory + "/mask.bmp");
		setMask(image);
	}

	// Set the alpha background
	if (QFile::exists(directory + "/alpha.png")) {
		platform.CreateAlphaBorder(this, directory + "/alpha.png");
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
		platform.Execute("http://www.launchy.net/donate.html", "");
		QDateTime def;
		gSettings->setValue("donateTime", def);
	}
}

void MyWidget::showLaunchy() {
	shouldDonate();
	visible = true;
	alternatives->hide();
	show();
	platform.ShowAlphaBorder();

	input->activateWindow();
//	activateWindow();
	input->selectAll();
	input->setFocus();
}

void MyWidget::hideLaunchy() {
	if (dropTimer != NULL && dropTimer->isActive())
		dropTimer->stop();
	if (alwaysShowLaunchy) return;
	visible = false;
	hide();
	if (alternatives != NULL)
		alternatives->hide();
	platform.HideAlphaBorder();
}



int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QStringList args = qApp->arguments();

	QCoreApplication::setApplicationName("Launchy");
	QCoreApplication::setOrganizationDomain("launchy.net");

	DSingleApplication instance( "LAUNCHY" );

	if ( instance.isRunning() ) {
//		instance.sendMessage( "Hey, i'm the other instance" );
		return 0;
	}




	MyWidget widget;
	widget.setObjectName("main");



	return app.exec();
} 