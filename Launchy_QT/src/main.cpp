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


LaunchyWidget::LaunchyWidget(QWidget *parent, PlatformBase *plat, StartModes startMode)
:
#ifdef Q_WS_WIN
QWidget(parent, Qt::FramelessWindowHint | Qt::Tool),
#endif
#ifdef Q_WS_X11
QWidget(parent, Qt::FramelessWindowHint | Qt::Tool),
#endif
	platform(plat),
	updateTimer(NULL),
	dropTimer(NULL),
	alternatives(NULL),
	iconExtractor(platform),
	frameGraphic(NULL),
	trayIcon(NULL)
{
	setObjectName("launchy");
	setWindowTitle(tr("Launchy"));
	setAttribute(Qt::WA_AlwaysShowToolTips);
	setAttribute(Qt::WA_InputMethodEnabled);
	if (platform->SupportsAlphaBorder())
	{
		setAttribute(Qt::WA_TranslucentBackground);
	}
	setFocusPolicy(Qt::ClickFocus);

	createActions();

	gMainWidget = this;
	menuOpen = false;
	optionsOpen = false;
	dragging = false;
	gSearchTxt = "";

	alwaysShowLaunchy = false;

	connect(&iconExtractor, SIGNAL(iconExtracted(int, QIcon)), this, SLOT(iconExtracted(int, QIcon)));

	fader = new Fader(this);
	connect(fader, SIGNAL(fadeLevel(double)), this, SLOT(setFadeLevel(double)));

	optionsButton = new QPushButton(this);
	optionsButton->setObjectName("opsButton");
	optionsButton->setToolTip(tr("Launchy Options"));
	optionsButton->setGeometry(QRect());
	connect(optionsButton, SIGNAL(clicked()), this, SLOT(showOptionsDialog()));

	closeButton = new QPushButton(this);
	closeButton->setObjectName("closeButton");
	closeButton->setToolTip(tr("Close Launchy"));
	closeButton->setGeometry(QRect());
	connect(closeButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	output = new QLabel(this);
	output->setObjectName("output");
	output->setAlignment(Qt::AlignHCenter);

	input = new CharLineEdit(this);
	input->setObjectName("input");
	connect(input, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(inputKeyPressEvent(QKeyEvent*)));
	connect(input, SIGNAL(focusIn(QFocusEvent*)), this, SLOT(focusInEvent(QFocusEvent*)));
	connect(input, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));
	connect(input, SIGNAL(inputMethod(QInputMethodEvent*)), this, SLOT(inputMethodEvent(QInputMethodEvent*)));

	outputIcon = new QLabel(this);
	outputIcon->setObjectName("outputIcon");

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
		startMode |= ShowLaunchy;
	}

	alternatives = new CharListWidget(this);
	alternatives->setObjectName("alternatives");
	alternatives->setWindowFlags(Qt::Window | Qt::Tool | Qt::FramelessWindowHint);
	alternatives->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	alternatives->setTextElideMode(Qt::ElideLeft);
	listDelegate = new IconDelegate(this);
	defaultListDelegate = alternatives->itemDelegate();
	setCondensed(gSettings->value("GenOps/condensedView", false).toBool());
	altScroll = alternatives->verticalScrollBar();
	altScroll->setObjectName("altScroll");
	connect(alternatives, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(alternativesKeyPressEvent(QKeyEvent*)));
	connect(alternatives, SIGNAL(focusOut(QFocusEvent*)), this, SLOT(focusOutEvent(QFocusEvent*)));

	// Load the plugins
	plugins.loadPlugins();

	// Set the general options
	if (setAlwaysShow(gSettings->value("GenOps/alwaysshow", false).toBool()))
		startMode |= ShowLaunchy;
	setAlwaysTop(gSettings->value("GenOps/alwaystop", false).toBool());
	setPortable(gSettings->value("GenOps/isportable", false).toBool());

	// Check for udpates?
	if (gSettings->value("GenOps/updatecheck", true).toBool())
	{
		checkForUpdate();
	}

	// Set the hotkey
#ifdef Q_WS_WIN
	int curMeta = Qt::AltModifier;
#elif Q_WS_X11
	int curMeta = Qt::ControlModifier;
