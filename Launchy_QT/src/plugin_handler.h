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

#ifndef PLUGIN_HANDLER
#define PLUGIN_HANDLER

#include <QPluginLoader>
#include <QHash>
#include "plugin_interface.h"
#include "catalog.h"
#include <QLayout>

struct PluginInfo {
	uint id;
	QString name;
	QString path;
	PluginInterface* obj;
	bool loaded;

	~PluginInfo() {
		QPluginLoader loader(path);
		loader.unload();
	}
};


class PluginHandler {
private:
	QHash<uint, PluginInfo> plugins;

public:

	PluginHandler();
	~PluginHandler();

	void loadPlugins();
	void getLabels(QList<InputData>* id);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalogs(QList<CatItem>* items);
	void execute(QList<InputData>*, CatItem*);
	QWidget* doDialog(QWidget* parent, uint id);
	void endDialog(uint id, bool accept);
	QHash<uint, PluginInfo> & getPlugins() { return plugins; }
};



#endif