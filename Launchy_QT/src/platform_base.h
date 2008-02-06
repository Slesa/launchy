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
#include <QObject>


#include "options.h"

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
//	virtual void Execute(QString path, QString args) = 0;
	virtual QString expandEnvironmentVars(QString) = 0;
	virtual bool isAlreadyRunning() = 0;

	QKeySequence oldKey;

	// Set hotkey
	virtual void SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot) = 0;



	// Alpha border functions	
	virtual bool SupportsAlphaBorder() { return false; }
	virtual bool CreateAlphaBorder(QWidget* w, QString ImageName) { w = w; ImageName = ImageName; return false; }
	virtual void DestroyAlphaBorder() { return; }
	virtual void MoveAlphaBorder(QPoint pos) { pos = pos; return; }
	virtual void HideAlphaBorder() { return; }
	virtual void ShowAlphaBorder() { return; }
	virtual void SetAlphaOpacity(double) { return; }


};

 Q_DECLARE_INTERFACE(PlatformBase,
                     "net.launchy.PlatformBase/1.0")

#endif