#endif
	curMeta = gSettings->value("GenOps/hotkeyModifier", curMeta).toInt();
	int curAction = gSettings->value("GenOps/hotkeyAction", Qt::Key_Space).toInt();

	QKeySequence hotkey(curMeta + curAction);
	if (!setHotkey(hotkey))
	{
		QMessageBox::warning(this, tr("Launchy"), tr("The hotkey %1 is already in use, please select another.").arg(hotkey.toString()));
		startMode = ShowLaunchy | ShowOptions;
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

	// Load the skin
	applySkin(gSettings->value("GenOps/skin", "Default").toString());

	// Move to saved position
	loadPosition();
	loadOptions();

	if (startMode & ShowLaunchy)
		showLaunchy();
	else
		hideLaunchy();

	if (startMode & ShowOptions)
		showOptionsDialog();
}


LaunchyWidget::~LaunchyWidget()
{
	delete updateTimer;
	delete dropTimer;
	delete alternatives;
	platform.reset();
}


bool LaunchyWidget::event(QEvent* event)
{
	if (event->type() == QEvent::User)
	{
		showLaunchy();
		return true;
	}

	return QWidget::event(event);
}


void LaunchyWidget::paintEvent(QPaintEvent* event)
{
	// Do the default draw first render stylesheet backgrounds
	QStyleOption styleOption;
	styleOption.init(this);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);

	// If there's a frame graphic, draw it
	if (frameGraphic)
	{
		painter.drawPixmap(0,0, *frameGraphic);
	}

	QWidget::paintEvent(event);
}


void LaunchyWidget::setCondensed(int condensed)
{
	if (alternatives == NULL || listDelegate == NULL || defaultListDelegate == NULL)
		return;
	if (condensed)
		alternatives->setItemDelegate(defaultListDelegate);
	else
		alternatives->setItemDelegate(listDelegate);
}


bool LaunchyWidget::setHotkey(QKeySequence hotkey)
{
	return platform->setHotkey(hotkey, this, SLOT(onHotkey()));
}


void LaunchyWidget::showTrayIcon()
{
	if (!QSystemTrayIcon::isSystemTrayAvailable())
		return;

	if (gSettings->value("GenOps/showtrayicon", true).toBool())
	{
		trayIcon = new QSystemTrayIcon(this);
		QKeySequence hotkey = platform->getHotkey();
		trayIcon->setToolTip(tr("Launchy (press %1 to activate)").arg(hotkey.toString()));
		trayIcon->setIcon(QIcon(":/resources/launchy16.png"));
		trayIcon->show();
		connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

		QMenu* trayMenu = new QMenu(this);
		trayMenu->addAction(actShow);
		trayMenu->addAction(actRebuild);
		trayMenu->addAction(actOptions);
		trayMenu->addSeparator();
		trayMenu->addAction(actExit);

		trayIcon->setContextMenu(trayMenu);
	}
	else
	{
		delete trayIcon;
		trayIcon = NULL;
	}
}


void LaunchyWidget::showAlternatives(bool show)
{
	if (!isVisible())
		return;

	if (show)
	{
		if (searchResults.size() < 1)
			return;

		alternatives->clear();
		for (int i = 0; i < searchResults.size(); ++i)
		{
			QListWidgetItem * item = new QListWidgetItem(QDir::toNativeSeparators(searchResults[i].fullPath), alternatives);
			QString s;
			s.sprintf(" (%d)", searchResults[i].usage);
			item->setData(ROLE_FULL, QDir::toNativeSeparators(searchResults[i].fullPath) + s);
			item->setData(ROLE_SHORT, searchResults[i].shortName);
			item->setToolTip(QDir::toNativeSeparators(searchResults[i].fullPath));
			alternatives->addItem(item);
		}

		iconExtractor.processIcons(searchResults);

		int numViewable = gSettings->value("GenOps/numviewable", "4").toInt();
		int min = alternatives->count() < numViewable ? alternatives->count() : numViewable;

		// The stylesheet doesn't load immediately, so we cache the rect here
		if (alternativesRect.isNull())
			alternativesRect = alternatives->geometry();
		QRect rect = alternativesRect;
		rect.setHeight(min * alternatives->sizeHintForRow(0));
		rect.translate(pos());

		// Is there room for the dropdown box?
		if (rect.y() + rect.height() > qApp->desktop()->height())
		{
			// Only move it if there's more space above
			// In both cases, ensure it doesn't spill off the screen
			if (pos().y() + input->pos().y() > qApp->desktop()->height() / 2)
			{
				rect.moveTop(pos().y() + input->pos().y() - rect.height());
				if (rect.top() < 0)
					rect.setTop(0);
			}
			else
			{
				rect.setBottom(qApp->desktop()->height());
			}
		}
		alternatives->setGeometry(rect);

		alternatives->show();
		alternatives->setFocus();
		qApp->syncX();
	}
	else
	{
		alternatives->hide();
		iconExtractor.stop();
	}
}


