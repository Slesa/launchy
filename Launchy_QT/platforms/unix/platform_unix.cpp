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

#include <QtGui>
#include <QApplication>
#include <QX11Info>
#include "platform_unix.h"
#include <boost/pointer_cast.hpp>  
#include <X11/Xlib.h>





PlatformUnix::PlatformUnix() : PlatformBase() 		
{
	alpha.reset();
//    alpha = NULL;
    icons.reset();
}

shared_ptr<QApplication> PlatformUnix::init(int & argc, char** argv)
{        
    //    QApplication * app = new QApplication(*argc, argv);
    shared_ptr<QApplication> app(new MyApp(argc, argv));
    icons.reset( (QFileIconProvider *) new UnixIconProvider());
    return app;
}

PlatformUnix::~PlatformUnix()
{ 
    GlobalShortcutManager::clear();
//    delete icons;
}

QList<Directory> PlatformUnix::GetInitialDirs() {
    QList<Directory> list;
    const char *dirs[] = { "/usr/share/applications/",
			   "/usr/local/share/applications/",
			   "/usr/share/gdm/applications/",
			   "/usr/share/applications/kde/",
			   "~/.local/share/applications/"};
    QStringList l;
    l << "*.desktop";
    
    for(int i = 0; i < 5; i++)
	list.append(Directory(dirs[i],l,false,false,100));

    list.append(Directory("~",QStringList(),true,false,0));
    
    return list;
}


QHash<QString, QList<QString> > PlatformUnix::GetDirectories() {
    QHash<QString, QList<QString> > out;
    QDir d;
    d.mkdir(QDir::homePath() + "/.launchy");
    
    out["skins"] += qApp->applicationDirPath() + "/skins";
    out["skins"] += QDir::homePath() + "/.launchy/skins";
    out["skins"] += SKINS_PATH;

    out["plugins"] += qApp->applicationDirPath() + "/plugins";
    out["plugins"] += QDir::homePath() + "/.launchy/plugins";
    out["plugins"] += PLUGINS_PATH;

    out["config"] += QDir::homePath() + "/.launchy/launchy.ini";
    out["portConfig"] += qApp->applicationDirPath() + "/launchy.ini";
    out["db"] += QDir::homePath() + "/.launchy/launchy.db";
    out["portDB"] += qApp->applicationDirPath() + "/launchy.db";
    
    if (QFile::exists(out["skins"].last() + "/Default"))
	out["defSkin"] += out["skins"].last() + "/Default";
    else
      out["defSkin"] += out["skins"].first() + "/Default";

    out["platforms"] += qApp->applicationDirPath();
    out["platforms"] += PLATFORMS_PATH;

    return out;
}



bool PlatformUnix::CreateAlphaBorder(QWidget* w, QString ImageName)
{
//   if (alpha)
//	delete alpha;
  
    if (ImageName == "")
	ImageName = alphaFile;
    alphaFile = ImageName;
    alpha.reset( new AlphaBorder(w, ImageName) ); 
    return true;
}

bool PlatformUnix::SupportsAlphaBorder()
{
    //    return QX11Info::isCompositingManagerRunning();
    
    QProcess qp;
    QString program = "/bin/sh";
    QStringList args;
    args << "-c" << "ps ax | grep 'compiz\\|beryl' | grep -v 'grep'";
    qp.start(program, args);   
    qp.waitForFinished();
    QByteArray result = qp.readAll();
    //    qDebug() << result;
    if (result.length() > 0)
	return true;
    return false;
    
}

//Q_EXPORT_PLUGIN2(platform_unix, PlatformUnix)


void PlatformUnix::alterItem(CatItem* item) {
    if (!item->fullPath.endsWith(".desktop", Qt::CaseInsensitive))
	return;

    QString locale = QLocale::system().name();
    
    

    QFile file(item->fullPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	return;

    QString name = "";
    QString icon = "";
    QString exe = "";
    while(!file.atEnd()) {
	QString line = file.readLine();
	
	if (line.startsWith("Name[" + locale, Qt::CaseInsensitive)) 
	    name = line.split("=")[1].trimmed();
	

	else if (line.startsWith("Name=", Qt::CaseInsensitive)) 
	    name = line.split("=")[1].trimmed();

	else if (line.startsWith("Icon", Qt::CaseInsensitive))
	    icon = line.split("=")[1].trimmed();
	else if (line.startsWith("Exec", Qt::CaseInsensitive))
	    exe = line.split("=")[1].trimmed();	
    }
    if (name.size() >= item->shortName.size() - 8) {
	item->shortName = name;
	item->lowName = item->shortName.toLower();
    }

        
    QStringList allExe = exe.trimmed().split(" ",QString::SkipEmptyParts);
    if (allExe.size() == 0)
	return;
    exe = allExe[0];
    allExe.removeFirst();
    //    exe = exe.trimmed().split(" ")[0];

    
    // Look for the executable in the path
    if (!QFile::exists(exe) && exe != "") {
	foreach(QString line, QProcess::systemEnvironment()) {
	    if (!line.startsWith("Path", Qt::CaseInsensitive)) 
		continue;

	    QStringList spl = line.split("=");
	    QStringList spl2 = spl[1].split(":");
	    foreach(QString dir, spl2) {
		QString tmp = dir + "/" + exe;
		if (QFile::exists(tmp)) {
		    exe = tmp;
		    break;
		}
	    }
	    break;
	}
    }
    
    
    item->fullPath = exe + " " + allExe.join(" ");

    // Cache the icon for this desktop file
    shared_ptr<UnixIconProvider> u(dynamic_pointer_cast<UnixIconProvider>(icons));
//    shared_ptr<UnixIconProvider> u((UnixIconProvider*) icons.get());
    
    icon = u->getDesktopIcon(file.fileName(), icon);

    item->icon = icon;

    file.close();
    return;
}




Q_EXPORT_PLUGIN2(platform_unix, PlatformUnix)
