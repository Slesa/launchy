#ifndef PLATFORM_GNOME_UTIL_H
#define PLATFORM_GNOME_UTIL_H
//#include <qdatastream.h>
//#include <
#include <QtGui>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrender.h>


#include <QFileIconProvider>

#include <QWidget>

class GnomeAlphaBorder : public QWidget
{
    Q_OBJECT    
private:
    public:
QString alphaFile;

 GnomeAlphaBorder(QWidget * parent, QString);
 ~GnomeAlphaBorder();
    void paintEvent(QPaintEvent *);
    void SetAlphaOpacity(double trans);
    //    QPaintEngine * paintEngine() { return 0; }
    GC			gc;	// Target Painter
    GC                  gcback;
    XImage 		*xmask;	// Image informations
    XImage              *buffer;
    XGCValues		values;	// Setup GC
    uint 		width;
    uint 		height;
};

class GnomeIconProvider : QFileIconProvider
{
 public:
    GnomeIconProvider() {}
    ~GnomeIconProvider() {}
    QIcon icon(const QFileInfo& info) const;
};


#endif
