#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QObject>
#include "catalog.h"



#define MSG_GET_ID 0
#define MSG_GET_LABELS 1
#define MSG_GET_RESULTS 2
#define MSG_GET_CATALOG 3
#define MSG_LAUNCH_ITEM 4



class PluginInterface
{
public:
	virtual ~PluginInterface() {};
	virtual bool msg(int msgId, void* wParam = NULL, void* lParam = NULL) = 0;
};




 Q_DECLARE_INTERFACE(PluginInterface,
                     "net.launchy.PluginInterface/1.0")



#endif