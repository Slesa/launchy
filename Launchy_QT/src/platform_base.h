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

#ifndef PLATFORM_BASE_H
#define PLATFORM_BASE_H
#include <QWidget>
#include <QPoint>
#include <QStringList>
#include <QList>
#include <QFileIconProvider>

#include "options.h"

#include "platform_base_hotkey.h"
#include "platform_Base_hottrigger.h"

class PlatformBase {
public:
	PlatformBase() {
		icons = NULL;
	}
	virtual ~PlatformBase() {
	}

	QFileIconProvider* icons;


	virtual QString GetSettingsDirectory() = 0;
	virtual QList<Directory> GetInitialDirs() = 0;
	virtual void AddToNotificationArea() = 0;
	virtual void RemoveFromNotificationArea() = 0;
	virtual void Execute(QString path, QString args) = 0;


	QKeySequence oldKey;

	// Set hotkey
	void SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot) {
		GlobalShortcutManager::disconnect(oldKey, receiver, slot);
		GlobalShortcutManager::connect(key, receiver, slot);
		oldKey = key;
	}


	// Alpha border functions	
	virtual bool SupportsAlphaBorder() { return false; }
	virtual bool CreateAlphaBorder(WId id, QString ImageName) { return false; }
	virtual void DestroyAlphaBorder() { return; }
	virtual void MoveAlphaBorder(QPoint pos) { return; }
	virtual void HideAlphaBorder() { return; }
	virtual void ShowAlphaBorder() { return; }


};

/*
class LaunchyIconProvider2 : friend QFileIconProvider {
	LaunchyIconProvider2() : QFileIconProvider() {}
//	~LaunchyIconProvider() : ~QFileIconProvider() {}
	QIcon icon ( const QFileInfo & info ) const;

};
*/
#endif