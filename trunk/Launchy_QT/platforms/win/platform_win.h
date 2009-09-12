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

#ifndef PLATFORM_WIN
#define PLATFORM_WIN


#include <QFileIconProvider>
#include <windows.h>
#include <shlobj.h>
#include <userenv.h>
#include <tchar.h>

#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QtDebug>
#include <QMouseEvent>
#include "platform_base.h"
#include "platform_win_util.h"
#include "platform_base_hotkey.h"


class PlatformWin : public QObject, public PlatformBase 
{
	Q_OBJECT
	Q_INTERFACES(PlatformBase)

public:
	PlatformWin();
	~PlatformWin();

	virtual shared_ptr<QApplication> init(int& argc, char** argv);

/*
	void KillLaunchys() {
		
		qint64 mypid = qApp->applicationPid();
		QString toRun = "TASKKILL /F /FI \"PID lt " + QString::number(mypid) + "\" /IM Launchy.exe";
		QProcess proc;
		proc.execute(toRun);	
		
	}
*/

	// Mandatory functions
	bool SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot)
	{
		GlobalShortcutManager::disconnect(oldKey, receiver, slot);
		GlobalShortcutManager::connect(key, receiver, slot);
		oldKey = key;
		return GlobalShortcutManager::isConnected(key);
	}

	QString GetSettingsDirectory()
	{ 
		return GetShellDirectory(CSIDL_APPDATA);
	}

	virtual QHash<QString, QList<QString> > GetDirectories();

	QList<Directory> GetDefaultCatalogDirectories()
	{
		QList<Directory> list;
		Directory tmp;

		tmp.name = GetShellDirectory(CSIDL_COMMON_STARTMENU);
		tmp.types << "*.lnk";
		list.append(tmp);
		
		tmp.name = GetShellDirectory(CSIDL_STARTMENU);
		list.append(tmp);
		tmp.name = "Utilities\\";
		tmp.indexDirs = true;
		list.append(tmp);

		Directory tmp2;
		tmp2.name = "%appdata%\\Microsoft\\Internet Explorer\\Quick Launch";
		tmp2.types << "*.*";
		list.append(tmp2);
		return list;
	}

	QString expandEnvironmentVars(QString);
	void AddToNotificationArea() {};
	void RemoveFromNotificationArea() {};

	// Alpha border functions	
	bool SupportsAlphaBorder() const { return true; }

	bool isAlreadyRunning()
	{
		return instance->IsAnotherInstanceRunning();
	}
	void showOtherInstance();

private:
	HANDLE m1, mg1;
	QString lastImageName;
	LimitSingleInstance* instance;
};


#endif
