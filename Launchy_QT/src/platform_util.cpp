#include <QPluginLoader>

#include "platform_util.h"

PlatformBase * loadPlatform()
{
	QString file;
#ifdef Q_WS_WIN
	file = "platform_win.dll";
#endif
#ifdef Q_WS_MAC
	file = "platform_mac.dll";
#endif
#ifdef Q_WS_X11
	file = "platform_kde.dll";
#endif
	QPluginLoader loader(file);
	QObject *plugin = loader.instance();
	if (!plugin)
		exit(1);
	return qobject_cast<PlatformBase*>(plugin);
}
