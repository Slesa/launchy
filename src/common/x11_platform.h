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
#ifndef X11PLATFORM_H
#define X11PLATFORM_H

#include "x11_iconprovider.h"
#include "platform_base.h"


#include <X11/X.h>
#include <X11/Xlib.h>

class X11Platform:  public PlatformBase
{
    Q_OBJECT

 public:
    X11Platform(int & argc, char** argv);
    ~X11Platform();
    
    void setPreferredIconSize(int size) { size = size; return; }

    //virtual shared_ptr<QApplication> init(int & argc, char** argv);
/*
    QString GetSettingsDirectory()
    {
        return "";
    }
*/
    
    QList<Directory> getDefaultCatalogDirectories();
    
   /*
    void AddToNotificationArea() {};
    void RemoveFromNotificationArea() {};
    */
    

    virtual QHash<QString, QList<QString> > getDirectories();
	virtual QString expandEnvironmentVars(QString txt);

    bool supportsAlphaBorder() const;
    /*
    QIcon icon(const QFileInfo& info) {
    	shared_ptr<UnixIconProvider> u(dynamic_pointer_cast<UnixIconProvider>(icons));
  	  	return u->getIcon(info);
//		return ((UnixIconProvider*) icons.get())->getIcon(info); 
    }
    */

    virtual void alterItem(CatItem*);
};

#endif
