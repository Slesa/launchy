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
#include <QX11Info>
#include "platform_gnome.h"


PlatformGnome::PlatformGnome() : PlatformBase() 		
{
    alpha = NULL;
}

QApplication* PlatformGnome::init(int* argc, char** argv)
{
    // MUST CALL GTK BEFORE QAPP!  Otherwise things get confused
    g_thread_init(NULL); // necessary since gtk called from catalog thread
    gdk_threads_init();
    gtk_init(argc, &argv);
    gnome_vfs_init();
    
        
    QApplication * app = new QApplication(*argc, argv);
    icons = (QFileIconProvider*) new GnomeIconProvider();
    return app;
}

PlatformGnome::~PlatformGnome()
{ 
    GlobalShortcutManager::clear();
    delete icons;
    gnome_vfs_shutdown();
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
    //    return;
    
    if (!item->fullPath.endsWith(".desktop", Qt::CaseInsensitive))
	return;
    GError * error = NULL;

    gdk_threads_enter();
    GnomeDesktopItem* ditem = gnome_desktop_item_new_from_file(item->fullPath.toLocal8Bit().data(),
							       (GnomeDesktopItemLoadFlags) 0,//GNOME_DESKTOP_ITEM_LOAD_ONLY_IF_EXISTS,
							       &error);
    if (error) {
	g_error_free(error);
	gdk_threads_leave();
	return;
    }

    if (!ditem) {
	gdk_threads_leave();
	return;
    }

    // The gnome errors are coming from here, because it falls back to
    // finding the file if it can't find the icon and spits out a
    // nasty error.  Nothing I can do but reimplement get_icon, pretty
    // annoying behavior on gnome's part
   
    GtkIconTheme* thm = gtk_icon_theme_get_for_screen(gdk_screen_get_default());
    //    item->icon = gnome_desktop_item_get_icon (ditem, gtk_icon_theme_get_default());
    item->icon = gnome_desktop_item_get_icon(ditem, thm);
    //item->fullPath = gnome_desktop_item_get_localestring(ditem, "Exec");
    //    item->shortName = gnome_desktop_item_get_localestring(ditem, "Name");
    //item->lowName = item->shortName.toLower();
    gnome_desktop_item_unref (ditem);    
    gdk_threads_leave();
    return;
}

bool PlatformGnome::Execute(QString path, QString args)
{
    if (!path.endsWith(".desktop", Qt::CaseInsensitive))
	return false;

    gdk_threads_enter();

    GError * error = NULL;
    GnomeDesktopItem* ditem = gnome_desktop_item_new_from_file(path.toLocal8Bit().data(),
							       (GnomeDesktopItemLoadFlags) 0,
							       &error);
    if (error) {
	g_error_free(error);
	gdk_threads_leave();
	return false;
    }
    if (!ditem) return false;
    
    
    gnome_desktop_item_launch(ditem, NULL, (GnomeDesktopItemLaunchFlags) 0, &error);
    if (error) {
	g_error_free(error);
	gnome_desktop_item_unref(ditem);
	gdk_threads_leave();
	return false;
    }
			      
    gnome_desktop_item_unref(ditem);
    gdk_threads_leave();
    return true;
}


bool PlatformGnome::CreateAlphaBorder(QWidget* w, QString ImageName)
{
    if (alpha)
	delete alpha;
  
    if (ImageName == "")
	ImageName = alphaFile;
    alphaFile = ImageName;
    alpha = new GnomeAlphaBorder(w, ImageName);
    //    alpha->ShowAlpha(ImageName);
    return true;
}

bool PlatformGnome::SupportsAlphaBorder()
{
    return QX11Info::isCompositingManagerRunning();
    /*
    QProcess qp;
    QString program = "/bin/sh";
    QStringList args;
    args << "-c" << "ps ax | grep 'compiz' | grep -v 'grep'";
    qp.start(program, args);   
    qp.waitForFinished();
    QByteArray result = qp.readAll();
    //    qDebug() << result;
    if (result.length() > 0)
	return true;
    return false;
    */
}

Q_EXPORT_PLUGIN2(platform_gnome, PlatformGnome)