void LaunchyWidget::launchItem(CatItem& item)
{
	if (item.id == HASH_LAUNCHY)
	{
		QString args = "";
		if (inputData.count() > 1)
			for(int i = 1; i < inputData.count(); ++i)
				args += inputData[i].getText() + " ";
		if (!platform->Execute(item.fullPath, args))
			runProgram(item.fullPath, args);
	}
	else
	{
		int ops = plugins.execute(&inputData, &item);
		if (ops > 1)
		{
			switch (ops)
			{
			case MSG_CONTROL_EXIT:
				close();
				break;
			case MSG_CONTROL_OPTIONS:
				showOptionsDialog();
				break;
			case MSG_CONTROL_REBUILD:
				buildCatalog();
				break;
			default:
				break;
			}
		}
	}
	catalog->incrementUsage(item);
	addToHistory(item);
}


void LaunchyWidget::addToHistory(const CatItem& catalogItem)
{
	CatItem item = catalogItem;
	item.fullPath = "";
	for (int i = 0; i < inputData.count(); ++i)
	{
		if (i > 0)
			item.fullPath += input->separatorText();
		item.fullPath += inputData[i].getText();
	}

	for (int i = 0; i < history.size(); ++i)
	{
		if (history[i].fullPath == item.fullPath)
		{
			history.removeAt(i);
			break;
		}
	}

	//history.push_front(item);
	if (history.size() > gSettings->value("GenOps/maxitemsinhistory", 20).toInt())
		history.pop_back();
}


void LaunchyWidget::focusInEvent(QFocusEvent* event)
{
	if (event->gotFocus() && fader->isFading())
		fader->fadeIn(false);

	QWidget::focusInEvent(event);
}


void LaunchyWidget::focusOutEvent(QFocusEvent* event)
{
	if (event->reason() == Qt::ActiveWindowFocusReason)
	{
		if (gSettings->value("GenOps/hideiflostfocus", false).toBool() &&
			!isActiveWindow() && !alternatives->isActiveWindow() && !optionsOpen)
		{
			hideLaunchy();
		}
	}
}


void LaunchyWidget::alternativesKeyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		showAlternatives(false);
		event->ignore();
	}
	else if (event->key() == Qt::Key_Up) {event->ignore();}
	else if (event->key() == Qt::Key_Down) {event->ignore();}
	else if (event->key() == Qt::Key_PageUp) {event->ignore();}
	else if (event->key() == Qt::Key_PageDown) {event->ignore();}
	else if (event->key() == Qt::Key_Control) {event->ignore();}
	else if (event->key() == Qt::Key_Shift) {event->ignore();}
	else if (event->key() == Qt::Key_Alt) {event->ignore();}
	else if (event->key() == Qt::Key_AltGr) {event->ignore();}
	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Tab)
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

				showAlternatives(false);

				if (event->key() == Qt::Key_Tab)
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
		input->setFocus();
		event->ignore();
		input->keyPressEvent(event);
		keyPressEvent(event);
	}
}


void LaunchyWidget::inputKeyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Tab)
	{
		keyPressEvent(event);
	}
	else
	{
		event->ignore();
	}

	if (input->text().length() == 0)
		showAlternatives(false);
}


void LaunchyWidget::parseInput(const QString& text)
{
	QStringList split = text.split(input->separatorText());

	if (split.count() < inputData.count())
	{
		inputData = inputData.mid(0, split.count());
	}

	for (int i = 0; i < inputData.size(); i++)
	{
		if (inputData[i].getText() != split[i])
		{
			inputData = inputData.mid(0, i);
			break;
		}
	}

	for (int i = inputData.count(); i < split.count(); i++)
	{
		InputData data(split[i]);
		inputData.push_back(data);
	}
}


