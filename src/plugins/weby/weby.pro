include(../plugins.pri)
TARGET 			= weby

QT				+= network widgets

CONFIG			+= plugin

VPATH			+= ../../launchy/
INCLUDEPATH		+= ../../launchy/

FORMS			= dlg.ui

HEADERS			= plugin_interface.h \
                weby.h \
                gui.h \
                globals.h \
                ../../common/DropTableWidget.h \
                IconCache.h

SOURCES			= plugin_interface.cpp \
                weby.cpp \
                gui.cpp \
                ../../common/DropTableWidget.cpp \
                IconCache.cpp

TRANSLATIONS	= \
                $${PWD}/../../../translations/weby_fr.ts \
                $${PWD}/../../../translations/weby_nl.ts \
                $${PWD}/../../../translations/weby_zh.ts \
                $${PWD}/../../../translations/weby_es.ts \
                $${PWD}/../../../translations/weby_de.ts \
                $${PWD}/../../../translations/weby_ja.ts \
                $${PWD}/../../../translations/weby_zh_TW.ts \
                $${PWD}/../../../translations/weby_rus.ts

lupdate.target	= lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/weby.pro)
makeqm.target	= makeqm
makeqm.depends	= $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/weby.pro)

QMAKE_EXTRA_TARGETS += lupdate makeqm
PRE_TARGETDEPS	+= lupdate makeqm


win32 { 
    CONFIG		-= embed_manifest_dll
    LIBS		+= user32.lib shell32.lib
}

linux {
    PREFIX		= /usr
    target.path	= $$PREFIX/lib/launchy/plugins/
    icon.path	= $$PREFIX/lib/launchy/plugins/icons/
    icon.files	= weby.png
    INSTALLS	+= target icon
}

macx {
    DESTDIR		= $${PWD}/../../../bin/app/Launchy.app/Contents/MacOS/plugins

    icons.path	 = ../../../app/Launchy.app/Contents/MacOS/plugins/icons/
    icons.files = weby.png
    INSTALLS	+= icons
}
