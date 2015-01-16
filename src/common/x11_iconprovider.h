#ifndef X11ICONPROVIDER_H
#define X11ICONPROVIDER_H

#include <QtGui>
#include <QFileIconProvider>
#include <QWidget>

class X11IconProvider : public QFileIconProvider
{
 private:
    QHash<QString, QString> file2mime;
    QHash<QString, QString> mime2desktop;
    QHash<QString, QString> desktop2icon;
    QHash<QString, QString> icon2path;
    QStringList xdgDataDirs;
 public:
    X11IconProvider();
    ~X11IconProvider() {}
    virtual QIcon icon(const QFileInfo& info);
    QString getDesktopIcon(QString desktopFile, QString IconName = "");
};

#endif
