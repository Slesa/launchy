
#include <libgnomeui/libgnomeui.h>
#include "platform_gnome_util.h"
#include <gtk/gtk.h>


#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include <QPainter>

#include <QX11Info>


GnomeAlphaBorder::GnomeAlphaBorder(QWidget * parent, QString file) : 
    QWidget(NULL, Qt::Tool | Qt::FramelessWindowHint)
{
    p = parent;
    // This stops a bunch of XCopyArea problems
    // Note that you need to override and return 0 for paintEngine()
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setEnabled(false);
    
    Display *d;
    int s;
    Window w;
    XEvent e;
    Colormap colormap;
    Visual * visual;
    
    
    //    setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    
    
    QX11Info info;
    d = info.display();
    s = info.screen();
    
    
    int event_base, error_base;
    bool argb_visual = false;
    XVisualInfo * xvi;
    int index = 0;
    
    if (  XRenderQueryExtension( d, &event_base, &error_base ) )
        {
            int nvi;
	    
            XVisualInfo templ;
            templ.screen  = s;
            templ.depth   = 32;
            templ.c_class = TrueColor;
	    
            xvi = XGetVisualInfo( d, VisualScreenMask | VisualDepthMask
                                  | VisualClassMask, &templ, &nvi );
	    
            for ( int i = 0; i < nvi; i++ ) {
                XRenderPictFormat *format = XRenderFindVisualFormat( d, xvi[i].visual );
                if ( format->type == PictTypeDirect && format->direct.alphaMask ) {
                    visual = xvi[i].visual;
		    
                    index = i;
                    colormap = XCreateColormap( d, info.appRootWindow(info.screen()), visual, AllocNone );
                    argb_visual=true;
                    break;
                }
            }
        }
    
    alphaFile = file;
    QImage img;
    img.load(alphaFile.toLocal8Bit().data());
    img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    width = img.width();
    height = img.height();
    


    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.background_pixel = WhitePixel(d,s);
    swa.border_pixel = BlackPixel(d,s);
    

    w = XCreateWindow(d, info.appRootWindow(info.screen()) , 10, 10, width, height, 0, 32,
                      InputOutput, visual,
                      CWBackPixel|CWBorderPixel|CWColormap,
                      &swa);
    /*
      w = XCreateWindow(d, parent->winId() , 10, 10, 100, 500, 0, 32,
      InputOutput, visual,
      CWBackPixel|CWBorderPixel|CWColormap,
      &swa);
    */
    
    /*
      w = XCreateWindow(d, RootWindow(d,s), 10, 10, 100, 500, 0, 32,
      InputOutput, visual,
      CWBackPixel|CWBorderPixel|CWColormap, &swa);
    */
    XWindowAttributes wa;
    Status success = XGetWindowAttributes(d, w, &wa);
    
    
    create(w, true, true);
    
    
    

    //    values.foreground = 0x00000000L;
    //values.background = 0x00000000L;
    
    
    
    gc = XCreateGC (info.display(), w, 0,0);//GCForeground | GCBackground, &values); // drawing content
    
    resize(width,height);
    
    
    xmask = XCreateImage(info.display(), visual, 
                         32, ZPixmap, 0, NULL, width, height, 32, 0);
    
    
    int len = width * height * sizeof(unsigned int);
    
    char *ColorBuffer = (char*) malloc(len);
    memcpy(ColorBuffer, img.bits(), len);
    
    xmask->data=ColorBuffer;
}

/*
  void GnomeAlphaBorder::mousePressEvent(QMouseEvent * event) {
  if (underMouse()) {
  qDebug() << "Under mouse!";
  //p->activateWindow();
  stackUnder(p);
  //        lower();
  }
  }
*/
void GnomeAlphaBorder::SetAlphaOpacity(double trans)
{
    setWindowOpacity(trans);
}


void GnomeAlphaBorder::paintEvent(QPaintEvent * e)
{
    QRect rect = e->rect();
    int l=rect.left();
    int t=rect.top();
    int w=rect.width();
    int h=rect.height();
    
    XPutImage(QX11Info::display(), winId(), gc, xmask, l,t,l,t,w,h);
    
    //    XPutImage(QX11Info::display(), winId(), gc, xmask, 0,0,0,0,width,height);
}



GnomeAlphaBorder::~GnomeAlphaBorder()
{
    
    
    /*
      qDebug() << "Destroying alpha border";
      XDestroyWindow(QX11Info::display(),winId());
      XFreeGC(QX11Info::display(), gc);
      // Frees the data as well
      XDestroyImage(xmask);
    */
    
    if (xmask->data)
        free(xmask->data);
}

QIcon GnomeIconProvider::icon(const QFileInfo& info) const
{
    if (info.fileName().endsWith(".png", Qt::CaseInsensitive))
        {
            return QIcon(info.absoluteFilePath());
        }
    gdk_threads_enter();
    GnomeIconLookupResultFlags resultFlags;
    char* file = gnome_icon_lookup_sync(gtk_icon_theme_get_default(),
                                        NULL,
                                        info.absoluteFilePath().toLocal8Bit().data(),
                                        NULL,
                                        GNOME_ICON_LOOKUP_FLAGS_NONE,
                                        &resultFlags);
    GtkIconInfo* icinfo = gtk_icon_theme_lookup_icon(gtk_icon_theme_get_default(),
                                                     file,
                                                     32,
                                                     GTK_ICON_LOOKUP_NO_SVG);
    
    GdkPixbuf* buff = gtk_icon_info_load_icon(icinfo, NULL);
    gchar* pixmap;
    gsize buflen;
    
    gdk_pixbuf_save_to_buffer (buff,
                               &pixmap,
                               &buflen,
                               "png",
                               NULL, NULL);
    QPixmap qp;
    qp.loadFromData((const uchar*) pixmap, buflen, "png");
    QIcon qico(qp);
    free(pixmap);
    g_object_unref(buff);
    gdk_threads_leave();
    
    return qp;
}
