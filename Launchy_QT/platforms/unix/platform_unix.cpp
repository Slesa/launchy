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


PlatformUnix::PlatformUnix() : PlatformBase() 		
{
    alpha = NULL;
}

QApplication* PlatformUnix::init(int* argc, char** argv)
{        
    QApplication * app = new QApplication(*argc, argv);
    icons = new QFileIconProvider();
    return app;
}

PlatformUnix::~PlatformUnix()
{ 
    GlobalShortcutManager::clear();
    delete icons;
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

    list.append(Directory("~",QStringList(),true,false,1));
    
    return list;
}


QHash<QString, QList<QString> > PlatformUnix::GetDirectories(bool portable) {
    QHash<QString, QList<QString> > out;
    QString baseDir = "/usr";
    QDir d;
    d.mkdir(QDir::homePath() + "/.launchy");
    
    out["skins"] += baseDir + "/share/launchy/skins";
    out["skins"] += QDir::homePath() + "/.launchy/skins";
    out["plugins"] += baseDir + "/lib/launchy/plugins";
    out["plugins"] += QDir::homePath() + "/.launchy/plugins";
    if (portable)
	out["config"] += qApp->applicationDirPath() + "/launchy.ini";
    else 
	out["config"] += QDir::homePath() + "/.launchy/launchy.ini";
    
    return out;
}



bool PlatformUnix::CreateAlphaBorder(QWidget* w, QString ImageName)
{
    if (alpha)
	delete alpha;
  
    if (ImageName == "")
	ImageName = alphaFile;
    alphaFile = ImageName;
    alpha = new AlphaBorder(w, ImageName);
    return true;
}

bool PlatformUnix::SupportsAlphaBorder()
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

//Q_EXPORT_PLUGIN2(platform_unix, PlatformUnix)
