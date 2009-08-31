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
#include <QTranslator>
#include <QNetworkProxy>

#include "icon_delegate.h"
#include "main.h"
#include "globals.h"
#include "options.h"
#include "plugin_interface.h"


LaunchyWidget::LaunchyWidget(QWidget *parent,  PlatformBase * plat, bool show)
:
#ifdef Q_WS_WIN
QWidget(parent, Qt::FramelessWindowHint | Qt::Tool ),
#endif
#ifdef Q_WS_X11
QWidget(parent, Qt::FramelessWindowHint | Qt::Tool ),
#endif
	platform(plat),
	updateTimer(NULL),
	dropTimer(NULL),
	alternatives(NULL),
	iconExtractor(platform)
{
	setAttribute(Qt::WA_AlwaysShowToolTips);
	setAttribute(Qt::WA_InputMethodEnabled);

	connect(&iconExtractor, SIGNAL(iconExtracted(int, QIcon)), this, SLOT(iconExtracted(int, QIcon)));

	gMainWidget = this;
	menuOpen = false;
	optionsOpen = false;
	gSearchTxt = "";

	setFocusPolicy(Qt::ClickFocus);

	alwaysShowLaunchy = false;

	fader = new Fader(this);
	connect(fader, SIGNAL(fadeLevel(double)), this, SLOT(setFadeLevel(double)));
	connect(fader, SIGNAL(finishedFade(double)), this, SLOT(finishedFade(double)));

	label = new QLabel(this);

	optionsButton = new QPushButton(label);
	optionsButton->setObjectName("opsButton");
	optionsButton->setToolTip(tr("Launchy Options"));
	connect(optionsButton, SIGNAL(clicked()), this, SLOT(menuOptions()));

	closeButton = new QPushButton(label);
	closeButton->setObjectName("closeButton");
	closeButton->setToolTip(tr("Close Launchy"));
	connect(closeButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	output = new LineEditMenu(label);
	output->setAlignment(Qt::AlignHCenter);
	output->setEnabled(false);
	output->setObjectName("output");
	connect(output, SIGNAL(menuEvent(QContextMenuEvent*)), this, SLOT(menuEvent(QContextMenuEvent*)));

	input = new CharLineEdit(label);
	input->setObjectName("input");
	connect(input, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(inputKeyPressEvent(QKeyEvent*)));
	connect(input, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));
	connect(input, SIGNAL(inputMethod(QInputMethodEvent*)), this, SLOT(inputMethodEvent(QInputMethodEvent*)));

	labelIcon = new QLabel(label);

	dirs = platform->GetDirectories();

	// Load settings
	if (QFile::exists(dirs["portConfig"][0]))
		gSettings = new QSettings(dirs["portConfig"][0], QSettings::IniFormat, this);
	else
		gSettings = new QSettings(dirs["config"][0], QSettings::IniFormat, this);

	// If this is the first time running or a new version, call updateVersion
	if (gSettings->value("version", 0).toInt() != LAUNCHY_VERSION)
	{
		updateVersion(gSettings->value("version", 0).toInt());
		show = true;
	}

	alternatives = new CharListWidget(this);
	listDelegate = new IconDelegate(this);
	defaultDelegate = alternatives->itemDelegate();
	setCondensed(gSettings->value("GenOps/condensedView", false).toBool());
	alternatives->setObjectName("alternatives");
	alternatives->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	alternatives->setTextElideMode(Qt::ElideLeft);
	alternatives->setWindowFlags(Qt::Window | Qt::Tool | Qt::FramelessWindowHint);
	altScroll = alternatives->verticalScrollBar();
	altScroll->setObjectName("altScroll");
	connect(alternatives, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(altKeyPressEvent(QKeyEvent*)));
	connect(alternatives, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));

	// Load the plugins
	plugins.loadPlugins();

	// Load the skin
	applySkin(gSettings->value("GenOps/skin", dirs["defSkin"][0]).toString());

	// Move to saved position
	QPoint x = loadPosition();
	move(x);
	platform->MoveAlphaBorder(x);

	// Set the general options
	setAlwaysShow(gSettings->value("GenOps/alwaysshow", false).toBool());
	setAlwaysTop(gSettings->value("GenOps/alwaystop", false).toBool());
	setPortable(gSettings->value("GenOps/isportable", false).toBool());

	// Check for udpates?
	if (gSettings->value("GenOps/updatecheck", true).toBool())
	{
		checkForUpdate();
	}

	// Set the hotkey
#ifdef Q_WS_WIN
	int curMeta = gSettings->value("GenOps/hotkeyModifier", Qt::AltModifier).toInt();
#elif Q_WS_X11
	int curMeta = gSettings->value("GenOps/hotkeyModifier", Qt::ControlModifier).toInt();
