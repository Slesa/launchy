#include "plugin_handler.h"

#include <QLibrary>
#include <QDir>
#include "globals.h"
#include "main.h"

PluginHandler::PluginHandler() {

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
			bool handled = false;
			QVariant nameV = plug->msg(MSG_GET_NAME, handled);
			QString name;
			if (handled)
				name = nameV.toString();

			if (handled && (!loadable.contains(name) || loadable[name])) {
				int hashID = qHash(name);
				PluginInfo info;
				info.name = name;
				info.obj = plug;
				info.path = pluginsDir.absoluteFilePath(fileName);
				if (!plugins.contains(qHash(name))) {
					plugins[qHash(name)] = info;
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