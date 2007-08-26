#include "plugin_handler.h"

#include <QLibrary>
#include <QDir>
#include "globals.h"
#include "main.h"


PluginHandler::PluginHandler() {

}

void PluginHandler::getLabels(QList<InputData>* id) 
{
	if (id->count() == 0) return;
	foreach(PluginInfo info, plugins) {
		info.obj->msg(MSG_GET_LABELS, (void*) id);
	}
}

void PluginHandler::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	if (id->count() == 0) return;
	foreach(PluginInfo info, plugins) {
		info.obj->msg(MSG_GET_RESULTS, (void*) id, (void*) results);
	}
}

void PluginHandler::getCatalogs(QList<CatItem>* items) {
	foreach(PluginInfo info, plugins) {
		info.obj->msg(MSG_GET_CATALOG, (void*) items);
	}
}

void PluginHandler::execute(QList<InputData>* id, CatItem* result) {
	if (!plugins.contains(result->id)) return;
	plugins[result->id].obj->msg(MSG_LAUNCH_ITEM, (void*) id, (void*) result);
}

void PluginHandler::loadPlugins() {
	// Get the list of loadable plugins
	QHash<uint,bool> loadable;
	int size = gSettings->beginReadArray("plugins");
	for(int i = 0; i < size; ++i) {
		gSettings->setArrayIndex(i);
		uint id = gSettings->value("id").toUInt();
		bool toLoad = gSettings->value("load").toBool();
		loadable[id] = toLoad;
	}
	gSettings->endArray();

	// Load up the plugins in the plugins/ directory
	QDir pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/");
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		if (!QLibrary::isLibrary(fileName)) continue;
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) {
			PluginInterface *plug = qobject_cast<PluginInterface *>(plugin);
			uint id;
			bool handled = plug->msg(MSG_GET_ID, (void*) &id);

			if (handled && (!loadable.contains(id) || loadable[id])) {
				PluginInfo info;
//				info.name = name;
				info.obj = plug;
				info.path = pluginsDir.absoluteFilePath(fileName);
				if (!plugins.contains(id)) {
					plugins[id] = info;
				}
			} else {
				// unload
				loader.unload();
			}
		}
	}	
}

PluginHandler::~PluginHandler() {

}