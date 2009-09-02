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

#ifndef OPTIONS_H
#define OPTIONS_H

#include "ui_options.h"
#include "FileBrowserDelegate.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QDragEnterEvent>
#include "Directory.h"


class OptionsDialog : public QDialog, private Ui::OptionsDlg
{
  Q_OBJECT

public:
	OptionsDialog(QWidget* parent = 0);
	~OptionsDialog();

	QStringList metaKeys;
	QStringList actionKeys;
	QList<int> iMetaKeys;
	QList<int> iActionKeys;
	QList<Directory> memDirs;
	QList<QPair<QString, uint> > pluginNames;
	QVBoxLayout* pluginLayout;
	QString lastDir;

public slots:
	void accept();
	void reject();
	void skinChanged(const QString);
	void dirRowChanged(int row);
	void catDirItemChanged(QListWidgetItem* item);
	void catDirDragEnter(QDragEnterEvent *event);
	void catDirDrop(QDropEvent *event);
	void catDirPlusClicked(bool c);
	void catDirMinusClicked(bool c);
	void catTypesItemChanged(QListWidgetItem* item);
	void catTypesPlusClicked(bool c);
	void catTypesMinusClicked(bool c);
	void catTypesDirChanged(int);
	void catTypesExeChanged(int);
	void catDepthChanged(int);
	void catProgressUpdated(float);
	void catalogBuilt();
	void catRescanClicked(bool);
	void pluginChanged(int row);
	void pluginItemChanged(QListWidgetItem* state);
	void tabChanged(int tab);

private:
	void addDirectory(const QString& directory);

private:
	FileBrowserDelegate directoryItemDelegate;
	int curPlugin;

	static QByteArray windowGeometry;
	static int currentTab;
	static int currentPlugin;
};


#endif
