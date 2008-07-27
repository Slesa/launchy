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
#include <QKeyEvent>
#include <QDir>
#include <QObject>
#include "platform_win.h"
#include "globals.h"

QHash<QString, QList<QString> > PlatformWin::GetDirectories() {
    QHash<QString, QList<QString> > out;
    out["skins"] += qApp->applicationDirPath() + "/skins";
    out["plugins"] += qApp->applicationDirPath() + "/plugins";
    out["portConfig"] += qApp->applicationDirPath() + "/Launchy.ini";
    QSettings tmp(QSettings::IniFormat, QSettings::UserScope, "Launchy", "Launchy");
    out["config"] += tmp.fileName();
    out["portDB"] += qApp->applicationDirPath() + "/Launchy.db";
	qDebug() << out["config"][0];
	QDir d(out["config"][0]);
    d.cdUp();
    out["db"] += d.absoluteFilePath("Launchy.db");
    out["defSkin"] += out["skins"][0] + "/Default";
    out["platforms"] += qApp->applicationDirPath();
    return out;
}


QString PlatformWin::expandEnvironmentVars(QString txt) 
{
	QString delim("%");
	QString out = "";
	int curPos = txt.indexOf(delim, 0);
	if (curPos == -1)
		return txt;
	while (curPos != -1) {
		int nextPos = txt.indexOf(delim, curPos+1);
		if (nextPos == -1) {
			out += txt.mid(curPos+1);
			break;
		}
		QString var = txt.mid(curPos+1, nextPos-curPos-1);
		DWORD size = GetEnvironmentVariableW((LPCTSTR) var.utf16(), NULL, 0);
		if (size > 0) {
			LPWSTR tmpString = (LPWSTR) malloc(size*sizeof(TCHAR));
			GetEnvironmentVariableW((LPCTSTR) var.utf16(), tmpString, size);
			out += QString::fromUtf16((const ushort*) tmpString);
			free(tmpString);
		} else {
			out += "%" + var + "%";
		}
		curPos = nextPos;
	}
	return out;
}

Q_EXPORT_PLUGIN2(platform_win, PlatformWin) 
