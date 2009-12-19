/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin

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
#include "controly.h"
#include "ControlPanelItemFinder.h"

controlyPlugin* gControlyInstance = NULL;

controlyPlugin::controlyPlugin() {
	HASH_controly = qHash(QString("controly"));
	gui = NULL;
}

controlyPlugin::~controlyPlugin() {
}

void controlyPlugin::init() {
	if (gControlyInstance == NULL) {
		// init() is currently called multiple times:
		// on plugin init and every time when opening the options

		gControlyInstance = this;

		// get config / settings directory (base for 'temporary' icon cache dir)
		QString iniFilename = (*settings)->fileName();
		QFileInfo info(iniFilename);
		QString userDataPath = info.absolutePath();
		
		// create (and store) controly icon cache folder
		iconCreator.setImagePath(userDataPath + "/controly-icon-cache");
		iconCreator.setBaseImageFileName("controly-icon-");
		iconCreator.setDefaultImageFileName(getIcon());
		iconCreator.setForceDefaultImage(false);
	}
}

void controlyPlugin::getID(uint* id) {
	*id = HASH_controly;
}

void controlyPlugin::getName(QString* str) {
	*str = "Controly";
}

QString controlyPlugin::getIcon() {
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/plugins/icons/controly.png";
#endif
}

#ifdef Q_WS_WIN
void controlyPlugin::getApps(QList<CatItem>* items) {
	ControlPanelItemFinder *pCplFinder = new ControlPanelItemFinder(HASH_controly, &iconCreator, items);
	pCplFinder->findItems();

	if (pCplFinder) {
		delete pCplFinder;
		pCplFinder = NULL;
	}
}
#endif

void controlyPlugin::getCatalog(QList<CatItem>* items) {
	getApps(items);

	CatItem tmp = CatItem("Launchy.controly", "Launchy", HASH_controly, getIcon());
	tmp.usage = 5000;
	items->push_back(tmp);
}

void controlyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	// if user enters "*.controly", dynamically return all elements that we added to the primary catalog (for informational / debugging purposes only)
	if (id->size() == 1) {
		const QString & text = id->first().getText();

		if (text.compare("*.controly") == 0) {
			QList<CatItem> controlyCatalog;
			getCatalog(&controlyCatalog);
			CatItem temp("ItemCount.controly", QString::number(controlyCatalog.size()), HASH_controly, getIcon());
			temp.usage = 32000;
			results->append(temp);
			(*results) += controlyCatalog;
		}
	}

	if (id->count() != 2)
		return;

	if (id->first().getTopResult().id == HASH_controly) {
		QSettings* set = *settings;
		if (set == NULL) return;

		CatItem tmp = CatItem("Launchy.options", "Options", HASH_controly, getIcon());
		tmp.usage = set->value("controly/OptionsCount",0).toInt();
		results->push_back(tmp);

		tmp = CatItem("Launchy.reindex", "Rebuild Index", HASH_controly, getIcon());
		tmp.usage = set->value("controly/RebuildCount",0).toInt();
		results->push_back(tmp);

		tmp = CatItem("Launchy.exit", "Exit", HASH_controly, getIcon());
		tmp.usage = set->value("controly/ExitCount",0).toInt();
		results->push_back(tmp);
	}	
}

int controlyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	item = item; // Compiler warning

	if (id->count() == 1) {
		// no parameters, just the item itsef

		QString path = item->fullPath;

		if (path.contains(",@")) {
			// dll cpl item indexing containing items, e.g. 'main.cpl,@1'

			runProgram("control.exe", item->fullPath); //runProgram(cmd, args);
			// use toNativeSeparators()?
		} else if ((path.startsWith("csidl:", Qt::CaseInsensitive)) && (path.endsWith(".controly", Qt::CaseSensitive))) {
			// Constant special item ID list (CSIDL)

			// shell instance object (special shell extension folder), e.g. 'csidl:0x0014.controly' ('shellinstance:0x0014')
			QString folderId = path.mid(strlen("csidl:"), strlen(path.toAscii())-strlen("csidl:")-strlen(".controly")); // e.g. 0x0014 = CSIDL_FONTS;
			bool ok;
			int folderIdx = folderId.toLong(&ok, 16);
			if (ok) {
				LPITEMIDLIST pidl;
				HRESULT hres = SHGetFolderLocation(NULL, folderIdx, NULL, 0, &pidl);
				if (hres == S_OK) {
					SHELLEXECUTEINFO sei;
					memset(&sei, 0, sizeof(sei));
					sei.cbSize = sizeof(SHELLEXECUTEINFO);
					sei.fMask = SEE_MASK_IDLIST;
					sei.hwnd = NULL;
					sei.lpVerb = NULL;
					sei.lpFile = NULL;
					sei.lpParameters = NULL;
					sei.lpDirectory = NULL;
					sei.nShow = SW_SHOW;
					sei.hInstApp = NULL;
					sei.lpIDList = pidl;
					sei.hProcess = NULL;
					//it seems we don't need CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
					ShellExecuteEx(&sei);

					CoTaskMemFree((void*)pidl); // needs objbase.h/ole32.lib
				}
			}
		} else {
			// exe cpl item with command line parameters, e.g. 'rundll32.exe shell32.dll,Options_RunDLL 1'
			// or item defined via application name, e.g. 'control.exe /name Microsoft.WindowsUpdate'

			QStringList spl = path.split(".exe ");
			if (spl.size() == 2) {
				// split size currently is always 2, as assured above
				QString executable = spl[0] + ".exe";
				QString parameters = spl[1];
				runProgram(executable, parameters);
			} else {
				runProgram(path, "");
			}
		}

		return 1;
	}

	if (id->count() != 2)
		return 1;

	CatItem last = id->last().getTopResult();
	QSettings* set = *settings;
	if (set == NULL) return 1;
	if (last.shortName == "Options") {
		set->setValue("controly/OptionsCount", set->value("controly/OptionsCount",0).toInt() + 1);
		return MSG_CONTROL_OPTIONS;
	}
	else if (last.shortName == "Rebuild Index") {
		set->setValue("controly/RebuildCount", set->value("controly/RebuildCount",0).toInt() + 1);
		return MSG_CONTROL_REBUILD;
	}
	else if (last.shortName == "Exit") {
		set->setValue("controly/ExitCount", set->value("controly/ExitCount",0).toInt() + 1);
		return MSG_CONTROL_EXIT;
	}
	return 1;

}

#ifdef WITH_GUI
void controlyPlugin::doDialog(QWidget* parent, QWidget** newDlg) {
	if (gui != NULL) {
		return;
	}
	gui = new Gui(parent);
	*newDlg = gui;
}

void controlyPlugin::endDialog(bool accept) {
	if (accept) {
		gui->writeOptions();
		init();
	}
	if (gui != NULL) {
		delete gui;
	}
	gui = NULL;
}
#endif

int controlyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	int handled = 0;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = 1;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = 1;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = 1;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = 1;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = 1;
			break;
		case MSG_LAUNCH_ITEM:
			handled = launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
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

Q_EXPORT_PLUGIN2(controly, controlyPlugin)