TEMPLATE            = lib
TARGET              = launchy.qxt

CONFIG          += qt_warn debug_and_release staticlib qpa
#unix:!macx:CONFIG      += qpa
#win:CONFIG      += qpa

# CLEAN_TARGET     = QxtWidgets
DEFINES	         += BUILD_QXT_GUI
win32:DEFINES   += UNICODE _UNICODE

QT               = core gui core-private gui-private
win32:QT        += winextras
#unix:!macx:QT        += gui-private x11extras
macx:QT          += macextras


isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets
}

QXT              = core
CONVENIENCE     += $$CLEAN_TARGET

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS	+= \
    qxtglobal.h \
    qxtglobalshortcut.h \
    qxtglobalshortcut_p.h

SOURCES	+= \
    qxtglobalshortcut.cpp

unix:!macx:SOURCES += \
    qxtglobalshortcut_x11.cpp

macx:SOURCES += \
    qxtglobalshortcut_mac.cpp

win32:SOURCES += \
    qxtglobalshortcut_win.cpp

contains(DEFINES,HAVE_XRANDR){
    !win32:LIBS += -lXrandr
}

win32:LIBS      += -luser32
macx:LIBS       += -framework Carbon


DESTDIR         = $${PWD}../../../bin/lib/

# Debian and derivatives pass --no-undefined to the linker, which
# means that each library must explicitly link to all dependencies 
# than assuming that the application or another library will bring
# in the necessary symbols at run time.
contains(QMAKE_LFLAGS, "-Wl,--no-undefined"):LIBS += $${QMAKE_LIBS_X11}

unix|win32-g++*:QMAKE_PKGCONFIG_REQUIRES += QxtCore QtGui

unix {
    PREFIX      = /usr
}
