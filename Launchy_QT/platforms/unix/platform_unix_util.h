#pragma once
#include <QtGui>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrender.h>


#include <QFileIconProvider>

#include <QWidget>

class AlphaBorder : public QWidget
{
    Q_OBJECT    
private:
    public:
QString alphaFile;

 AlphaBorder(QWidget * parent, QString);
 ~AlphaBorder();
 QWidget* p;
 QPoint moveStartPoint;

    void paintEvent(QPaintEvent *);
    void SetAlphaOpacity(double trans);
    
    //    QPaintEngine * paintEngine() { return 0; }
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

    GC			gc;	// Target Painter
    GC                  gcback;
    XImage 		*xmask;	// Image informations
    XImage              *buffer;
    XGCValues		values;	// Setup GC
    uint 		width;
    uint 		height;

    void contextMenuEvent(QContextMenuEvent* evt);
};



class UnixIconProvider : public QFileIconProvider
{
 private:
    QHash<QString, QString> file2mime;
    QHash<QString, QString> mime2desktop;
    QHash<QString, QString> desktop2icon;
    QHash<QString, QString> icon2path;
    QStringList xdgDataDirs;
 public:
    UnixIconProvider();
    ~UnixIconProvider() {}
    QIcon getIcon(const QFileInfo& info);
    QString getDesktopIcon(QString desktopFile, QString IconName = "");
};