#endif
	int curAction = gSettings->value("GenOps/hotkeyAction", Qt::Key_Space).toInt();
	if (!setHotkey(curMeta, curAction))
	{
		QMessageBox::warning(this, tr("Launchy"), tr("The hotkey you have chosen is already in use. Please select another from Launchy's preferences."));
		show = true;
	}

	// Set the timers
	updateTimer = new QTimer(this);
	dropTimer = new QTimer(this);
	dropTimer->setSingleShot(true);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimeout()));
	connect(dropTimer, SIGNAL(timeout()), this, SLOT(dropTimeout()));
	int time = gSettings->value("GenOps/updatetimer", 10).toInt();
	if (time > 0)
		updateTimer->start(time * 60000);

	// Load the catalog
	gBuilder.reset(new CatalogBuilder(true, &plugins));
	connect(gBuilder.get(), SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
	gBuilder->start();

	if (show)
		showLaunchy();
	else
		hideLaunchy();

	loadOptions();
}


LaunchyWidget::~LaunchyWidget()
{
	delete updateTimer;
	delete dropTimer;
	delete alternatives;
	platform.reset();
}


bool LaunchyWidget::event(QEvent *e)
{
	if (e->type() == QEvent::User)
	{
		showLaunchy();
		return true;
	}

	return QWidget::event(e);
}

void LaunchyWidget::setCondensed(int condensed)
{
	if (alternatives == NULL || listDelegate == NULL || defaultDelegate == NULL)
		return;
	if (condensed)
		alternatives->setItemDelegate(defaultDelegate);
	else
		alternatives->setItemDelegate(listDelegate);
}


bool LaunchyWidget::setHotkey(int meta, int key)
{
	QKeySequence keys = QKeySequence(meta + key);
	return platform->SetHotkey(keys, this, SLOT(onHotKey()));
}



void LaunchyWidget::menuEvent(QContextMenuEvent* evt)
{
	contextMenuEvent(evt);
}


void LaunchyWidget::showAlternatives(bool show)
{
	if (!isVisible())
		return;

	if (show)
	{
		if (searchResults.size() < 1)
			return;

		QRect n = altRect;
		n.translate(pos());
		alternatives->setGeometry(n);

		alternatives->clear();

		for(int i = 0; i < searchResults.size(); ++i)
		{
			QListWidgetItem * item = new QListWidgetItem(QDir::toNativeSeparators(searchResults[i].fullPath), alternatives);
			item->setData(ROLE_FULL, QDir::toNativeSeparators(searchResults[i].fullPath));
			item->setData(ROLE_SHORT, searchResults[i].shortName);
			item->setToolTip(QDir::toNativeSeparators(searchResults[i].fullPath));
			alternatives->addItem(item);
		}

		iconExtractor.processIcons(searchResults);

		if (alternatives->count() > 0)
		{
			int numViewable = gSettings->value("GenOps/numviewable", "4").toInt();
			//QRect r = alternatives->geometry();
			int min = alternatives->count() < numViewable ? alternatives->count() : numViewable;
			n.setHeight(min * alternatives->sizeHintForRow(0));

			altRect.setHeight(n.height());

			// Is there room for the dropdown box?
			if (n.y() + n.height() > qApp->desktop()->height())
			{
				n.moveTop(pos().y() + input->pos().y() - n.height());
			}
			alternatives->setGeometry(n);
		}
		double opaqueness = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
		opaqueness /= 100.0;
		alternatives->setWindowOpacity(opaqueness);
		alternatives->show();
		alternatives->setFocus();
		qApp->syncX();
		alternatives->raise();
	}
	else
	{
		alternatives->hide();
		iconExtractor.stop();
	}
}


void LaunchyWidget::launchObject()
{
	CatItem res = inputData[0].getTopResult();
	if (res.id == HASH_LAUNCHY)
	{
		QString args = "";
		if (inputData.count() > 1)
			for(int i = 1; i < inputData.count(); ++i)
				args += inputData[i].getText() + " ";
		if (!platform->Execute(res.fullPath, args))
			runProgram(res.fullPath, args);
	}
	else
	{
		int ops = plugins.execute(&inputData, &res);
		if (ops > 1)
		{
			switch (ops)
			{
			case MSG_CONTROL_EXIT:
				close();
				break;
			case MSG_CONTROL_OPTIONS:
				menuOptions();
				break;
			case MSG_CONTROL_REBUILD:
				buildCatalog();
				break;
			default:
				break;
			}
		}
	}
	catalog->incrementUsage(res);
}


void LaunchyWidget::focusOutEvent( QFocusEvent * evt)
{
	if (evt->reason() == Qt::ActiveWindowFocusReason)
	{
		if (gSettings->value("GenOps/hideiflostfocus", false).toBool())
			if (!this->isActiveWindow() && !alternatives->isActiveWindow() && !optionsOpen)
			{
				hideLaunchy();
			}
	}
}