// Print all of the input up to the last entry
QString LaunchyWidget::formatInput()
{
	QString res = "";
	for (int i = 0; i < inputData.count()-1; ++i)
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

			input->setText(formatInput() + QDir::toNativeSeparators(path));
		}
		else
		{
			// Looking for a plugin
			input->selectAll();
			input->insert(input->text() + input->separatorText());
			//input->setText(input->text() + input->separatorText());
			inputData.last().setText(searchResults[0].shortName);
			input->selectAll();
			input->insert(formatInput() + searchResults[0].shortName + input->separatorText());
			//input->setText(formatInput() + searchResults[0].shortName + input->separatorText());

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
	{
		CatItem& item = inputData[0].getTopResult();
		launchItem(item);
	}
	hideLaunchy();
}


void LaunchyWidget::inputMethodEvent(QInputMethodEvent* event)
{
	event; // Warning removal
	processKey();
}


void LaunchyWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_F5 && (event->modifiers() & ~Qt::ShiftModifier) == 0)
	{
		setSkin(gSettings->value("GenOps/skin", "Default").toString());
		if ((event->modifiers() & Qt::ShiftModifier) == 0)
			buildCatalog();
	}

	else if (event->key() == Qt::Key_Escape)
	{
		if (alternatives->isVisible())
			showAlternatives(false);
		else
			hideLaunchy();
	}

	else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		doEnter();
	}

	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_PageDown ||
		event->key() == Qt::Key_PageUp)
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
					qApp->sendEvent(alternatives, event);
				}
			}
		}
		else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_PageDown)
		{
			if (searchResults.count() == 0)
				searchOnInput();

			dropTimer->stop();
			showAlternatives();
			alternatives->activateWindow();
			if (alternatives->count() > 0)
				alternatives->setCurrentRow(0);
		}
	}

	else if (
		event->key() == Qt::Key_Left ||
		event->key() == Qt::Key_Right ||
		event->key() == Qt::Key_Up ||
		event->key() == Qt::Key_Down ||
		event->key() == Qt::Key_PageUp ||
		event->key() == Qt::Key_PageDown ||
		event->key() == Qt::Key_Control ||
		event->key() == Qt::Key_Shift ||
		event->key() == Qt::Key_Alt ||
		event->key() == Qt::Key_AltGr)
	{
		// Prevent alternatives from being hidden
	}

	else
	{
		if (event->key() == Qt::Key_Tab)
		{
			doTab();
		}
		else if (event->key() == Qt::Key_Slash || event->key() == Qt::Key_Backslash)
		{
			if (inputData.size() > 0 && inputData.last().hasLabel(LABEL_FILE))
				doTab();
		}
		else
		{
			event->ignore();
		}

		processKey();

	}
}


void LaunchyWidget::processKey()
{
	dropTimer->stop();
	int delay = gSettings->value("GenOps/autoSuggestDelay", 1000).toInt();
	if (delay > 0)
		dropTimer->start(delay);

	parseInput(input->text());
	searchOnInput();
	updateDisplay();
}


void LaunchyWidget::searchOnInput()
{
	if (catalog == NULL)
		return;

	QString searchText = inputData.count() > 0 ? inputData.last().getText() : "";
	gSearchTxt = searchText;
	searchResults.clear();

	if (searchText.length() == 0)
		searchHistory(searchText, searchResults);
	else if (inputData.count() == 1)
		catalog->searchCatalogs(gSearchTxt, searchResults);

	if (searchResults.count() != 0)
		inputData.last().setTopResult(searchResults[0]);

	plugins.getLabels(&inputData);
	plugins.getResults(&inputData, &searchResults);
	qSort(searchResults.begin(), searchResults.end(), CatLessNoPtr);

	//	    qDebug() << gSearchTxt;
	// Is it a file?

	if (searchText.contains(QDir::separator()) || searchText.startsWith("~") ||
		(searchText.size() == 2 && searchText[0].isLetter() && searchText[1] == ':'))
		searchFiles(searchText, searchResults);

	catalog->checkHistory(gSearchTxt, searchResults);
}


