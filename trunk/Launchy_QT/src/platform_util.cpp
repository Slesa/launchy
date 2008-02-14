#include <QPluginLoader>
#include <QDebug>
#include "plugin_interface.h"
#include "platform_util.h"

PlatformBase * loadPlatform()
{
	QString file;
	int desktop = getDesktop();
	if (desktop == DESKTOP_WINDOWS)
	    file = "platform_win.dll";
	else if (desktop == DESKTOP_GNOME)
	    file = "libplatform_gnome.so";
	else if (desktop == DESKTOP_KDE)
	    file = "libplatform_kde.so";
	QPluginLoader loader(file);
	QObject *plugin = loader.instance();

	if (!plugin)
		exit(1);

	return qobject_cast<PlatformBase*>(plugin);
}
