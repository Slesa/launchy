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

#ifndef PLATFORM_BASE_H
#define PLATFORM_BASE_H

#include "catalog.h"
#include "directory.h"
#include <QApplication>
#include <QFileIconProvider>
#include <QKeySequence>
/*
#include <QtGui> // OSX needs this
#include "globals.h"
*/

class PlatformBase : public QApplication
{
public:
    PlatformBase(int& argc, char** argv);
    virtual ~PlatformBase();

	QIcon icon(const QFileInfo& info) { return icons->icon(info); }
	QIcon icon(QFileIconProvider::IconType type) { return icons->icon(type); }
	virtual void setPreferredIconSize(int size) = 0;

	virtual QList<Directory> getDefaultCatalogDirectories() = 0;
	virtual void sendInstanceCommand(int command) { Q_UNUSED(command); }

	// Need to alter an indexed item?  e.g. .desktop files
	virtual void alterItem(CatItem*) { }
	virtual QHash<QString, QList<QString> > getDirectories() = 0;
	virtual QString expandEnvironmentVars(QString txt) = 0;

	virtual bool supportsAlphaBorder() const { return false; }
	virtual bool getComputers(QStringList& computers) const { Q_UNUSED(computers); return false; }


protected:
    QFileIconProvider* icons;
};

#endif
