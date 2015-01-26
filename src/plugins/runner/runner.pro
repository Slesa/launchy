TEMPLATE		= lib
TARGET			= runner

QT				+= widgets

CONFIG			+= plugin debug_and_release

VPATH			+= ../../launchy \
                ../../common
INCLUDEPATH		+= ../../launchy \
                ../../common

UI_DIR			= .ui
MOC_DIR			= .moc

FORMS			= dlg.ui

HEADERS			= plugin_interface.h \
                runner.h \
                gui.h \
                globals.h \
                ../../common/FileBrowserDelegate.h \
                ../../common/FileBrowser.h \
                ../../common/DropTableWidget.h

SOURCES			= plugin_interface.cpp \
                runner.cpp \
                gui.cpp \
                ../../common/FileBrowser.cpp \
                ../../common/FileBrowserDelegate.cpp \
                ../../common/DropTableWidget.cpp

win32 { 
    CONFIG		-= embed_manifest_dll
    LIBS		+= user32.lib shell32.lib
	QMAKE_CXXFLAGS_RELEASE += /Zi
	QMAKE_LFLAGS_RELEASE += /DEBUG
}
 
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) DESTDIR = ../../debug/plugins
if(!debug_and_release|build_pass):CONFIG(release, debug|release) DESTDIR = ../../release/plugins

linux {
    PREFIX		= /usr
    target.path	= $$PREFIX/lib/launchy/plugins/
    icon.path	= $$PREFIX/lib/launchy/plugins/icons/
    icon.files	= runner.png
    INSTALLS	+= target icon
}

macx {
    if(!debug_and_release|build_pass):CONFIG(debug, debug|release):DESTDIR = ../../debug/Launchy.app/Contents/MacOS/plugins
    if(!debug_and_release|build_pass):CONFIG(release, debug|release):DESTDIR = ../../release/Launchy.app/Contents/MacOS/plugins

    CONFIG(debug, debug|release):icons.path = ../../debug/Launchy.app/Contents/MacOS/plugins/icons/
    CONFIG(release, debug|release):icons.path = ../../release/Launchy.app/Contents/MacOS/plugins/icons/

    icons.files	= runner.png
    INSTALLS	+= icons
    INCLUDEPATH	+= /opt/local/include/
}