void LaunchyWidget::altKeyPressEvent(QKeyEvent* key)
{
	if (key->key() == Qt::Key_Escape)
	{
		showAlternatives(false);
		key->ignore();
	}
	else if (key->key() == Qt::Key_Up) {key->ignore();}
	else if (key->key() == Qt::Key_Down) {key->ignore();}
	else if (key->key() == Qt::Key_PageUp) {key->ignore();}
	else if (key->key() == Qt::Key_PageDown) {key->ignore();}
	else if (key->key() == Qt::Key_Control) {key->ignore();}
	else if (key->key() == Qt::Key_Shift) {key->ignore();}
	else if (key->key() == Qt::Key_Alt) {key->ignore();}
	else if (key->key() == Qt::Key_AltGr) {key->ignore();}
	else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Tab)
	{
		if (searchResults.count() > 0)
		{
			int row = alternatives->currentRow();
			if (row > -1)
			{
				QString location = "History/" + input->text();
				QStringList hist;
				hist << searchResults[row].lowName <<
					searchResults[row].fullPath;
				gSettings->setValue(location, hist);

				CatItem tmp = searchResults[row];
				searchResults[row] = searchResults[0];
				searchResults[0] = tmp;

				updateDisplay();

				/* This seems to be unnecessary
				if (key->key() == Qt::Key_Tab)
				{
				inputData.last().setText(searchResults[0].fullPath);
				input->setText(printInput() + searchResults[0].fullPath);
				}
				*/
				showAlternatives(false);

				if (key->key() == Qt::Key_Tab)
				{
					doTab();
					parseInput(input->text());
					searchOnInput();
					updateDisplay();
					dropTimer->stop();
					int delay = gSettings->value("GenOps/autoSuggestDelay",1000).toInt();
					if (delay > 0)
						dropTimer->start(delay);
				}
				else
				{
					doEnter();
				}
			}
		}
	}
	else
	{
		activateWindow();
		raise();
		input->setFocus();
		key->ignore();
		input->keyPressEvent(key);
		keyPressEvent(key);
	}
}


void LaunchyWidget::inputKeyPressEvent(QKeyEvent* key)
{
	if (key->key() == Qt::Key_Tab)
	{
		keyPressEvent(key);
	}
	else
	{
		key->ignore();
	}

	if (input->text().length() == 0)
		showAlternatives(false);
}


void LaunchyWidget::parseInput(QString text)
{
	QStringList split = text.split(input->separatorText());

	if (split.count() < inputData.count())
	{
		inputData = inputData.mid(0, split.count());
	}


	for(int i = 0; i < inputData.size(); i++)
	{
		if (inputData[i].getText() != split[i])
		{
			inputData = inputData.mid(0, i);
			break;
		}
	}

	for(int i = inputData.count(); i < split.count(); i++)
	{
		InputData data(split[i]);
		inputData.push_back(data);
	}

}


// Print all of the input up to the last entry
QString LaunchyWidget::printInput()
{
	QString res = "";
	for(int i = 0; i < inputData.count()-1; ++i)
	{
		res += inputData[i].getText();
		res += input->separatorText();
	}
	return res;
}


void LaunchyWidget::doTab()
{
	if (inputData.count() > 0 && searchResults.count() > 0)
	{
		// If it's an incomplete file or dir, complete it
		QFileInfo info(searchResults[0].fullPath);

		if ((inputData.last().hasLabel(LABEL_FILE) || info.isDir())
			)//	&& input->text().compare(QDir::toNativeSeparators(searchResults[0].fullPath), Qt::CaseInsensitive) != 0)
		{
			QString path;
			if (info.isSymLink())
				path = info.symLinkTarget();
			else
				path = searchResults[0].fullPath;

			if (info.isDir() && !path.endsWith(QDir::separator()))
				path += QDir::separator();

			input->setText(printInput() + QDir::toNativeSeparators(path));
		}
		else
		{
			// Looking for a plugin
			input->setText(input->text() + input->separatorText());
			inputData.last().setText(searchResults[0].shortName);
			input->setText(printInput() + searchResults[0].shortName + input->separatorText());

			QRect rect = input->rect();
			repaint(rect);
		}
	}
}


void LaunchyWidget::doEnter()
{
	if (dropTimer->isActive())
		dropTimer->stop();

	if (searchResults.count() > 0 || inputData.count() > 1)
		launchObject();
	hideLaunchy();
}


