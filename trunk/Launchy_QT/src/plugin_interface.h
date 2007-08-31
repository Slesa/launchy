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

#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QObject>
#include <QSettings>
#include "catalog.h"

#ifdef WIN32
#define VC_EXTRALEAN
#define WINVER 0x05100
#define _WIN32_WINNT 0x0510	
#define _WIN32_WINDOWS 0x0510 
#define _WIN32_IE 0x0600

#include <windows.h>
#include <shlobj.h>
#endif

#ifndef _UNICODE
#define _UNICODE
#endif


#define MSG_GET_ID 0
#define MSG_GET_LABELS 1
#define MSG_GET_RESULTS 2
#define MSG_GET_CATALOG 3
#define MSG_LAUNCH_ITEM 4
#define MSG_INIT 5
#define MSG_HAS_DIALOG 6
#define MSG_DO_DIALOG 7
#define MSG_GET_NAME 8
#define MSG_END_DIALOG 9


class PluginInterface
{
public:
	virtual ~PluginInterface() {};
	virtual bool msg(int msgId, void* wParam = NULL, void* lParam = NULL) = 0;
	QSettings** settings;
};


void runProgram(QString file, QString args);



 Q_DECLARE_INTERFACE(PluginInterface,
                     "net.launchy.PluginInterface/1.0")



#endif