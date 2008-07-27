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
#include "plugin_interface.h"
#include <QProcess>
#include <QDebug>
#include <QLocale>


/*! \file
    \brief A Documented file.
    
    Details.
*/


#ifdef Q_WS_WIN
int getDesktop() { return DESKTOP_WINDOWS; }
void runProgram(QString path, QString args) {
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (LPCTSTR) (path).utf16();
	if (args != "") {
		ShExecInfo.lpParameters = (LPCTSTR) args.utf16();
	} else {
		ShExecInfo.lpParameters = NULL;
	}
	QDir dir(path);
	QFileInfo info(path);
	if (!info.isDir() && info.isFile())
		dir.cdUp();
	ShExecInfo.lpDirectory = (LPCTSTR)QDir::toNativeSeparators(dir.absolutePath()).utf16();
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	ShellExecuteEx(&ShExecInfo);	
}
#endif

#ifdef Q_WS_MAC

#endif

#ifdef Q_WS_X11
int getDesktop()
{
    QStringList list = QProcess::systemEnvironment();
    foreach(QString s, list)
	{
	    if (s.startsWith("GNOME_DESKTOP_SESSION"))
		return DESKTOP_GNOME;
	    else if (s.startsWith("KDE_FULL_SESSION"))
		return DESKTOP_KDE;
	}
    return -1;
}

void runProgram(QString path, QString args) {
    QProcess proc;
    QStringList largs;
    QFileInfo info(path);


    if (path.contains("%")) {
	path.replace("%u", args);
	path.replace("%U", args);
	path.replace("%f", args);
	path.replace("%F", args);
	path.replace("%c", path.split(" ")[0]);
	path.replace("%k", path.split(" ")[0]);
	args = "";
    } 

    QString toRun = path + " " + args;
    toRun = toRun.simplified();
    






    QString r;

    //    r = "xdg-open \"" + path + "\" " + args + " 2>/dev/null || sh -c \"" + path + "\" " + args;

    r = "xdg-open \"" + path.trimmed() + "\" " + args.trimmed() + " 2>/dev/null || sh -c \"" + path.trimmed() + " "  + args + "\"";


    //    qDebug() << r.simplified();
    QStringList ra;

    ra += "-c";
    ra += r.trimmed().simplified();
    //    qDebug() << ra;

    // Firefox needs special treatment in KDE
    // else it falls behind a window
    if ((path.contains("http://",Qt::CaseInsensitive) ||
	 path.contains("firefox",Qt::CaseInsensitive)) &&
	getDesktop() == DESKTOP_KDE) {
	proc.execute("sh",ra);
    } else {
	proc.startDetached("sh",ra);
    }

    //proc.execute("sh", ra);
    
    return;
}











/*
void runProgram(QString path, QString args) {
    // My own launcher..
    QString mimetype;
    QString locale = QLocale::system().name();
    if (path.startsWith("http", Qt::CaseInsensitive))
	mimetype = "text/html";
    if (mimetype == "") {

	QProcess proc;
	
	QStringList args;
	args += "query";
	args += "filetype";
	args += path;
	proc.setReadChannel(QProcess::StandardOutput);
	proc.start(QString("xdg-mime"),args);
	proc.waitForFinished(10000);
	mimetype = proc.readAll().trimmed();
	proc.close();
    }

    // Get the default app for the mime-type
    QString defapp;
    if (mimetype.startsWith("application", Qt::CaseInsensitive))
	defapp = path;

    if (mimetype == "")
	defapp = path;

    if (defapp == "") {
	QProcess proc;
	QStringList args;
	args += "query";
	args += "default";
	args += mimetype;
	proc.start(QString("xdg-mime"),args);
	proc.waitForFinished(10000);
	QString desk = proc.readAll().trimmed();
	if (desk == "")
	    defapp = path;
	else {
	    QString icon;
	    QString name;
	    // Read the .desktop file
	    const char *dirs[] = { "/usr/share/applications/",
				   "/usr/local/share/applications/",
				   "/usr/share/gdm/applications/",
				   "/usr/share/applications/kde/",
				   "~/.local/share/applications/"};
	    for(int i = 0; i < 5; i++) {
		QString dir = dirs[i];
		QString path = dir + "/" + desk;
		
		if (QFile::exists(path)) {
		    QFile file(path);
		    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
			break;
		    while(!file.atEnd()) {
			QString line = file.readLine();
			if (line.startsWith("Exec", Qt::CaseInsensitive)) {
			    defapp = line.split("=")[1].trimmed();
			}
			else if (line.startsWith("Icon",Qt::CaseInsensitive))
			    icon = line.split("=")[1].trimmed();
			else if (line.startsWith("Name",Qt::CaseInsensitive)) {
			    if (line.startsWith("Name[" + locale, Qt::CaseInsensitive))
				name = line.split("=")[1].trimmed();
			    else if (!line.contains("["))
				name = line.split("=")[1].trimmed();
			    
			}
		    }
		    defapp.replace("%k", path);
		    break;
		}
	    }	    
	    defapp.replace("%u", "\"" + path + "\"");
	    defapp.replace("%U", "\"" + path + "\"");
	    defapp.replace("%f", "\"" + path + "\"");
	    defapp.replace("%F", "\"" + path + "\"");
	    defapp.replace("%i", "--icon " + icon);
	    defapp.replace("%c", name);
	}
    }
    


    //    qDebug() << mimetype << defapp;
    QString toRun = defapp + " " + args;
    QStringList largs = toRun.simplified().split(" ", QString::SkipEmptyParts);
    
    qDebug() << largs;
    QProcess proc;

    QString exec = largs[0];
    largs.removeFirst();
    qDebug() << exec << largs.join(" ");
    proc.startDetached(exec, QStringList(largs.join(" ")));

    //proc.startDetached(exec, largs);
    return;
}
*/
#endif

