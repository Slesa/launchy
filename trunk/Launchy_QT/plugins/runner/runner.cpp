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

#include <QtGui>
#include <QUrl>
#include <QFile>
#include <QRegExp>

#ifdef Q_WS_WIN
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>



#endif

#include "runner.h"
#include "gui.h"

runnerPlugin* grunnerInstance = NULL;

void runnerPlugin::init()
{
	if (grunnerInstance == NULL)
		grunnerInstance = this;

	QSettings* set = *settings;
	cmds.clear();

	if ( set->value("runner/version", 0.0).toDouble() == 0.0 ) {
		set->beginWriteArray("runner/cmds");
		set->setArrayIndex(0);
		#ifdef Q_WS_WIN
		set->setValue("name", "cmd");
		set->setValue("file", "C:\\Windows\\System32\\cmd.exe");
		set->setValue("args", "/K $$");
		#endif
		#ifdef Q_WS_X11
		set->setValue("name", "cmd");
		set->setValue("file", "/usr/bin/xterm");
		set->setValue("args", "-hold -e $$");
		#endif
		set->endArray();
	}
	set->setValue("runner/version", 2.0);


	// Read in the array of websites
	int count = set->beginReadArray("runner/cmds");
	for(int i = 0; i < count; ++i) {
		set->setArrayIndex(i);
		runnerCmd cmd;
		cmd.file = set->value("file").toString();
		cmd.name = set->value("name").toString();
		cmd.args = set->value("args").toString();
		cmds.push_back(cmd);
	}
	set->endArray();
}

void runnerPlugin::getID(uint* id)
{
	*id = HASH_runner;
}

void runnerPlugin::getName(QString* str)
{
	*str = "Runner";
}


QString runnerPlugin::getIcon()
{
    return libPath + "/icons/runner.png";
}

void runnerPlugin::getCatalog(QList<CatItem>* items)
{
	foreach(runnerCmd cmd, cmds) {
		items->push_back(CatItem(cmd.file + "%%%" + cmd.args, cmd.name, HASH_runner, getIcon()));
	}
}


void runnerPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
    if ( id->count() > 1 && (unsigned int)id->first().getTopResult().id == HASH_runner) {
		const QString & text = id->last().getText();
		// This is user search text, create an entry for it
		results->push_front(CatItem(text, text, HASH_runner, getIcon()));
	}
}

void runnerPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	item = item; // Compiler warning

	QString file = "";
	QString args = "";

	CatItem* base = &id->first().getTopResult();

	file = base->fullPath;

	// Replace the $'s with arguments
	QStringList spl = file.split("$$");

	file = spl[0];
	for(int i = 1; i < spl.size(); ++i) {
		if (id->count() >= i+1) { 
//			const InputData* ij = &id->at(i);
			CatItem* it = &((InputData)id->at(i)).getTopResult();
			file += it->fullPath;
		}
		file += spl[i];
	}

	// Split the command from the arguments
	spl = file.split("%%%");

	file = spl[0];
	if (spl.count() > 0)
		args = spl[1];



	if (file.contains("http://")) {
		QUrl url(file);
		file = url.toEncoded();
	}
	//	qDebug() << file << args;
	runProgram(file, args);
}

void runnerPlugin::doDialog(QWidget* parent, QWidget** newDlg) {
	if (gui != NULL) return;
	gui.reset(new Gui(parent));
//	gui = new Gui(parent);
	*newDlg = gui.get();
}

void runnerPlugin::endDialog(bool accept) {
	if (accept) {
		gui->writeOptions();
		init();
	}
/*
	if (gui != NULL) 
		delete gui;
	
	gui = NULL;
	*/
	gui.reset();
}

void runnerPlugin::setPath(QString * path) {
    libPath = *path;
}

int runnerPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = true;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = true;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;
		case MSG_LAUNCH_ITEM:
			launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
			handled = true;
			break;
		case MSG_HAS_DIALOG:
			handled = true;
			break;
		case MSG_DO_DIALOG:
			doDialog((QWidget*) wParam, (QWidget**) lParam);
			break;
		case MSG_END_DIALOG:
			endDialog((bool) wParam);
			break;
	case MSG_PATH:
	    setPath((QString*) wParam);

		default:
			break;
	}
		
	return handled;
}

Q_EXPORT_PLUGIN2(runner, runnerPlugin) 
