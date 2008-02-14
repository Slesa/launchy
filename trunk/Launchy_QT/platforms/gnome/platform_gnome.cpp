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

#include <gtk/gtk.h>
#include <libgnomevfs/gnome-vfs.h>
#include <QtGui>
#include <QApplication>
#include "platform_gnome.h"


PlatformGnome::PlatformGnome() : PlatformBase() 		
{
}

QApplication* PlatformGnome::init(int* argc, char** argv)
{
    // MUST CALL GTK BEFORE QAPP!  Otherwise things get confused
    gtk_init(argc, &argv);
    gnome_vfs_init();
    QApplication * app = new QApplication(*argc, argv);
    icons = (QFileIconProvider*) new GnomeIconProvider();
    return app;
}

QList<Directory> PlatformGnome::GetInitialDirs() {
    QList<Directory> list;
    Directory tmp;
    QString ret;
    tmp.name = "/media/d/Music/";
    tmp.types << "*.mp3";
    list.append(tmp);
    tmp.name = "/usr/share/applications/";
    tmp.types << "*.desktop";
    list.append(tmp);
    
    return list;
}

Q_EXPORT_PLUGIN2(platform_gnome, PlatformGnome)