void LaunchyWidget::updateDisplay()
{
	if (searchResults.count() > 0 && gSearchTxt.length() > 0)
	{
		output->setText(Catalog::decorateText(searchResults[0].shortName, gSearchTxt, true));

		iconExtractor.processIcon(searchResults[0]);

		// Did the plugin take control of the input?
		if (inputData.last().getID() != 0)
			searchResults[0].id = inputData.last().getID();

		inputData.last().setTopResult(searchResults[0]);
	}
	else
	{
		output->clear();
		outputIcon->clear();
	}
}


void LaunchyWidget::iconExtracted(int itemIndex, QIcon icon)
{
	if (itemIndex == -1)
	{
		// An index of -1 means update the output icon
		outputIcon->setPixmap(icon.pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
	}
	else if (itemIndex < alternatives->count())
	{
		// >=0 is an item in the list
		QListWidgetItem* listItem = alternatives->item(itemIndex);
		listItem->setIcon(icon);
		listItem->setData(ROLE_ICON, icon);

		QRect rect = alternatives->visualItemRect(listItem);
		repaint(rect);
	}
}


void LaunchyWidget::searchFiles(const QString& searchText, QList<CatItem>& searchResults)
{
	QString searchPath = QDir::fromNativeSeparators(searchText);

	if (searchPath.startsWith("~"))
		searchPath.replace("~", QDir::homePath());

#ifdef Q_WS_WIN
	if (searchPath == "/")
	{
		// Special case for Windows: list available drives
		QFileInfoList driveList = QDir::drives();
		foreach(QFileInfo info, driveList)
		{
			// Retrieve volume name
			QString volumeName;
			WCHAR volName[MAX_PATH];
			if (GetVolumeInformation((WCHAR*)info.filePath().utf16(), volName, MAX_PATH, NULL, NULL, NULL, NULL, 0))
				volumeName = QString::fromUtf16((const ushort*)volName);
			else
				volumeName = info.filePath();
			CatItem item(QDir::toNativeSeparators(info.filePath()), volumeName);
			searchResults.push_back(item);
		}
		return;
	}
	if (searchPath.size() == 2 && searchText[0].isLetter() && searchPath[1] == ':')
		searchPath += "/";
#endif

	// Network searches are too slow
	if (searchPath.startsWith("//"))
		return;

	// Split the string on the last slash
	QString directoryPart = searchPath.mid(0,searchPath.lastIndexOf("/")+1);
	QString filePart = searchPath.mid(searchPath.lastIndexOf("/")+1);

	QFileInfo info(directoryPart);
	if (!info.isDir())
		return;

	inputData.last().setLabel(LABEL_FILE);

	// Okay, we have a directory, find files that match "file"
	QDir dir(directoryPart);
	QStringList fileList;
	QDir::Filters filters = QDir::Dirs | QDir::Files;
	if (gSettings->value("GenOps/showHiddenFiles", false).toBool())
		filters |= QDir::Hidden;
	
	bool userWildcard = filePart.contains("*") || filePart.contains("?");
	fileList = dir.entryList(QStringList(filePart + "*"), filters, QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware);

	foreach(QString fileName, fileList)
	{
		// don't include self and parent entries
		if (fileName == "." || fileName == "..")
			continue;

		if (userWildcard || fileName.indexOf(filePart, 0, Qt::CaseInsensitive) == 0)
		{
			QString filePath = dir.absolutePath() + "/" + fileName;
			filePath = QDir::cleanPath(filePath);
			if (dir.exists(filePath))
				filePath += "/";

			CatItem item(QDir::toNativeSeparators(filePath), fileName);
			searchResults.push_back(item);
		}
	}

	// Showing a directory
	if (filePart.count() == 0)
	{
		QString fullPath = QDir::toNativeSeparators(directoryPart);
		if (!fullPath.endsWith(QDir::separator()))
			fullPath += QDir::separator();
		QString name = info.dir().dirName();
		CatItem item(fullPath, name.count() == 0 ? fullPath : name);
		searchResults.push_front(item);
	}
}


void LaunchyWidget::searchHistory(const QString& /*searchText*/, QList<CatItem>& searchResults)
{
	foreach(CatItem item, history)
	{
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

	connect(http, SIGNAL(done( bool)), this, SLOT(httpGetFinished(bool)));
	http->setHost("www.launchy.net");
	http->get("http://www.launchy.net/version2.html", verBuffer);
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


void LaunchyWidget::setSkin(const QString& name)
{
	hideLaunchy(true);
	applySkin(name);
	showLaunchy(false);
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

	if (oldVersion < 220)
	{
		gSettings->setValue("GenOps/skin", "Default");
	}

	if (oldVersion < LAUNCHY_VERSION)
	{
		gSettings->setValue("donateTime", QDateTime::currentDateTime().addDays(21));
		gSettings->setValue("version", LAUNCHY_VERSION);
	}
}


void LaunchyWidget::loadPosition()
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

	move(pt);
}


void LaunchyWidget::updateTimeout()
{
	// Save the settings periodically
	savePosition();
	gSettings->sync();

	buildCatalog();
	
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


void LaunchyWidget::onHotkey()
{
	if (menuOpen || optionsOpen)
	{
		showLaunchy(true);
		return;
	}
	if (!alwaysShowLaunchy && isVisible() && !fader->isFading())
	{
		hideLaunchy();
	}
	else
	{
		showLaunchy();
	}
}


void LaunchyWidget::closeEvent(QCloseEvent* event)
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


bool LaunchyWidget::setAlwaysShow(bool alwaysShow)
{
	alwaysShowLaunchy = alwaysShow;
	return !isVisible() && alwaysShow;
}


bool LaunchyWidget::setAlwaysTop(bool alwaysTop)
{
	if (alwaysTop && (windowFlags() & Qt::WindowStaysOnTopHint) == 0)
	{
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		alternatives->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		return true;
	}
	else if (!alwaysTop && (windowFlags() & Qt::WindowStaysOnTopHint) != 0)
	{
		setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
		alternatives->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
		return true;
	}

	return false;
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


void LaunchyWidget::setOpaqueness(int level)
{
	double value = level / 100.0;
	setWindowOpacity(value);
	alternatives->setWindowOpacity(value);
}


void LaunchyWidget::applySkin(const QString& name)
{
	if (listDelegate == NULL)
		return;

	QString directory = dirs["skins"][0] + QString("/") + name + "/";

	// Set a few defaults
	delete frameGraphic;
	frameGraphic = NULL;
	closeButton->setGeometry(QRect());
	optionsButton->setGeometry(QRect());
	input->setAlignment(Qt::AlignLeft);
	output->setAlignment(Qt::AlignCenter);

	// Load the style sheet
	if (QFile::exists(directory + "style.qss"))
	{
		QFile file(directory + "style.qss");
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QString styleSheet = QLatin1String(file.readAll());
			// This is causing the ::destroyed connect errors
			qApp->setStyleSheet(styleSheet);

			file.close();
		}
	}
	alternativesRect = QRect();

	// Use default skin if this one doesn't exist
	if (!QFile::exists(directory + "misc.txt")) 
	{
		directory = dirs["defSkin"][0];
		gSettings->setValue("GenOps/skin", "Default");
	}

	// Set positions
	if (QFile::exists(directory + "misc.txt"))
	{
		QFile file(directory + "misc.txt");
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
						alternativesRect = rect;
					else if (spl.at(0).trimmed().compare("boundary", Qt::CaseInsensitive) == 0)
						resize(rect.size());
					else if (spl.at(0).trimmed().compare("icon", Qt::CaseInsensitive) == 0)
						outputIcon->setGeometry(rect);
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

	bool validFrame = false;
	QPixmap frame;

	if (platform->SupportsAlphaBorder())
	{
		if (frame.load(directory + "frame.png"))
		{
			validFrame = true;
		}
		else if (frame.load(directory + "background.png"))
		{
			QPixmap border;
			if (border.load(directory + "mask.png"))
			{
				frame.setMask(border);
			}
			if (border.load(directory + "alpha.png"))
			{
				QPainter surface(&frame);
				surface.drawPixmap(0,0, border);
			}
			validFrame = true;
		}
	}
	
	if (!validFrame)
	{
		// Set the background image
		if (frame.load(directory + "background_nc.png"))
		{
			validFrame = true;

			// Set the background mask
			QPixmap mask;
			if (mask.load(directory + "mask_nc.png"))
			{
				// For some reason, w/ compiz setmask won't work
				// for rectangular areas.  This is due to compiz and
				// XShapeCombineMask
				setMask(mask);
			}
		}
	}

	if (validFrame)
	{
		QPixmap* newFrame = new QPixmap(frame);
		if (newFrame)
		{
			delete frameGraphic;
			frameGraphic = newFrame;
			resize(frameGraphic->size());
		}
	}
}


void LaunchyWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->buttons() == Qt::LeftButton)
	{
		dragging = true;
		dragStartPoint = e->pos();
	}
	showAlternatives(false);
	activateWindow();
	input->setFocus();
}


void LaunchyWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() == Qt::LeftButton && dragging)
	{
		QPoint p = e->globalPos() - dragStartPoint;
		move(p);
		showAlternatives(false);
		input->setFocus();
	}
}


