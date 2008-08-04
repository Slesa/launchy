#include <QPluginLoader>
#include <QDebug>
#include "plugin_interface.h"
#include "platform_util.h"
#include "globals.h"
#include "main.h"


PlatformBase * loadPlatform()
{
    QList<QString> files;
    QString file;

    #ifdef Q_WS_WIN
    files += "platform_win.dll";
    #endif
    #ifdef Q_WS_X11
    files += "libplatform_unix.so";
    files += QString(PLATFORMS_PATH) + "/libplatform_unix.so";
    #endif

    /*
    int desktop = getDesktop();
    if (desktop == DESKTOP_WINDOWS)
	files += "/platform_win.dll";
    else if (desktop == DESKTOP_GNOME) {
	files += "libplatform_unix.so";
	//	files += "libplatform_gnome.so";
	//files += QString(PLATFORMS_PATH) + QString("/libplatform_gnome.so");
    }
    else if (desktop == DESKTOP_KDE) {
	files += "libplatform_kde.so";
	files += QString(PLATFORMS_PATH) + "/libplatform_kde.so";
    }
    */

    QObject * plugin = NULL;
    foreach(QString file, files) {
	QPluginLoader loader(file);
	plugin = loader.instance();

	if (plugin) break;
    }
    
    if (!plugin)
	{
	    qDebug() << "Could not load platform file!";
	    exit(1);
	}
    
    return qobject_cast<PlatformBase*>(plugin);
}
