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

#include <QX11Info>
#include <boost/shared_ptr.hpp>

using namespace boost;

/*
  This QAPP scans all x events for keypresses
  and sends them off to the hotkey manager
*/


class MyApp : public QApplication {
    Q_OBJECT
    public:
	MyApp(int & argc, char** argv) : QApplication(argc,argv) {}
	bool x11EventFilter ( XEvent * event ) {
	    if (event->type == KeyPress) {
		emit xkeyPressed(event);
	    }
	return false;
    }    
signals:
    void xkeyPressed(XEvent*);
};

class PlatformUnix : public QObject, public PlatformBase 
{
    Q_OBJECT
	Q_INTERFACES(PlatformBase)
	private:
	shared_ptr<AlphaBorder> alpha;
    QString alphaFile;
 public:
    PlatformUnix();
    ~PlatformUnix();
    
    virtual shared_ptr<QApplication> init(int & argc, char** argv);
    // Mandatory functions
    // Mandatory functions
    bool SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot)
    {
	GlobalShortcutManager::disconnect(oldKey, receiver, slot);
	GlobalShortcutManager::connect(key, receiver, slot);
	oldKey = key;
	return GlobalShortcutManager::isConnected(key);
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
    

    shared_ptr<QWidget> getAlphaWidget() {
		return alpha;
    }

    virtual QHash<QString, QList<QString> > GetDirectories();

    bool SupportsAlphaBorder();
    bool CreateAlphaBorder(QWidget* w, QString ImageName);
    void DestroyAlphaBorder() { alpha.reset(); /*delete alpha ; alpha = NULL;*/ return; }
    void MoveAlphaBorder(QPoint pos) { if (alpha) alpha->move(pos); }
    void ShowAlphaBorder() { if (alpha) alpha->show(); }
    void HideAlphaBorder() { if (alpha) alpha->hide(); }
    void SetAlphaOpacity(double trans ) { if (alpha) alpha->SetAlphaOpacity(trans); }

    QIcon icon(const QFileInfo& info) {
    	shared_ptr<UnixIconProvider> u(dynamic_pointer_cast<UnixIconProvider>(icons));
  	  	return u->getIcon(info);
//		return ((UnixIconProvider*) icons.get())->getIcon(info); 
    }

    virtual void alterItem(CatItem*);

};




