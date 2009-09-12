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
	shared_ptr<PlatformBase> platform;	
	shared_ptr<Catalog> catalog;
	PluginHandler plugins;

	void showLaunchy(bool noFade = false);

	void setCondensed(int condensed);
	bool setHotkey(int, int);
	bool setAlwaysShow(bool);
	bool setAlwaysTop(bool);
	void setPortable(bool);
	void setSkin(const QString& name);
	void loadOptions();

protected:
	bool event(QEvent* event);
    void paintEvent(QPaintEvent* event);

public slots:
    void focusInEvent(QFocusEvent* event);
	void focusOutEvent(QFocusEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void menuOptions();
	void contextMenuEvent(QContextMenuEvent* event);
	void onHotKey();
	void updateTimeout();
	void dropTimeout();
	void setOpaqueness(int level);
	void httpGetFinished(bool result);
	void catalogBuilt();
	void inputMethodEvent(QInputMethodEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void inputKeyPressEvent(QKeyEvent* event);
	void alternativesKeyPressEvent(QKeyEvent* event);
	void setFadeLevel(double level);
	void buildCatalog();
	void iconExtracted(int itemIndex, QIcon icon);

private:
	void connectAlpha();
	void applySkin(const QString& name);
	void closeEvent(QCloseEvent* event);
	void hideLaunchy(bool noFade = false);
	void updateVersion(int oldVersion);
	void checkForUpdate();
	void shouldDonate();
	void showAlternatives(bool show = true);
	void searchFiles(const QString& searchText, QList<CatItem>& searchResults);
	void searchHistory(const QString& searchText, QList<CatItem>& searchResults);
	void parseInput(const QString& text);
	void resetLaunchy();
	void updateDisplay();
	void searchOnInput();
	void loadPosition();
	void savePosition() { gSettings->setValue("Display/pos", pos()); }
	void doTab();
	void doEnter();
	QString formatInput();
	void processKey();
	void launchItem(CatItem& item);
	void addToHistory(const CatItem& catalogItem);

	Fader* fader;
	QPixmap* frameGraphic;
	CharLineEdit* input;
	QLabel* output;
	QLabel* outputIcon;
	CharListWidget* alternatives;
	QRect alternativesRect;
	QPushButton* optionsButton;
	QPushButton* closeButton;
	QScrollBar* altScroll;
	QTimer* updateTimer;
	QTimer* dropTimer;
	shared_ptr<CatalogBuilder> catalogBuilder;
	IconExtractor iconExtractor;
	QList<CatItem> searchResults;
	QList<InputData> inputData;
	QList<CatItem> history;
	bool alwaysShowLaunchy;
	bool dragging;
	QPoint dragStartPoint;
	bool menuOpen;
	bool optionsOpen;

	IconDelegate* listDelegate;
	QAbstractItemDelegate* defaultListDelegate;

	QHttp *http;
	QBuffer *verBuffer;
	QBuffer *counterBuffer;

};


#endif
