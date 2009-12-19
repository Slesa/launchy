#include "platform_unix_util.h"


#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include <QPainter>
#include <QProcess>
#include <QX11Info>

//#include "../../src/main.h"

AlphaBorder::AlphaBorder(QWidget * parent, QString file) : 
    QWidget(NULL, Qt::SplashScreen | Qt::Tool | Qt::FramelessWindowHint)
    //QWidget(NULL,  Qt::Tool | Qt::FramelessWindowHint)
{

    p = parent;


    //    stackUnder(parent);
    //setFocusPolicy(Qt::NoFocus);
    //    setFocusProxy(parent);
    // This stops a bunch of XCopyArea problems
    // Note that you need to override and return 0 for paintEngine()
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    //    setEnabled(false);
    
    Display *d;
    int s;
    Window w;
    Colormap colormap = NULL;
    Visual * visual = NULL;
    
    
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
    //    XWindowAttributes wa;
    //    Status success = XGetWindowAttributes(d, w, &wa);
    
    
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
void AlphaBorder::mousePressEvent(QMouseEvent * event) {
    moveStartPoint = event->pos();
    if (underMouse()) {
	p->activateWindow();
	p->raise();	
    }
}

void AlphaBorder::mouseMoveEvent(QMouseEvent *e)
{    
    QPoint px = e->globalPos();
    px -= moveStartPoint;
    move(px);
    p->move(px);    
}
*/


void AlphaBorder::SetAlphaOpacity(double trans)
{
    setWindowOpacity(trans);
}


void AlphaBorder::paintEvent(QPaintEvent * e)
{
    QRect rect = e->rect();
    int l=rect.left();
    int t=rect.top();
    int w=rect.width();
    int h=rect.height();
    
    XPutImage(QX11Info::display(), winId(), gc, xmask, l,t,l,t,w,h);
}



AlphaBorder::~AlphaBorder()
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

UnixIconProvider::UnixIconProvider() {
    foreach(QString line, QProcess::systemEnvironment()) {
	if (!line.startsWith("XDG_DATA_DIRS", Qt::CaseInsensitive))
	    continue;
	QStringList spl = line.split("=");
	xdgDataDirs = spl[1].split(":");	
    }
}


QIcon UnixIconProvider::getIcon(const QFileInfo& info)
{
    QString name = info.fileName();

    if (name.endsWith(".png", Qt::CaseInsensitive))
	return QIcon(info.absoluteFilePath());    
    if (name.endsWith(".ico", Qt::CaseInsensitive))
	return QIcon(info.absoluteFilePath());
    if (!name.contains("."))
	return icon(QFileIconProvider::File);


    QString end = name.mid(name.lastIndexOf(".")+1);
    if (!file2mime.contains(end.toLower())) {
	
	QProcess proc;
	
	QStringList args;
	args += "query";
	args += "filetype";
	args += info.absoluteFilePath();
	proc.setReadChannel(QProcess::StandardOutput);
	proc.start(QString("xdg-mime"), args);
	proc.waitForFinished(10000);
	QString mimeType = proc.readAll().trimmed();
	proc.close();
	file2mime.insert(end.toLower(), mimeType);
    }

    QString mimeType = file2mime[end.toLower()];

    if (!mime2desktop.contains(mimeType)) {
	QProcess proc2;
	QStringList args;
	args += "query";
	args += "default";
	args += mimeType;
	proc2.start(QString("xdg-mime"),args);
	proc2.waitForFinished(10000);
	QString desk = proc2.readAll().trimmed();
	proc2.close();
	
	mime2desktop[mimeType] = desk;
    }

    
    QString desktop = mime2desktop[mimeType];

    if (desktop == "")
	return icon(QFileIconProvider::File);

    return QIcon(getDesktopIcon(desktop));
}

QString UnixIconProvider::getDesktopIcon(QString desktopFile, QString IconName) {    
    if (QFile::exists(desktopFile)) 
	desktopFile = desktopFile.mid(desktopFile.lastIndexOf("/")+1);	
    
    if (desktop2icon.contains(desktopFile) && IconName == "")
	IconName = desktop2icon[desktopFile];    
    if (IconName == "") {
	const char *dirs[] = { "/usr/share/applications/",
			       "/usr/local/share/applications/",
			       "/usr/share/gdm/applications/",
			       "/usr/share/applications/kde/",
			       "~/.local/share/applications/"};
	for(int i = 0; i < 5; i++) {
	    QString dir = dirs[i];
	    QString path = dir + "/" + desktopFile;

	    if (QFile::exists(path)) {
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		    return "";
		while(!file.atEnd()) {
		    QString line = file.readLine();
		    if (line.startsWith("Icon", Qt::CaseInsensitive)) {
			desktop2icon[desktopFile] = line.split("=")[1].trimmed();
		    }
		}
		break;
	    }
	}
    }

    if (IconName == "")
	IconName = desktop2icon[desktopFile];

    if (IconName == "")
	return "";
    
    // Find the icon path
    QString iconPath;
    if (icon2path.contains(IconName)) {
	iconPath = icon2path[IconName];
    }
    else if (QFile::exists(IconName)) {
	iconPath = IconName;
    }
    else {
	QStringList inames;
	if (IconName.endsWith(".png") || IconName.endsWith(".xpm") || IconName.endsWith(".svg"))
	    inames += IconName;
	else {
	    inames += IconName + ".png";
	    inames += IconName + ".xpm";
	    inames += IconName + ".svg";
	}


	bool ifound = false;
	QStringList themes;
	themes += "/hicolor/32x32";
	themes += "/hicolor/48x48";
	themes += "/hicolor/64x64";
	
	QStringList dirs;	
	dirs += QDir::homePath() + "/.icons" + themes[0];

	foreach(QString dir, xdgDataDirs) {
	    foreach(QString thm, themes) {
		dirs += dir + "/icons" + thm;
	    }
	}

	dirs += "/usr/share/pixmaps";

	
	foreach(QString dir, dirs) {
	    QDir d(dir);
	    QStringList sdirs;
	    if (!dir.endsWith("pixmaps"))
		sdirs = d.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	    sdirs += "."; 
	    
	    foreach (QString subdir, sdirs) {
		foreach(QString iname, inames) {
		    if (QFile::exists(dir + "/" + subdir + "/" +  iname)) {
			iconPath = dir + "/" + subdir + "/" + iname;
			icon2path[IconName] = iconPath;
			ifound = true;
			break;
		    }
		}
	    }
	    if (ifound)
		break;
	}
    }	


    return iconPath;
}
