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

    QString toRun = path + " " + args;
    toRun = toRun.simplified();

    // Try xdg-open first, which should spawn quickly
    /*
    proc.start(QString("xdg-open"), QStringList(toRun.trimmed()));
    bool done = proc.waitForFinished(10000);
    int status = proc.exitCode();
    
    if (args.trimmed().size() > 0)
	largs = args.trimmed().split(" ");

    // If it couldn't handle it, see if we can
    if (status > 0 && info.isExecutable()) {
	proc.startDetached(path, largs);
	return;
    }
    */


    // xdg-open <args> || <args>

    /*
    QStringList r;
    r += "xdg-open";
    r += path;
    r += args.trimmed().split(" ");
    r += "||";
    r += path;
    r += args.trimmed().split(" ");
    */
    QString r = toRun + " 2>/dev/null || xdg-open \"" + toRun + "\" 2>/dev/null";
    //    QString r = "xdg-open \"" + toRun + "\" 2>/dev/null || " + toRun;

    //    qDebug() << r.simplified();
    QStringList ra;

    ra += "-c";
    ra += r.simplified();
    proc.startDetached("sh", ra);
    //proc.execute("sh", ra);
    return;
}
#endif