void LaunchyWidget::mouseReleaseEvent(QMouseEvent* event)
{
	event;
	dragging = false;
	showAlternatives(false);
	input->setFocus();
}


void LaunchyWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	menu.addAction(actRebuild);
	menu.addAction(actOptions);
	menu.addSeparator();
	menu.addAction(actExit);
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


void LaunchyWidget::showOptionsDialog()
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


void LaunchyWidget::setFadeLevel(double level)
{
	level = min(level, 1);
	level = max(level, 0);
	setWindowOpacity(level);
	alternatives->setWindowOpacity(level);
	if (level <= 0)
	{
		hide();
	}
	else
	{
		if (!isVisible())
			show();
	}
}


void LaunchyWidget::showLaunchy(bool noFade)
{
	shouldDonate();
	showAlternatives(false);

#ifdef Q_WS_WIN
	// There's a problem with alpha layered windows under Vista after resuming
	// from sleep. The alpha image may need to be reapplied.
#endif
	loadPosition();

	fader->fadeIn(noFade || alwaysShowLaunchy);

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
	input->selectAll();
	input->setFocus();
	qApp->syncX();
	// Let the plugins know
	plugins.showLaunchy();
}


void LaunchyWidget::showLaunchy()
{
	showLaunchy(false);
}


void LaunchyWidget::hideLaunchy(bool noFade)
{
	if (!isVisible())
		return;

	savePosition();
	if (dropTimer != NULL && dropTimer->isActive())
		dropTimer->stop();
	if (alwaysShowLaunchy)
		return;

	showAlternatives(false);

	if (isVisible())
	{
		fader->fadeOut(noFade);
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

	showTrayIcon();
}


void LaunchyWidget::createActions()
{
	actShow = new QAction(tr("Show Launchy"), this);
	connect(actShow, SIGNAL(triggered()), this, SLOT(showLaunchy()));

	actRebuild = new QAction(tr("Rebuild catalog"), this);
	connect(actRebuild, SIGNAL(triggered()), this, SLOT(buildCatalog()));

	actOptions = new QAction(tr("Options"), this);
	connect(actOptions, SIGNAL(triggered()), this, SLOT(menuOptions()));

	actExit = new QAction(tr("Exit"), this);
	connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
}


void LaunchyWidget::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		showLaunchy();
		break;
	case QSystemTrayIcon::MiddleClick:
		buildCatalog();
		break;
	default:
		break;
	}
}


int main(int argc, char *argv[])
{
	PlatformBase * platform = loadPlatform();
	shared_ptr<QApplication> app(platform->init(argc, argv));

	QStringList args = qApp->arguments();
	app->setQuitOnLastWindowClosed(false);
	StartModes startMode = Normal;
	bool allowMultipleInstances = false;

	if (args.size() > 1)
	{
		foreach (QString arg, args)
		{
			if (arg.compare("/rescue", Qt::CaseInsensitive) == 0)
			{
				// Kill all existing Launchys
				//			platform->KillLaunchys();
				startMode = ShowOptions;
			}
			else if (arg.compare("/show", Qt::CaseInsensitive) == 0)
			{
				startMode = ShowLaunchy;
			}
			else if (arg.compare("/options", Qt::CaseInsensitive) == 0)
			{
				startMode = ShowOptions;
			}
			else if (arg.compare("/multiple", Qt::CaseInsensitive) == 0)
			{
				allowMultipleInstances = true;
			}
		}
	}

	if (!allowMultipleInstances && platform->isAlreadyRunning())
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

	LaunchyWidget widget(NULL, platform, startMode);

	app->exec();
}
