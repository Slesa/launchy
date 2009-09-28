/*
Launchy: Application Launcher
Copyright (C) 2007-2009  Josh Karlin, Simon Capewell

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
#include "main.h"
#include "platform_win.h"
#include "WinIconProvider.h"


// Override the main widget to handle incoming system messages. We could have done this in the QApplication 
// event handler, but then we'd have to filter out the duplicates for messages like WM_SETTINGCHANGE.
class LaunchyWidgetWin : public LaunchyWidget
{
public:
	LaunchyWidgetWin(CommandFlags command) :
		LaunchyWidget(command)
	{
		commandMessageId = RegisterWindowMessage(_T("LaunchyCommand"));
	}

	virtual bool winEvent(MSG* msg, long* result)
	{
		switch (msg->message)
		{
		case WM_SETTINGCHANGE:
			// Refresh Launchy's environment on settings changes
			if (msg->lParam && _tcscmp((TCHAR*)msg->lParam, _T("Environment")) == 0)
			{
				UpdateEnvironment();
			}
			break;

		// Might need to capture these two messages if Vista gives any problems with alpha borders
		// when restoring from standby
		case WM_POWERBROADCAST:
			break;
		case WM_WTSSESSION_CHANGE:
			break;

		default:
			if (msg->message == commandMessageId)
			{
				// A Launchy startup command
				executeStartupCommand(msg->wParam);
			}
			break;
		}
		return LaunchyWidget::winEvent(msg, result);
	}

private:
	UINT commandMessageId;
};


// Create the main widget for the application
LaunchyWidget* createLaunchyWidget(CommandFlags command)
{
	return new LaunchyWidgetWin(command);
}



PlatformWin::PlatformWin(int& argc, char** argv) :
	PlatformBase(argc, argv)
{
	instance = new LimitSingleInstance(TEXT("Local\\{ASDSAD0-DCC6-49b5-9C61-ASDSADIIIJJL}"));

	// Create local and global application mutexes so that installer knows when
	// Launchy is running
	localMutex = CreateMutex(NULL,0,_T("LaunchyMutex"));
	globalMutex = CreateMutex(NULL,0,_T("Global\\LaunchyMutex"));

	icons = (QFileIconProvider*)new WinIconProvider();
}


PlatformWin::~PlatformWin()
{
	if (localMutex)
		CloseHandle(localMutex);
	if (globalMutex)
		CloseHandle(globalMutex);
	delete instance;
	instance = NULL;
}


void PlatformWin::setPreferredIconSize(int size)
{
	((WinIconProvider*)icons)->setPreferredIconSize(size);
}


QHash<QString, QList<QString> > PlatformWin::getDirectories()
{
    QHash<QString, QList<QString> > out;
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");
    QString iniFilename = settings.fileName();
	QFileInfo info(iniFilename);
	QString userDataPath = info.absolutePath();

	out["config"] << iniFilename;
    out["db"] << userDataPath + "/Launchy.db";
    out["history"] << userDataPath + "/history.db";
	out["skins"] << qApp->applicationDirPath() + "/skins"
				 << userDataPath + "/skins";
    out["plugins"] << qApp->applicationDirPath() + "/plugins"
				   << userDataPath + "/plugins";
    out["portConfig"] << qApp->applicationDirPath() + "/Launchy.ini";
    out["portDB"] << qApp->applicationDirPath() + "/Launchy.db";
    out["defSkin"] << "Default";
    return out;
}


QList<Directory> PlatformWin::getDefaultCatalogDirectories()
{
	QList<Directory> list;
	Directory tmp;

	tmp.name = GetShellDirectory(CSIDL_COMMON_STARTMENU);
	tmp.types << "*.lnk";
	list.append(tmp);
	
	tmp.name = GetShellDirectory(CSIDL_STARTMENU);
	list.append(tmp);
	tmp.name = "Utilities\\";
	tmp.indexDirs = false;
	list.append(tmp);

	Directory tmp2;
	tmp2.name = "%appdata%\\Microsoft\\Internet Explorer\\Quick Launch";
	tmp2.types << "*.*";
	list.append(tmp2);
	return list;
}


QString PlatformWin::expandEnvironmentVars(QString txt) 
{
	QString delim("%");
	QString out = "";
	int curPos = txt.indexOf(delim, 0);
	if (curPos == -1)
		return txt;
	while (curPos != -1) {
		int nextPos = txt.indexOf(delim, curPos+1);
		if (nextPos == -1) {
			out += txt.mid(curPos+1);
			break;
		}
		QString var = txt.mid(curPos+1, nextPos-curPos-1);
		DWORD size = GetEnvironmentVariableW((LPCTSTR) var.utf16(), NULL, 0);
		if (size > 0) {
			LPWSTR tmpString = (LPWSTR) malloc(size*sizeof(TCHAR));
			GetEnvironmentVariableW((LPCTSTR) var.utf16(), tmpString, size);
			out += QString::fromUtf16((const ushort*) tmpString);
			free(tmpString);
		} else {
			out += "%" + var + "%";
		}
		curPos = nextPos;
	}
	return out;
}


void PlatformWin::sendInstanceCommand(int command)
{
	UINT commandMessageId = RegisterWindowMessage(_T("LaunchyCommand"));
	PostMessage(HWND_BROADCAST, commandMessageId, command, 0);
}


bool PlatformWin::isAlreadyRunning() const
{
	return instance->IsAnotherInstanceRunning();
}


// Mandatory functions
QKeySequence PlatformWin::getHotkey() const
{
	return hotkey;
}

bool PlatformWin::setHotkey(const QKeySequence& newHotkey, QObject* receiver, const char* slot)
{
	GlobalShortcutManager::disconnect(hotkey, receiver, slot);
	GlobalShortcutManager::connect(newHotkey, receiver, slot);
	hotkey = newHotkey;
	return GlobalShortcutManager::isConnected(newHotkey);
}


bool PlatformWin::supportsAlphaBorder() const
{
	return true;
}


// Create the application object
QApplication* createApplication(int& argc, char** argv)
{
	return new PlatformWin(argc, argv);
}
