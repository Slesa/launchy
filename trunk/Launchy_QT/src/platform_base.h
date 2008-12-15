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

#ifndef PLATFORM_BASE_H
#define PLATFORM_BASE_H
#include <QtGui>
#include <QWidget>
#include <QPoint>
#include <QStringList>
#include <QList>
#include <QFileIconProvider>
#include <QObject>
#include <QProcess>
#include <QDebug>
#include "catalog.h"
#include "options.h"
#include <boost/shared_ptr.hpp>

using namespace boost;
class PlatformBase {
public:
	PlatformBase() {
//		icons = NULL;
	}

	virtual ~PlatformBase() {
	}

	shared_ptr<QFileIconProvider> icons;

	virtual shared_ptr<QApplication> init(int & argc, char**  argv) { 
		return shared_ptr<QApplication>(new QApplication(argc, argv));
	}

	virtual shared_ptr<QWidget> getAlphaWidget() = 0;

	virtual QIcon icon(const QFileInfo & info) { return icons->icon(info); }
	virtual QString GetSettingsDirectory() = 0;
	virtual QList<Directory> GetInitialDirs() = 0;
	virtual void AddToNotificationArea() = 0;
	virtual void RemoveFromNotificationArea() = 0;
	virtual bool Execute(QString path, QString args) { 
	    path = path ; args = args; // warning remover
	    return false; 
	}

	virtual bool isAlreadyRunning() = 0;

	QKeySequence oldKey;

	// Set hotkey
	virtual bool SetHotkey(const QKeySequence& key, QObject* receiver, const char* slot) = 0;

	// Need to alter an indexed item?  e.g. .desktop files
	virtual void alterItem(CatItem*) {return;};

	// Alpha border functions	
	virtual bool SupportsAlphaBorder() { return false; }
	virtual bool CreateAlphaBorder(QWidget* w, QString ImageName) { w = w; ImageName = ImageName; return false; }
	virtual void DestroyAlphaBorder() { return; }
	virtual void MoveAlphaBorder(QPoint pos) { pos = pos; return; }
	virtual void HideAlphaBorder() { return; }
	virtual void ShowAlphaBorder() { return; }
	virtual void SetAlphaOpacity(double) { return; }
	//virtual void KillLaunchys() = 0;
	virtual QHash<QString, QList<QString> > GetDirectories() = 0;

	virtual QString expandEnvironmentVars(QString txt)
	{	    
	    QStringList list = QProcess::systemEnvironment();
	    txt.replace('~', "$HOME$");
	    QString delim("$");
	    QString out = "";
	    int curPos = txt.indexOf(delim, 0);
	    if (curPos == -1) return txt;
	    
	    while(curPos != -1) {
		int nextPos = txt.indexOf("$", curPos+1);
		if (nextPos == -1) {
		    out += txt.mid(curPos+1);
		    break;
		}
		QString var = txt.mid(curPos+1, nextPos-curPos-1);
		bool found = false;
		foreach(QString s, list) {
		    if (s.startsWith(var, Qt::CaseInsensitive)) {
			found = true;
			out += s.mid(var.length()+1);
			break;
		    }			
		}
		if (!found)
		    out += "$" + var;
		curPos = nextPos;
	    }
	    return out;
	}


};

 Q_DECLARE_INTERFACE(PlatformBase,
                     "net.launchy.PlatformBase/1.0")

#endif
