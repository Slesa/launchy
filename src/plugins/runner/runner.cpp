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

#include <QString>
#include "winfiles.h"
#include "runner.h"
#include "gui.h"
#include <QUrl>
#include <QtPlugin>

RunnerPlugin::RunnerPlugin()
{
    gui.reset();
    HASH_runner = qHash(QString("runner"));
}

void RunnerPlugin::init()
{
	cmds.clear();

    if ( _settings->value("runner/version", 0.0).toDouble() == 0.0 )
	{
        _settings->beginWriteArray("runner/cmds");
        _settings->setArrayIndex(0);
        #ifdef Q_OS_WIN
        _settings->setValue("name", "cmd");
        _settings->setValue("file", "C:\\Windows\\System32\\cmd.exe");
        _settings->setValue("args", "/K $$");
		#endif
        #ifdef Q_OS_LINUX
        _settings->setValue("name", "cmd");
        _settings->setValue("file", "/usr/bin/xterm");
        _settings->setValue("args", "-hold -e $$");
		#endif
        #ifdef Q_OS_BSD4
        _settings->setValue("name", "cmd");
        _settings->setValue("file", "/usr/local/bin/xterm");
        _settings->setValue("args", "-hold -e $$");
        #endif
                /*
                #ifdef Q_OS_MAC
                set->setValue("name", "cmd");
                set->setValue("file", "")
                #endif
                */
        _settings->endArray();
	}
    _settings->setValue("runner/version", 2.0);

	// Read in the array of websites
    int count = _settings->beginReadArray("runner/cmds");
	for(int i = 0; i < count; ++i)
	{
        _settings->setArrayIndex(i);
		runnerCmd cmd;
        cmd.file = _settings->value("file").toString();
        cmd.name = _settings->value("name").toString();
        cmd.args = _settings->value("args").toString();
		cmds.push_back(cmd);
	}
    _settings->endArray();
}


void RunnerPlugin::getID(uint* id)
{
	*id = HASH_runner;
}


void RunnerPlugin::getName(QString* str)
{
	*str = "Runner";
}


QString RunnerPlugin::getIcon()
{
    return libPath + "/icons/runner.png";
}


QString RunnerPlugin::getIcon(QString file)
{
    file = file; // Warning removal
#ifdef Q_OS_WIN
	QRegExp rx("\\.(exe|lnk)$", Qt::CaseInsensitive);
    if (rx.indexIn(file) != -1)
		return file;
#endif
	return getIcon();
}


void RunnerPlugin::getCatalog(QList<CatItem>* items)
{
	foreach(runnerCmd cmd, cmds)
	{
		items->push_back(CatItem(cmd.file + "%%%" + cmd.args, cmd.name, HASH_runner, getIcon(cmd.file)));
	}
}


void RunnerPlugin::getResults(QList<InputData>* inputData, QList<CatItem>* results)
{
    if (inputData->count() <= 1)
		return;

	CatItem& catItem = inputData->first().getTopResult();
        if (catItem.id == (int) HASH_runner && inputData->last().hasText())
	{
		const QString & text = inputData->last().getText();
		// This is user search text, create an entry for it
		results->push_front(CatItem(text, text, HASH_runner, getIcon(catItem.icon)));
	}
}


void RunnerPlugin::launchItem(QList<InputData>* inputData, CatItem* item)
{
    Q_UNUSED(item) // Compiler warning

	QString file = "";
	QString args = "";

	CatItem* base = &inputData->first().getTopResult();

	file = base->fullPath;

	// Replace the $'s with arguments
	QStringList spl = file.split("$$");

	file = spl[0];
	for (int i = 1; i < spl.size(); ++i)
	{
		if (inputData->count() >= i+1)
		{ 
//			const InputData* ij = &inputData->at(i);
			CatItem* it = &((InputData)inputData->at(i)).getTopResult();
			file += it->fullPath;
		}
		file += spl[i];
	}

	// Split the command from the arguments
	spl = file.split("%%%");

	file = spl[0];
	if (spl.count() > 0)
		args = spl[1];

	if (file.contains("http://"))
	{
		QUrl url(file);
		file = url.toEncoded();
	}
	//	qDebug() << file << args;
	runProgram(file, args);
}


void RunnerPlugin::doDialog(QWidget* parent, QWidget** newDlg)
{
	if (gui != NULL)
		return;
    gui.reset(new Gui(parent, _settings));
    *newDlg = gui.data();
}


void RunnerPlugin::endDialog(bool accept)
{
	if (accept)
	{
		gui->writeOptions();
		init();
	}
	gui.reset();
}

void RunnerPlugin::setPath(QString * path)
{
    libPath = *path;
}


int RunnerPlugin::msg(int msgId, void* wParam, void* lParam)
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
		endDialog(wParam != 0);
		break;
	case MSG_PATH:
	    setPath((QString*) wParam);

	default:
		break;
	}
		
	return handled;
}
