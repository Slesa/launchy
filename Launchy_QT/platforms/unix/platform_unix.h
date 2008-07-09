/*
  Launchy: Application Launcher
  Copyright (C) 2008  Josh Karlin
  
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

#pragma once

#include "platform_unix_util.h"
#include "platform_base.h"
#include "platform_base_hotkey.h"
#include "platform_base_hottrigger.h"



class PlatformUnix : public QObject, public PlatformBase 
{
    Q_OBJECT
	Q_INTERFACES(PlatformBase)
	private:
    AlphaBorder * alpha;
    QString alphaFile;
 public:
    PlatformUnix();
    ~PlatformUnix();
    
    virtual QApplication* init(int* argc, char** argv);
    // Mandatory functions
    // Mandatory functions
    void SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot)
    {
	GlobalShortcutManager::disconnect(oldKey, receiver, slot);
	GlobalShortcutManager::connect(key, receiver, slot);
	oldKey = key;
    }
    
    QString GetSettingsDirectory() { 
	return "";
    };
    
    
    QList<Directory> GetInitialDirs();
    
    
    void AddToNotificationArea() {};
    void RemoveFromNotificationArea() {};
    
    bool isAlreadyRunning() {
	return false;
    }
    

    virtual QHash<QString, QList<QString> > GetDirectories();

    bool SupportsAlphaBorder();
    bool CreateAlphaBorder(QWidget* w, QString ImageName);
    void DestroyAlphaBorder() { delete alpha ; alpha = NULL; return; }
    void MoveAlphaBorder(QPoint pos) { if (alpha) alpha->move(pos); }
    void ShowAlphaBorder() { if (alpha) alpha->show(); }
    void HideAlphaBorder() { if (alpha) alpha->hide(); }
    void SetAlphaOpacity(double trans ) { if (alpha) alpha->SetAlphaOpacity(trans); }

    virtual void alterItem(CatItem*);
};



