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

#ifndef controly_H
#define controly_H

#include "plugin_interface.h"

#ifdef WITH_GUI
	#include "gui.h"
#endif

#include "../fhocommon/fhoicon.h"

class controlyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

private:
	uint HASH_controly;

	FhoIconCreator iconCreator;

	#ifdef WITH_GUI
		Gui* gui;
	#endif

public:
	controlyPlugin();
	~controlyPlugin();

	int msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

private:
	void getID(uint*);
	void getName(QString*);
	void getCatalog(QList<CatItem>*);
	void init();
	QString getIcon();
	void getApps(QList<CatItem>* items);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	int launchItem(QList<InputData>* id, CatItem* item);

	#ifdef WITH_GUI
		void doDialog(QWidget* parent, QWidget**);
		void endDialog(bool accept);
	#endif

};

extern controlyPlugin* gControlyInstance;

#endif