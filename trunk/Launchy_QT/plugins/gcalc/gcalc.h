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

#ifndef gcalc_H
#define gcalc_H

#include <QtGui>
#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QHttp>
#include <QEventLoop>
#include <QBuffer>
#include <QString>
#include "plugin_interface.h"


class Process : public QObject {
	Q_OBJECT
public:
	QString query;
	QHttp http;
	QBuffer resBuffer;
	QString result;
	QEventLoop loop; 
	Process() {}
	Process(QString str) : query(str) {}
	void run();
public slots:
	void httpGetFinished(bool error);
};

class gcalcPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
	QRegExp* reg;

public:
	uint HASH_gcalc;

public:
	gcalcPlugin() {
		HASH_gcalc = qHash(QString("gcalc"));
	}
	~gcalcPlugin() {delete reg;}
	int msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalog(QList<CatItem>* items);
	void init();
	QString getIcon();

public slots:

};




#endif
