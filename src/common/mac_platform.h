/*
Launchy: Application Launcher
Copyright (C) 2007-2010 Josh Karlin

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
#ifndef MACPLATFORM_H
#define MACPLATFORM_H

#include "platform_base.h"
#include "globalshortcutmanager.h"
#include "globalshortcuttrigger.h"

class MacPlatform: public PlatformBase
{
	Q_OBJECT

public:
    MacPlatform(int& argc, char** argv);
    ~MacPlatform();

    virtual void setPreferredIconSize(int size) { Q_UNUSED(size) return; }
    virtual QHash<QString, QList<QString> > getDirectories();
    virtual QList<Directory> getDefaultCatalogDirectories();
    virtual QString expandEnvironmentVars(QString);
    virtual bool supportsAlphaBorder() const { return true; }
    virtual void alterItem(CatItem* item);
};

#endif
