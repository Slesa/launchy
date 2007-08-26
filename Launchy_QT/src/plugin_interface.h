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