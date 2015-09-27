TEMPLATE		= lib
TARGET			= launchy.common

CONFIG			+= qt_warn debug_and_release staticlib
#win:CONFIG		+= qpa
QT				+= widgets
win32:QT		+= winextras gui-private
#mac:QT			+= macextras

win32:DEFINES	+= UNICODE _UNICODE
INCLUDEPATH		+= ../

HEADERS 		= \
                catalog.h \
                globals.h \
                platform_base.h \
                settingsmanager.h
linux:HEADERS	+= \
                directory.h \
                x11_iconprovider.h \
                x11_platform.h
win32:HEADERS	+= \
                win_iconprovider.h \
                win_files.h \
                win_platform.h \
                win_minidump.h \
                win_util.h
mac:HEADERS		+= \
                mac_platform.h

SOURCES 		= \
                catalog.cpp \
                globals.cpp \
                platform_base.cpp \
                settingsmanager.cpp
linux:SOURCES	+= x11.cpp \
                x11_iconprovider.cpp \
                x11_platform.cpp
win32:SOURCES	+= win.cpp \
                win_iconprovider.cpp \
                win_platform.cpp \
                win_minidump.cpp \
                win_util.cpp
mac:SOURCES		+= mac.cpp \
                mac_platform.cpp

DESTDIR 		= $${PWD}../../../bin/lib/

TRANSLATIONS	= \
                ../../translations/common_fr.ts \
                ../../translations/common_nl.ts \
                ../../translations/common_zh.ts \
                ../../translations/common_es.ts \
                ../../translations/common_de.ts \
                ../../translations/common_ja.ts \
                ../../translations/common_zh_TW.ts \
                ../../translations/common_rus.ts

lupdate.target	= lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/common.pro)
makeqm.target	= makeqm
makeqm.depends	= $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/common.pro)

QMAKE_EXTRA_TARGETS += lupdate makeqm
PRE_TARGETDEPS	+= lupdate makeqm

unix {
    PREFIX		= /usr
    DEFINES		+= SKINS_PATH=\\\"$$PREFIX/share/launchy/skins/\\\" \
                PLUGINS_PATH=\\\"$$PREFIX/lib/launchy/plugins/\\\" \
                PLATFORMS_PATH=\\\"$$PREFIX/lib/launchy/\\\"
}

