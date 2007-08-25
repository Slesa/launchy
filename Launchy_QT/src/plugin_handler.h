#ifndef PLUGIN_HANDLER
#define PLUGIN_HANDLER

#include <QPluginLoader>
#include <QHash>
#include "plugin_interface.h"
#include "catalog.h"

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
	QHash<uint, PluginInfo> plugins;

	PluginHandler();
	~PluginHandler();

	void loadPlugins();
	void getLabels(QList<InputData>* id);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
};



#endif