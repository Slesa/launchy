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
#include <libgnome/gnome-desktop-item.h>
#include <QtGui>
#include <QApplication>
#include "platform_gnome.h"


PlatformGnome::PlatformGnome() : PlatformBase() 		
{
    alpha = NULL;
}

QApplication* PlatformGnome::init(int* argc, char** argv)
{
    // MUST CALL GTK BEFORE QAPP!  Otherwise things get confused
    qDebug() << "initing";
    g_thread_init(NULL);
    gdk_threads_init();
    gtk_init(argc, &argv);
    gnome_vfs_init();
    QApplication * app = new QApplication(*argc, argv);
    icons = (QFileIconProvider*) new GnomeIconProvider();
    return app;
}

QList<Directory> PlatformGnome::GetInitialDirs() {
    QList<Directory> list;
    const char *dirs[] = { "/usr/share/applications/",
			   "/usr/local/share/applications/",
			   "/usr/share/gdm/applications/",
			   "/usr/share/applications/kde/",
			   "~/.local/share/applications/"};
    for(int i = 0; i < 5; i++)
	{
	    Directory tmp;
	    tmp.name = dirs[i];
	    tmp.types << "*.desktop";
	    list.append(tmp);
	}
    
    return list;
}

void PlatformGnome::alterItem(CatItem* item) {
    if (!item->fullPath.endsWith(".desktop", Qt::CaseInsensitive))
	return;
    GnomeDesktopItem* ditem = gnome_desktop_item_new_from_file(item->fullPath.toLocal8Bit().data(),
							       GNOME_DESKTOP_ITEM_LOAD_ONLY_IF_EXISTS,
							       NULL);
    if (!ditem) return;
    
    item->icon = gnome_desktop_item_get_icon (ditem, gtk_icon_theme_get_default());

    //item->fullPath = gnome_desktop_item_get_localestring(ditem, "Exec");
    //    item->shortName = gnome_desktop_item_get_localestring(ditem, "Name");
    //item->lowName = item->shortName.toLower();
    gnome_desktop_item_unref (ditem);    
    return;
}

bool PlatformGnome::Execute(QString path, QString args)
{
    if (!path.endsWith(".desktop", Qt::CaseInsensitive))
	return false;

    GnomeDesktopItem* ditem = gnome_desktop_item_new_from_file(path.toLocal8Bit().data(),
							       (GnomeDesktopItemLoadFlags) 0,
							       NULL);
    if (!ditem) return false;
    
    
    gnome_desktop_item_launch(ditem, NULL, (GnomeDesktopItemLaunchFlags) 0, NULL);
    
    gnome_desktop_item_unref(ditem);
    return true;
}


bool PlatformGnome::CreateAlphaBorder(QWidget* w, QString ImageName)
{
    if (ImageName == "")
	ImageName = alphaFile;
    alphaFile = ImageName;
    alpha = new GnomeAlphaBorder(w);
    alpha->ShowAlpha(ImageName);
    return true;
}


Q_EXPORT_PLUGIN2(platform_gnome, PlatformGnome)
