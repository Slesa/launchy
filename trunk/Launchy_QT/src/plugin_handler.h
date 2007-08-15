#ifndef PLUGIN_HANDLER
#define PLUGIN_HANDLER

#include <QPluginLoader>
#include <QHash>
#include "plugin_interface.h"

class PluginHandler {
public:
	struct PluginInfo {
		QString name;
		QString path;
		PluginInterface* obj;

		~PluginInfo() {
			QPluginLoader loader(path);
			loader.unload();
		}
	};

public:
	QHash<int, PluginInfo> plugins;

	PluginHandler();
	~PluginHandler();

	void loadPlugins();

};



#endif