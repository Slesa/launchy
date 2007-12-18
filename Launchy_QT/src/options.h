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
#include <QString>
#include <QStringList>
#include <QList>

struct Directory {
	Directory() 
		: indexDirs(false), indexExe(false), depth(100)
	{
	
	}
	bool indexDirs;
	bool indexExe;
	QString name;
	QStringList types;
	int depth;
};

class OptionsDlg : public QDialog, private Ui::OptionsDlg
{
  Q_OBJECT

private:
	int curPlugin;

public:

QStringList metaKeys;
QStringList actionKeys;
QList<int> iMetaKeys;
QList<int> iActionKeys;
OptionsDlg(QWidget* parent = 0);
~OptionsDlg();
QList<Directory> memDirs;
QList<QPair<QString, uint> > pluginNames;
QVBoxLayout* pluginLayout;

public slots:
	void accept();
	void reject();
	void skinChanged(const QString);
	void dirChanged(int row);
	void catDirPlusClicked(bool c);
	void catDirMinusClicked(bool c);
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
	void catDirTextChanged(QListWidgetItem * item );
};


#endif
