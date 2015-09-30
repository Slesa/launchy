include(../plugins.pri)
TARGET			= controly

QT				+= widgets
win32:QT		+= winextras

CONFIG			+= plugin
DEFINES			+= WITH_GUI

VPATH			+= ../../launchy \
                ../../common
INCLUDEPATH		+= ../../launchy

FORMS			= dlg.ui

HEADERS			= plugin_interface.h \
                controly.h \
                gui.h \
                ControlPanelItemFinder.h \
                fhoicon.h \
                fhoenv.h \
                fhoreg.h \
                fhores.h

SOURCES			= plugin_interface.cpp \
                controly.cpp \
                gui.cpp \
                ControlPanelItemFinder.cpp \
                fhoicon.cpp \
                fhoenv.cpp \
                fhoreg.cpp \
                fhores.cpp

TRANSLATIONS	= \
                $${PWD}/../../../translations/controly_fr.ts \
                $${PWD}/../../../translations/controly_nl.ts \
                $${PWD}/../../../translations/controly_zh.ts \
                $${PWD}/../../../translations/controly_es.ts \
                $${PWD}/../../../translations/controly_de.ts \
                $${PWD}/../../../translations/controly_ja.ts \
                $${PWD}/../../../translations/controly_zh_TW.ts \
                $${PWD}/../../../translations/controly_rus.ts

lupdate.target	= lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/controly.pro)
makeqm.target	= makeqm
makeqm.depends	= $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/controly.pro)

QMAKE_EXTRA_TARGETS += lupdate makeqm
PRE_TARGETDEPS	+= lupdate makeqm

win32 { 
    CONFIG		-= embed_manifest_dll
    LIBS		+= shell32.lib \
                user32.lib \
                Gdi32.lib \
                comctl32.lib \
                Advapi32.lib \
                ole32.lib \
                shlwapi.lib
	QMAKE_CXXFLAGS_RELEASE += /Zi
	QMAKE_LFLAGS_RELEASE += /DEBUG
# disable optimizations to prevent crashes with certain third party control panel
# applets when Controly is built using VC++ 2005.
	QMAKE_CXXFLAGS_RELEASE -= -O2
}
