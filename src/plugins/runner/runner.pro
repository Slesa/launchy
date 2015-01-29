include(../plugins.pri)
TARGET			= runner

QT				+= widgets

CONFIG			+= plugin debug_and_release

VPATH			+= ../../launchy \
                ../../common
INCLUDEPATH		+= ../../launchy \
                ../../common

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

DESTDIR			= $${PWD}/../../../bin/app/plugins

TRANSLATIONS	= \
                ../../../translations/runner_fr.ts \
                ../../../translations/runner_nl.ts \
                ../../../translations/runner_zh.ts \
                ../../../translations/runner_es.ts \
                ../../../translations/runner_de.ts \
                ../../../translations/runner_ja.ts \
                ../../../translations/runner_zh_TW.ts \
                ../../../translations/runner_rus.ts

lupdate.target	= lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/runner.pro)
makeqm.target	= makeqm
makeqm.depends	= $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/runner.pro)

QMAKE_EXTRA_TARGETS += lupdate makeqm
PRE_TARGETDEPS	+= lupdate makeqm


win32 { 
    CONFIG		-= embed_manifest_dll
    LIBS		+= user32.lib shell32.lib
	QMAKE_CXXFLAGS_RELEASE += /Zi
	QMAKE_LFLAGS_RELEASE += /DEBUG
}
 
linux {
    PREFIX		= /usr
    target.path	= $$PREFIX/lib/launchy/plugins/
    icon.path	= $$PREFIX/lib/launchy/plugins/icons/
    icon.files	= runner.png
    INSTALLS	+= target icon
}

macx {
    DESTDIR		= $${PWD}/../../../bin/app/Launchy.app/Contents/MacOS/plugins

    icons.path	= ../../../app/Launchy.app/Contents/MacOS/plugins/icons/
    icons.files	= runner.png
    INSTALLS	+= icons
}
