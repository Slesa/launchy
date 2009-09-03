/*
Verby: Plugin for Launchy
Copyright (C) 2009  Simon Capewell

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

#ifndef VERBY_H
#define VERBY_H

#include "gui.h"
#include "plugin_interface.h"


class VerbyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

public:
	VerbyPlugin()
	{
		gui = NULL;
		HASH_VERBY = qHash(QString("verby"));
		HASH_DIR = qHash(QString("directory"));
		HASH_FILE = qHash(QString("file"));
		HASH_LINK = qHash(QString("link"));
	}

	~VerbyPlugin()
	{
	}

	int msg(int msgId, void* wParam, void* lParam); 

private:
	void getID(uint* id);
	void getName(QString* name);
	void getCatalog(QList<CatItem>* items);
	void getResults(QList<InputData>* inputData, QList<CatItem>* results);
	void launchItem(QList<InputData>* inputData, CatItem* item);
	void doDialog(QWidget* parent, QWidget**);
	void endDialog(bool accept);
	void getLabels(QList<InputData>* inputData);
	void init();
	QString getIcon();
	QString getIconPath() const;

private:
	Gui* gui;
	uint HASH_VERBY;
	uint HASH_FILE;
	uint HASH_DIR;
	uint HASH_LINK;
};

extern VerbyPlugin* verbyInstance;


#endif
