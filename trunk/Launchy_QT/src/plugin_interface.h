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

#ifdef Q_WS_WIN
#define VC_EXTRALEAN
#ifndef WINVER
#define WINVER 0x05100
#define _WIN32_WINNT 0x0510	
#define _WIN32_WINDOWS 0x0510 
#define _WIN32_IE 0x0600
#endif

#include <windows.h>
#include <shlobj.h>
#endif

#ifndef UNICODE
#define UNICODE
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
#define MSG_LAUNCHY_SHOW 10
#define MSG_LAUNCHY_HIDE 11

#define MSG_CONTROL_OPTIONS 500
#define MSG_CONTROL_EXIT 501
#define MSG_CONTROL_REBUILD 502

/*! \file
    \brief The plugin interface to Launchy
    
*/

/*! \def MAX(a,b)
    \brief A macro that returns the maximum of \a a and \a b.
   
    Details.
*/

/*! \var typedef unsigned int UINT32
    \brief A type definition for a .
    
    Details.
*/

	/** 
		\fn int blah(int MSG_GET_ID, void* blah)
		\param MSG_GET_ID blah
		\param blah halb
		\return blah
		This is a function
	*/

class PluginInterface
{
public:
	virtual ~PluginInterface() {};


	/**
		\brief This is how messages are passed to the plugin
		\param msgId The type of the message, such as show dialog, initialize, or get results
		\param wParam If this message type requires any parameters, this will be the first and casted to void*
		\param lParam If this message requires two parameters, this is the second, casted to void*
		\return 1 means the message was handled, 0 means it was ignored
	*/
	virtual int msg(int msgId, void* wParam = NULL, void* lParam = NULL) = 0;

	/** The setting manager in which you store your persistent data, please refer to the QT manual for QSettings */
	QSettings** settings;
};


void runProgram(QString file, QString args);



 Q_DECLARE_INTERFACE(PluginInterface,
                     "net.launchy.PluginInterface/1.0")



#endif