void LaunchyWidget::keyPressEvent(QKeyEvent* key)
{
	if (key->key() == Qt::Key_F5 && (key->modifiers() & ~Qt::ShiftModifier) == 0)
	{
		setSkin(gSettings->value("GenOps/skin", dirs["defSkin"][0]).toString(),"");
		if ((key->modifiers() & Qt::ShiftModifier) == 0)
			buildCatalog();
	}

	else if (key->key() == Qt::Key_Escape)
	{
		if (alternatives->isVisible())
			showAlternatives(false);
		else
			hideLaunchy();
	}

	else if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter)
	{
		doEnter();
	}

	else if (key->key() == Qt::Key_Down || key->key() == Qt::Key_PageDown ||
		key->key() == Qt::Key_PageUp)
	{
		if (alternatives->isVisible())
		{
			if (this->isActiveWindow())
			{
				if (alternatives->currentRow() < 0 && alternatives->count() > 0)
				{
					alternatives->activateWindow();
					alternatives->setCurrentRow(0);
				}
				else
				{
					alternatives->activateWindow();
					qApp->sendEvent(alternatives, key);
				}
			}
		}
		else if (key->key() == Qt::Key_Down || key->key() == Qt::Key_PageDown)
		{
			dropTimer->stop();
			showAlternatives();
			alternatives->activateWindow();
			if (alternatives->count() > 0)
				alternatives->setCurrentRow(0);
		}
	}

	else if (
		key->key() == Qt::Key_Left ||
		key->key() == Qt::Key_Right ||
		key->key() == Qt::Key_Up ||
		key->key() == Qt::Key_Down ||
		key->key() == Qt::Key_PageUp ||
		key->key() == Qt::Key_PageDown ||
		key->key() == Qt::Key_Control ||
		key->key() == Qt::Key_Shift ||
		key->key() == Qt::Key_Alt ||
		key->key() == Qt::Key_AltGr)
	{
		// Prevent alternatives from being hidden
	}

	else
	{
		if (key->key() == Qt::Key_Tab)
		{
			doTab();
		}
		else if (key->key() == Qt::Key_Slash || key->key() == Qt::Key_Backslash)
		{
			if (inputData.size() > 0 && inputData.last().hasLabel(LABEL_FILE))
				doTab();
		}
		else
		{
			key->ignore();
		}

		processKey();

	}
}


void LaunchyWidget::processKey()
{
	dropTimer->stop();
	int delay = gSettings->value("GenOps/autoSuggestDelay",1000).toInt();
	if (delay > 0)
		dropTimer->start(delay);

	parseInput(input->text());
	searchOnInput();
	updateDisplay();
}


void LaunchyWidget::inputMethodEvent(QInputMethodEvent *e)
{
	e = e; // Warning removal
	processKey();
}


void LaunchyWidget::searchOnInput()
{
	if (catalog == NULL) return;
	if (inputData.count() == 0) return;

	QString stxt = inputData.last().getText();
	gSearchTxt = stxt;
	searchResults.clear();

	if (inputData.count() <= 1)
		catalog->searchCatalogs(gSearchTxt, searchResults);

	if (searchResults.count() != 0)
		inputData.last().setTopResult(searchResults[0]);

	plugins.getLabels(&inputData);
	plugins.getResults(&inputData, &searchResults);
	qSort(searchResults.begin(), searchResults.end(), CatLessNoPtr);

	//	    qDebug() << gSearchTxt;
	// Is it a file?

	if (stxt.contains(QDir::separator()) || stxt.startsWith("~") || (stxt.size() == 2 && stxt[1] == ':'))
	{
		searchFiles(stxt, searchResults);
	}
	catalog->checkHistory(gSearchTxt, searchResults);
}


void LaunchyWidget::updateDisplay()
{
	if (searchResults.count() > 0)
	{
		QIcon icon = iconExtractor.getIcon(searchResults[0]);
		labelIcon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
		output->setText(searchResults[0].shortName);

		// Did the plugin take control of the input?
		if (inputData.last().getID() != 0)
			searchResults[0].id = inputData.last().getID();

		inputData.last().setTopResult(searchResults[0]);

	}
	else
	{
		labelIcon->clear();
		output->clear();
	}
}


void LaunchyWidget::iconExtracted(int itemIndex, QIcon icon)
{
	if (itemIndex < alternatives->count())
	{
		QListWidgetItem* listItem = alternatives->item(itemIndex);
		listItem->setIcon(icon);
		listItem->setData(ROLE_ICON, icon);

		QRect rect = alternatives->visualItemRect(listItem);
		repaint(rect);
	}
}


void LaunchyWidget::searchFiles(const QString & input, QList<CatItem>& searchResults)
{
	// Split the string on the last slash
	QString path = QDir::fromNativeSeparators(input);
	if (path.startsWith("~"))
		path.replace("~",QDir::homePath());

	if (path.size() == 2 && path[1] == ':')
		path += "/";

	// Network searches are too slow
	if (path.startsWith("//"))
		return;

	QString dir, file;
	dir = path.mid(0,path.lastIndexOf("/")+1);
	file = path.mid(path.lastIndexOf("/")+1);


	QFileInfo info(dir);
	if (!info.isDir())
		return;

	inputData.last().setLabel(LABEL_FILE);

	// Okay, we have a directory, find files that match "file"
	QDir qd(dir);
	QStringList ilist;
	if (gSettings->value("GenOps/showHiddenFiles", false).toBool())
		ilist = qd.entryList(QStringList(file + "*"), QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);
	else
		ilist = qd.entryList(QStringList(file + "*"), QDir::AllDirs | QDir::Files , QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);

	for(int i = ilist.size()-1; i >= 0; i--)
	{
		QString inf = ilist[i];

		if (inf.startsWith("."))
			continue;
		if (inf.mid(0, file.count()).compare(file,  Qt::CaseInsensitive) == 0)
		{
			QString fp = qd.absolutePath() + "/" + inf;
			fp = QDir::cleanPath(fp);
			QFileInfo in(fp);
			if (in.isDir())
				fp += "/";


			CatItem item(QDir::toNativeSeparators(fp), inf);
			searchResults.push_front(item);
		}
	}

	// Showing a directory
	if (file == "")
	{
		QString n = QDir::toNativeSeparators(dir);
		if (!n.endsWith(QDir::separator()))
			n += QDir::separator();
		CatItem item(n);
		searchResults.push_front(item);
	}
}


