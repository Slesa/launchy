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

#ifndef MAIN_H
#define MAIN_H


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QString>
#include <QtNetwork/QHttp>
#include <QBuffer>
#include <QKeyEvent>
#include <QScrollBar>
#include "plugin_handler.h"
#include "platform_util.h"
#include "catalog.h"
#include "catalog_builder.h"
#include "icon_delegate.h"
#include "icon_extractor.h"
#include "globals.h"
#include <boost/shared_ptr.hpp>
#include "CharLineEdit.h"
#include "LineEditMenu.h"
#include "CharListWidget.h"
#include "Fader.h"


using namespace boost;


class LaunchyWidget : public QWidget
{
	Q_OBJECT  // Enable signals and slots

public:
	LaunchyWidget();
    LaunchyWidget(QWidget *parent, PlatformBase*, bool show);
	~LaunchyWidget();

	QHash<QString, QList<QString> > dirs;
	Fader* fader;
	QPoint moveStartPoint;
	shared_ptr<PlatformBase> platform;	
	QLabel* label;
	LineEditMenu *output;
	CharLineEdit *input;
	QTimer* updateTimer;
	QTimer* dropTimer;
	CharListWidget *alternatives;
	QPushButton *optionsButton;
	QPushButton *closeButton;
	QRect altRect;
	QLabel* labelIcon;
	QScrollBar* altScroll;
	shared_ptr<Catalog> catalog;
	shared_ptr<CatalogBuilder> catalogBuilder;
	IconExtractor iconExtractor;
	QList<CatItem> searchResults;
	QList<InputData> inputData;
	QStringList history;
	PluginHandler plugins;
	bool visible;
	bool alwaysShowLaunchy;
	bool menuOpen;
	bool optionsOpen;

	IconDelegate* listDelegate;
	QAbstractItemDelegate * defaultDelegate;

	void connectAlpha();

	void applySkin(QString);
	void contextMenuEvent(QContextMenuEvent *event);
	void closeEvent(QCloseEvent *event);
	void showLaunchy(bool now = false);
	void hideLaunchy(bool now = false);
	void updateVersion(int oldVersion);
	void checkForUpdate();
	void shouldDonate();
	void setCondensed(int condensed);
	bool setHotkey(int, int);
	void showAlternatives(bool show = true);
	void launchObject();
	void searchFiles(const QString & input, QList<CatItem>& searchResults);
	void parseInput(QString text);
	void resetLaunchy();
	void updateDisplay();
	void searchOnInput();
	void fadeIn();
	void fadeOut();
	QPoint loadPosition();
	void savePosition() { gSettings->setValue("Display/pos", pos()); }
	void doTab();
	void doEnter();
	QString printInput();
	void processKey();

	void loadOptions();

protected:
	bool event(QEvent *e);

private:
    QHttp *http;
    QBuffer *verBuffer;
	QBuffer *counterBuffer;

public slots:
	void menuOptions();
	void onHotKey();
	void updateTimeout();
	void dropTimeout();
	void setAlwaysShow(bool);
	void setAlwaysTop(bool);
	void setPortable(bool);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void setSkin(QString, QString);
	void httpGetFinished(bool result);
	void catalogBuilt();
	void inputMethodEvent(QInputMethodEvent* e);
	void keyPressEvent(QKeyEvent*);
	void inputKeyPressEvent(QKeyEvent* key);
	void altKeyPressEvent(QKeyEvent* key);
	void focusOutEvent(QFocusEvent* evt);
	void setOpaqueness(int val);
	void setFadeLevel(double);
	void finishedFade(double d);
	void menuEvent(QContextMenuEvent*);
	void buildCatalog();
	void iconExtracted(int itemIndex, QIcon icon);
};


#endif
