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

#include "precompiled.h"
#include "Verby.h"
#include "gui.h"


void VerbyPlugin::init()
{
}


void VerbyPlugin::getID(uint* id)
{
	*id = HASH_VERBY;
}


void VerbyPlugin::getName(QString* str)
{
	*str = "Verby";
}


QString VerbyPlugin::getIcon()
{
	return getIconPath() + "verby.png";
}


void VerbyPlugin::getLabels(QList<InputData>* inputData)
{
        QString  text = inputData->last().getText();

	// Is it a file?
	if (text.contains("\\") || text.contains("/"))
		return;

	QDir qd;
	QFile qf;

	QString path = inputData->last().getTopResult().fullPath;
	QFileInfo info(path);

	if (info.isSymLink())
	{
		inputData->last().setLabel(HASH_LINK);
	}
	else if (qd.exists(path))
	{
		inputData->last().setLabel(HASH_DIR);
	}
	else if (qf.exists(path)) 
	{
		inputData->last().setLabel(HASH_FILE);
	}
}


QString VerbyPlugin::getIconPath() const
{
        return libPath + "/icons/";
}


void VerbyPlugin::setPath(QString * path)
{
        libPath = *path;
}

void VerbyPlugin::getResults(QList<InputData>* inputData, QList<CatItem>* results)
{
	if (inputData->count() == 2)
	{
		if (inputData->first().hasLabel(HASH_DIR))
		{
			results->push_back(CatItem("Properties", "Directory properties", HASH_VERBY, getIconPath() + "properties.png"));
		}
		else if (inputData->first().hasLabel(HASH_FILE))
		{
			results->push_back(CatItem("Open containing folder", "Open containing folder", HASH_VERBY, getIconPath() + "opencontainer.png"));
			results->push_back(CatItem("Properties", "File properties", HASH_VERBY, getIconPath() + "properties.png"));
		}
		else if (inputData->first().hasLabel(HASH_LINK))
		{
			results->push_back(CatItem("Run", "Run", HASH_VERBY, getIconPath() + "run.png"));
			results->push_back(CatItem("Run as", "Run as a different user", HASH_VERBY, getIconPath() + "run.png"));
			results->push_back(CatItem("Open containing folder", "Open containing folder", HASH_VERBY, getIconPath() + "opencontainer.png"));
			results->push_back(CatItem("Copy path", "Copy path to clipboard", HASH_VERBY, getIconPath() + "copy.png"));
			results->push_back(CatItem("Properties", "File properties", HASH_VERBY, getIconPath() + "properties.png"));
			inputData->first().setID(HASH_VERBY);
			inputData->first().getTopResult().id = HASH_VERBY;
		}
		else
		{
			return;
		}

		if (inputData->last().hasText())
			results->push_front(CatItem(inputData->last().getText(), inputData->last().getText(), 0, inputData->first().getTopResult().icon));
	}
}


void VerbyPlugin::launchItem(QList<InputData>* inputData, CatItem* item)
{
	item = item; // Compiler Warning

	if (inputData->count() > 2)
		return;

	QString noun = inputData->first().getTopResult().fullPath;
	QString verb;
	if (inputData->count() > 1)
		verb = inputData->last().getTopResult().shortName;

	if (verb == "Run")
	{
		runProgram(noun, "");
	}
	else if (verb == "Open containing folder")
	{
		QFileInfo info(noun);
		if (info.isSymLink())
		{
			info.setFile(info.symLinkTarget());
		}

#ifdef Q_WS_WIN
		runProgram("explorer.exe", "\"" + QDir::toNativeSeparators(info.absolutePath()) + "\"");
#endif
	}
	else if (verb == "Run as")
	{

#ifdef Q_WS_WIN
            SHELLEXECUTEINFO shellExecInfo;

		shellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shellExecInfo.fMask = SEE_MASK_FLAG_NO_UI;
		shellExecInfo.hwnd = NULL;
		shellExecInfo.lpVerb = L"runas";
		shellExecInfo.lpFile = (LPCTSTR)noun.utf16();
		shellExecInfo.lpParameters = NULL;
		QDir dir(noun);
		QFileInfo info(noun);
		if (!info.isDir() && info.isFile())
			dir.cdUp();
		QString filePath = QDir::toNativeSeparators(dir.absolutePath());
		shellExecInfo.lpDirectory = (LPCTSTR)filePath.utf16();
		shellExecInfo.nShow = SW_NORMAL;
		shellExecInfo.hInstApp = NULL;

		ShellExecuteEx(&shellExecInfo);
#endif
	}
	else if (verb == "File properties")
	{
#ifdef Q_WS_WIN
		SHELLEXECUTEINFO shellExecInfo;

		shellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		shellExecInfo.fMask = SEE_MASK_FLAG_NO_UI;
		shellExecInfo.hwnd = NULL;
		shellExecInfo.lpVerb = L"properties";
		QString filePath = QDir::toNativeSeparators(noun);
		shellExecInfo.lpFile = (LPCTSTR)filePath.utf16();
		shellExecInfo.lpParameters = NULL;
		shellExecInfo.lpDirectory = NULL;
		shellExecInfo.nShow = SW_NORMAL;
		shellExecInfo.hInstApp = NULL;

		ShellExecuteEx(&shellExecInfo);
#endif
	}
	else if (verb == "Copy path to clipboard")
	{
		QFileInfo info(noun);
		if (info.isSymLink())
		{
			info.setFile(info.symLinkTarget());
		}
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(QDir::toNativeSeparators(info.canonicalFilePath()));
	}
	else
	{
		// Standard open routine
		QString args = "";
		if (inputData->count() > 1)
			for(int i = 1; i < inputData->count(); ++i)
				args += ((InputData)inputData->at(i)).getText() + " ";
		runProgram(noun, args);
	}
}


void VerbyPlugin::doDialog(QWidget* parent, QWidget** newDlg)
{
	if (gui == NULL)
	{
		gui = new Gui(parent);
		*newDlg = gui;
	}
}


void VerbyPlugin::endDialog(bool accept)
{
	if (accept)
	{
		gui->writeOptions();
		init();
	}
	if (gui != NULL) 
		delete gui;

	gui = NULL;
}


int VerbyPlugin::msg(int msgId, void* wParam, void* lParam)
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
	case MSG_GET_LABELS:
		getLabels((QList<InputData>*) wParam);
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

	default:
		break;
	}

	return handled;
}


Q_EXPORT_PLUGIN2(Verby, VerbyPlugin)
