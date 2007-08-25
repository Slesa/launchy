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

void PluginHandler::loadPlugins() {
	// Get the list of loadable plugins
	QHash<QString,bool> loadable;
	int size = gSettings->beginReadArray("plugins");
	for(int i = 0; i < size; ++i) {
		gSettings->setArrayIndex(i);
		QString name = gSettings->value("name").toString();
		bool toLoad = gSettings->value("load").toBool();
		loadable[name] = toLoad;
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
			QString name;
			bool handled = plug->msg(MSG_GET_NAME, (QObject*) &name);

			if (handled && (!loadable.contains(name) || loadable[name])) {
				uint hashID = qHash(name);
				PluginInfo info;
				info.name = name;
				info.obj = plug;
				info.path = pluginsDir.absoluteFilePath(fileName);
				if (!plugins.contains(hashID)) {
					plugins[hashID] = info;
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