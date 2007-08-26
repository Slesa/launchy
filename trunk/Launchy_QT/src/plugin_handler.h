#ifndef PLUGIN_HANDLER
#define PLUGIN_HANDLER

#include <QPluginLoader>
#include <QHash>
#include "plugin_interface.h"
#include "catalog.h"
#include "plugin_info.h"

struct PluginInfo {
	uint id;
	QString name;
	QString path;
	PluginInterface* obj;
	bool loaded;

	~PluginInfo() {
		QPluginLoader loader(path);
		loader.unload();
	}
};


class PluginHandler {
private:
	QHash<uint, PluginInfo> plugins;

public:

	PluginHandler();
	~PluginHandler();

	void loadPlugins();
	void getLabels(QList<InputData>* id);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalogs(QList<CatItem>* items);
	void execute(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent, uint id);
	QHash<uint, PluginInfo> & getPlugins() { return plugins; }
};



#endif