void LaunchyWidget::catalogBuilt()
{
	catalog = gBuilder->getCatalog();

	gBuilder->wait();
	gBuilder.reset();

	// Do a search here of the current input text
	searchOnInput();
	updateDisplay();
}


void LaunchyWidget::checkForUpdate()
{
	http = new QHttp(this);
	verBuffer = new QBuffer(this);
	counterBuffer = new QBuffer(this);
	verBuffer->open(QIODevice::ReadWrite);
	counterBuffer->open(QIODevice::ReadWrite);

	connect(http, SIGNAL(done( bool)), this, SLOT(httpGetFinished( bool)));
	http->setHost("www.launchy.net");
	http->get("http://www.launchy.net/version2.html", verBuffer);

	/*
	QHttpRequestHeader header("GET", "/n?id=AEJV3A4l/cDSX3qBPvhGeIRGerIg");
	header.setValue("Host", "m1.webstats.motigo.com");
	header.setValue("Referer", "http://www.launchy.net/stats.html");
	header.setContentType("image/gif, text/plain, text/html, text/htm");
	http->setHost("m1.webstats.motigo.com");
	http->request(header, NULL, counterBuffer);
	*/
}


void LaunchyWidget::httpGetFinished(bool error)
{
	if (!error)
	{
		QString str(verBuffer->data());
		int ver = str.toInt();
		if (ver > LAUNCHY_VERSION)
		{
			QMessageBox box;
			box.setIcon(QMessageBox::Information);
			box.setTextFormat(Qt::RichText);
			box.setWindowTitle(tr("A new version of Launchy is available"));
			box.setText(tr("A new version of Launchy is available.\n\nYou can download it at \
						   <qt><a href=\"http://www.launchy.net/\">http://www.launchy.net</a></qt>"));
			box.exec();
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


void LaunchyWidget::setSkin(QString dir, QString name)
{
	bool wasShowing = isVisible();
	QPoint p = pos();
	hideLaunchy(true);
	applySkin(dir + "/" + name);

	move(p);
	platform->MoveAlphaBorder(p);
	platform->ShowAlphaBorder();
	if (wasShowing)
		showLaunchy(true);
}


void LaunchyWidget::updateVersion(int oldVersion)
{
	if (oldVersion < 199)
	{
		// We've completely changed the database and ini between 1.25 and 2.0
		// Erase all of the old information
		QString origFile = gSettings->fileName();
		delete gSettings;

		QFile oldIniPerm(dirs["config"][0]);
		oldIniPerm.remove();
		oldIniPerm.close();

		QFile oldDbPerm(dirs["db"][0]);
		oldDbPerm.remove();
		oldDbPerm.close();

		QFile oldDB(dirs["portDB"][0]);
		oldDB.remove();
		oldDB.close();

		QFile oldIni(dirs["portConfig"][0]);
		oldIni.remove();
		oldIni.close();

		gSettings = new QSettings(origFile, QSettings::IniFormat, this);
	}

	if (oldVersion < 210)
	{
		QString oldSkin = gSettings->value("GenOps/skin", dirs["defSkin"][0]).toString();
		QString newSkin = dirs["skins"][0] + "/" + oldSkin;
		gSettings->setValue("GenOps/skin", newSkin);
	}

	if (oldVersion < LAUNCHY_VERSION)
	{
		gSettings->setValue("donateTime", QDateTime::currentDateTime().addDays(21));
		gSettings->setValue("version", LAUNCHY_VERSION);
	}
}


QPoint LaunchyWidget::loadPosition()
{
	QPoint pt = gSettings->value("Display/pos", QPoint(0,0)).toPoint();
	QRect r = geometry();
	int primary = qApp->desktop()->primaryScreen();
	QRect scr = qApp->desktop()->availableGeometry(primary);

	// See if pt is in the current screen resolution, if not pull it inside
	if (pt.x()+r.width()/2 < 0)
		pt.setX(0);
	else if (pt.x()-r.width()/2 > scr.width())
		pt.setX(scr.width()-r.width());
	if (pt.y()+r.height()/2 < 0)
		pt.setY(0);
	else if (pt.y()-r.height()/2 > scr.height())
		pt.setY(scr.height()-r.height());

	int center = gSettings->value("GenOps/alwayscenter", 3).toInt();
	if (center & 1)
		pt.setX(scr.width()/2 - r.width()/2);
	if (center & 2)
		pt.setY(scr.height()/2 - r.height()/2);

	return pt;
}


void LaunchyWidget::updateTimeout()
{
	// Save the settings periodically
	savePosition();
	gSettings->sync();

	// Perform the database update
	if (gBuilder == NULL)
	{
		gBuilder.reset(new CatalogBuilder(false, &plugins));
		gBuilder->setPreviousCatalog(catalog);
		connect(gBuilder.get(), SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
		gBuilder->start(QThread::IdlePriority);
	}

	int time = gSettings->value("GenOps/updatetimer", 10).toInt();
	updateTimer->stop();
	if (time != 0)
		updateTimer->start(time * 60000);
}


void LaunchyWidget::dropTimeout()
{
	if (input->text() != "")
		showAlternatives();
}


void LaunchyWidget::onHotKey()
{
	if (menuOpen || optionsOpen)
	{
		showLaunchy();
		return;
	}
	if (isVisible())
	{
		hideLaunchy();
	}
	else
	{
		showLaunchy();
	}
}


void LaunchyWidget::closeEvent(QCloseEvent *event)
{
	savePosition();
	gSettings->sync();

	QDir dest(gSettings->fileName());
	dest.cdUp();
	CatalogBuilder builder(catalog, &plugins);
	builder.storeCatalog(dest.absoluteFilePath("Launchy.db"));

	platform.reset();

	event->accept();
	qApp->quit();
}


void LaunchyWidget::setAlwaysShow(bool alwaysShow)
{
	alwaysShowLaunchy = alwaysShow;
	if (!isVisible()  && alwaysShow)
		showLaunchy();
}


void LaunchyWidget::setAlwaysTop(bool alwaysTop)
{
	if (alwaysTop)	{
		setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint);
	}
	else
	{
		if ((windowFlags() & Qt::WindowStaysOnTopHint) != 0)
			setWindowFlags( windowFlags() & ~Qt::WindowStaysOnTopHint);
	}

}


void LaunchyWidget::setPortable(bool portable)
{
	if (portable && gSettings->fileName().compare(dirs["portConfig"][0], Qt::CaseInsensitive) != 0)
	{
		delete gSettings;

		// Copy the old settings
		QFile oldSet(dirs["config"][0]);
		oldSet.copy(dirs["portConfig"][0]);
		oldSet.close();

		QFile oldDB(dirs["db"][0]);
		oldDB.copy(dirs["portDB"][0]);
		oldDB.close();

		gSettings = new QSettings(dirs["portConfig"][0], QSettings::IniFormat, this);
	}
	else if (!portable && gSettings->fileName().compare(dirs["portConfig"][0], Qt::CaseInsensitive) == 0)
	{
		delete gSettings;

		// Remove the ini file we're going to copy to so that copy can work
		QFile newF(dirs["config"][0]);
		newF.remove();
		newF.close();

		// Copy the local ini + db files to the users section
		QFile oldSet(dirs["portConfig"][0]);
		oldSet.copy(dirs["config"][0]);
		oldSet.remove();
		oldSet.close();

		QFile oldDB(dirs["portDB"][0]);
		oldDB.copy(dirs["db"][0]);
		oldDB.remove();
		oldDB.close();

		// Load up the user section ini file
		gSettings = new QSettings(dirs["config"][0], QSettings::IniFormat, this);

	}
}


void LaunchyWidget::setOpaqueness(int val)
{
	double max = (double)val;
	max /= 100.0;
	this->setWindowOpacity(max);
	platform->SetAlphaOpacity(max);
}


void LaunchyWidget::applySkin(QString directory)
{
	// Hide the buttons by default
	closeButton->hide();
	optionsButton->hide();

	if (listDelegate == NULL)
		return;

	// Use default skin if this one doesn't exist
	if (!QFile::exists(directory + "/misc.txt")) 
	{
		directory = dirs["defSkin"][0];
		gSettings->setValue("GenOps/skin", dirs["defSkin"][0]);
	}

	// Set positions
	if (QFile::exists(directory + "/misc.txt"))
	{
		QFile file(directory + "/misc.txt");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			while (!in.atEnd())
			{
				QString line = in.readLine();
				if (line.startsWith(";")) continue;
				QStringList spl = line.split("=");
				if (spl.size() == 2)
				{
					QStringList sizes = spl.at(1).trimmed().split(",");
					QRect rect;
					if (sizes.size() == 4)
					{
						rect.setRect(sizes[0].toInt(), sizes[1].toInt(), sizes[2].toInt(), sizes[3].toInt());
					}

					if (spl.at(0).trimmed().compare("input", Qt::CaseInsensitive) == 0)
						input->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("inputAlignment", Qt::CaseInsensitive) == 0)
						input->setAlignment(
						sizes[0].trimmed().compare("left", Qt::CaseInsensitive) == 0 ? Qt::AlignLeft : 
						sizes[0].trimmed().compare("right", Qt::CaseInsensitive) == 0 ? Qt::AlignRight : Qt::AlignHCenter );
					else if (spl.at(0).trimmed().compare("output", Qt::CaseInsensitive) == 0) 
						output->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("outputAlignment", Qt::CaseInsensitive) == 0)
						output->setAlignment(
						sizes[0].trimmed().compare("left", Qt::CaseInsensitive) == 0 ? Qt::AlignLeft : 
						sizes[0].trimmed().compare("right", Qt::CaseInsensitive) == 0 ? Qt::AlignRight : Qt::AlignHCenter );
					else if (spl.at(0).trimmed().compare("alternatives", Qt::CaseInsensitive) == 0)
						altRect = rect;
					else if (spl.at(0).trimmed().compare("boundary", Qt::CaseInsensitive) == 0)
					{
						setGeometry(rect);
						label->setGeometry(rect);
					}
					else if (spl.at(0).trimmed().compare("icon", Qt::CaseInsensitive) == 0)
						labelIcon->setGeometry(rect);
					else if (spl.at(0).trimmed().compare("optionsbutton", Qt::CaseInsensitive) == 0)
					{
						optionsButton->setGeometry(rect);
						optionsButton->show();
					}
					else if (spl.at(0).trimmed().compare("closebutton", Qt::CaseInsensitive) == 0)
					{
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
	if (QFile::exists(directory + "/style.qss"))
	{
		QFile file(directory + "/style.qss");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QString styleSheet = QLatin1String(file.readAll());
			// This is causing the ::destroyed connect errors
			qApp->setStyleSheet(styleSheet);
			file.close();
		}
	}

	// Set the background image
	if (!platform->SupportsAlphaBorder() && QFile::exists(directory + "/background_nc.png"))
	{
		QPixmap image(directory + "/background_nc.png");
		label->setPixmap(image);
	}

	else if (QFile::exists(directory + "/background.png"))
	{
		QPixmap image(directory + "/background.png");
		label->setPixmap(image);
		if (image.hasAlpha())
			setMask(image.mask());
	}

	// Set the background mask
	if (!platform->SupportsAlphaBorder() && QFile::exists(directory + "/mask_nc.png"))
	{
		QPixmap image(directory + "/mask_nc.png");
		setMask(image);
	}

	else if (QFile::exists(directory + "/mask.png"))
	{
		QPixmap image(directory + "/mask.png");
		// For some reason, w/ compiz setmask won't work
		// for rectangular areas.  This is due to compiz and
		// XShapeCombineMask
		setMask(image );
	}

	// Set the alpha background
	if (QFile::exists(directory + "/alpha.png") && platform->SupportsAlphaBorder())
	{
		platform->CreateAlphaBorder(this, directory + "/alpha.png");
		connectAlpha();
		platform->MoveAlphaBorder(pos());
	}

}


void LaunchyWidget::connectAlpha()
{
	shared_ptr<QWidget> w = platform->getAlphaWidget();
	if (!w) return;
	connect(w.get(), SIGNAL(menuEvent(QContextMenuEvent*)), this, SLOT(menuEvent(QContextMenuEvent*)));
	connect(w.get(), SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
	connect(w.get(), SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
}


void LaunchyWidget::mousePressEvent(QMouseEvent *e)
{
	//showAlternatives(false);
	activateWindow();
	raise();
	showAlternatives(false);
	moveStartPoint = e->pos();
	input->setFocus();
}


void LaunchyWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPoint p = e->globalPos();
	p -= moveStartPoint;
	move(p);
	platform->MoveAlphaBorder(p);
	showAlternatives(false);
	input->setFocus();
}


void LaunchyWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	QAction* actMenu = menu.addAction(tr("Rebuild Catalog"));
	connect(actMenu, SIGNAL(triggered()), this, SLOT(buildCatalog()));
	QAction* actOptions = menu.addAction(tr("Options"));
	connect(actOptions, SIGNAL(triggered()), this, SLOT(menuOptions()));
	menu.addSeparator();
	QAction* actExit = menu.addAction(tr("Exit"));
	connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
	menuOpen = true;
	menu.exec(event->globalPos());
	menuOpen = false;
}


void LaunchyWidget::buildCatalog()
{
	// Perform the database update
	if (gBuilder == NULL)
	{
		gBuilder.reset(new CatalogBuilder(false, &plugins));
		gBuilder->setPreviousCatalog(catalog);
		connect(gBuilder.get(), SIGNAL(catalogFinished()), this, SLOT(catalogBuilt()));
		gBuilder->start(QThread::IdlePriority);
	}
}


void LaunchyWidget::menuOptions()
{
	dropTimer->stop();
	showAlternatives(false);
	optionsOpen = true;
	OptionsDialog options(this);
	options.setObjectName("options");
	options.exec();

	// Perform the database update
	if (gBuilder == NULL)
		buildCatalog();

	input->activateWindow();
	input->setFocus();
	optionsOpen = false;
}


void LaunchyWidget::shouldDonate()
{
	QDateTime time = QDateTime::currentDateTime();
	QDateTime donateTime = gSettings->value("donateTime",time.addDays(21)).toDateTime();
	if (donateTime.isNull()) return;
	gSettings->setValue("donateTime", donateTime);

	if (donateTime <= time)
	{
#ifdef Q_WS_WIN
		runProgram("http://www.launchy.net/donate.html", "");
#endif
		QDateTime def;
		gSettings->setValue("donateTime", def);
	}
}


void LaunchyWidget::setFadeLevel(double d)
{
	this->setWindowOpacity(d);
	platform->SetAlphaOpacity(d);
}


void LaunchyWidget::finishedFade(double d)
{
	if (d == 0.0)
	{
		hide();
		platform->HideAlphaBorder();
	}
}


void LaunchyWidget::fadeIn()
{
	if (fader->isRunning())
	{
		fader->stop();
		fader->wait();
	}
	fader->setFadeType(true);
	fader->start();
}


void LaunchyWidget::fadeOut()
{
	if (fader->isRunning())
	{
		fader->stop();
		fader->wait();
	}
	fader->setFadeType(false);
	fader->start();
}


void LaunchyWidget::showLaunchy(bool now)
{
	shouldDonate();
	showAlternatives(false);

	// This gets around the weird Vista bug
	// where the alpha border would dissappear
	// on sleep or user switch

	move(loadPosition());
#ifdef Q_WS_WIN
	platform->CreateAlphaBorder(this, "");
	connectAlpha();
#endif
	platform->MoveAlphaBorder(pos());

	setFadeLevel(0.0);

	platform->ShowAlphaBorder();
	this->show();

	if (!now)
	{
		fadeIn();
	}
	else
	{
		double end = (double) gSettings->value("GenOps/opaqueness", 100).toInt();
		end /= 100.0;
		setFadeLevel(end);
	}

#ifdef Q_WS_X11
	// Terrible hack to steal focus from other apps when using splashscreen
	for(int i = 0; i < 100; i++)
	{
		activateWindow();
		raise();
		qApp->syncX();
	}
#endif

	qApp->syncX();
	input->activateWindow();
	input->raise();
	input->selectAll();
	input->setFocus();
	qApp->syncX();
	// Let the plugins know
	plugins.showLaunchy();
}


void LaunchyWidget::hideLaunchy(bool now)
{
	if (!isVisible())
		return;
	savePosition();
	if (dropTimer != NULL && dropTimer->isActive())
		dropTimer->stop();
	if (alwaysShowLaunchy)
		return;

	if (alternatives != NULL)
		showAlternatives(false);

	if (isVisible())
	{
		if (!now)
			fadeOut();
		else
		{
			setFadeLevel(0.0);
			finishedFade(0.0);
		}
	}

	// let the plugins know
	plugins.hideLaunchy();
}


void LaunchyWidget::loadOptions()
{
	// If a network proxy server is specified, apply an application wide NetworkProxy setting
	QString proxyHost = gSettings->value("WebProxy/hostAddress", "").toString();
	if (proxyHost.length() > 0)
	{
		QNetworkProxy proxy;
		proxy.setType((QNetworkProxy::ProxyType)gSettings->value("WebProxy/type", 0).toInt());
		proxy.setHostName(gSettings->value("WebProxy/hostAddress", "").toString());
		proxy.setPort(gSettings->value("WebProxy/port", "").toInt());
		QNetworkProxy::setApplicationProxy(proxy);
	}
}


int main(int argc, char *argv[])
{
	PlatformBase * platform = loadPlatform();
	shared_ptr<QApplication> app(platform->init(argc, argv));

	QStringList args = qApp->arguments();
	app->setQuitOnLastWindowClosed(false);
	bool show = false;

	if (args.size() > 1)
	{
		foreach (QString arg, args)
		{
			if (arg.compare("/rescue", Qt::CaseInsensitive) == 0)
			{
				// Kill all existing Launchys
				//			platform->KillLaunchys();
				show = true;
			}
			else if (arg.compare("/show", Qt::CaseInsensitive) == 0)
			{
				show = true;
			}
		}
	}

	if (platform->isAlreadyRunning())
	{
		platform->showOtherInstance();
		exit(1);
	}

	QCoreApplication::setApplicationName("Launchy");
	QCoreApplication::setOrganizationDomain("Launchy");

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("tr/launchy_" + locale));
	app->installTranslator(&translator);

	LaunchyWidget widget(NULL, platform, show);
	widget.setObjectName("main");

	app->exec();
}
