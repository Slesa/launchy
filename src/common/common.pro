TEMPLATE		= lib
TARGET			= launchy.common
CONFIG			+= qt_warn debug_and_release staticlib
QT				+= widgets
win32:QT		+= winextras
linux:QT    	+= x11extras
mac:QT			+= macextras

HEADERS 		= \
                catalog.h \
                globals.h \
                platform_base.h \
                globalshortcutmanager.h \
                globalshortcuttrigger.h \
                settingsmanager.h
linux:HEADERS	+= \
                directory.h \
                x11_iconprovider.h \
                x11_platform.h \
				x11_keytrigger.h
win32:HEADERS	+= \
                win_platform.h
mac:HEADERS		+= \
                mac_platform.h \
                mac_keytrigger.h

SOURCES 		= \
                catalog.cpp \
                globals.cpp \
                platform_base.cpp \
                globalshortcutmanager.cpp \
                settingsmanager.cpp
linux:SOURCES	+= x11.cpp \
                x11_iconprovider.cpp \
                x11_platform.cpp \
				x11_keytrigger.cpp
win32:SOURCES	+= win.cpp \
                win_platform.cpp \
                win_keytrigger.cpp
mac:SOURCES		+= mac.cpp \
                mac_platform.cpp \
                mac_keytrigger.cpp

DESTDIR 		= ../../lib/

unix {
    PREFIX		= /usr
    DEFINES		+= SKINS_PATH=\\\"$$PREFIX/share/launchy/skins/\\\" \
                PLUGINS_PATH=\\\"$$PREFIX/lib/launchy/plugins/\\\" \
                PLATFORMS_PATH=\\\"$$PREFIX/lib/launchy/\\\"}

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#        DESTDIR = ../../lib/
#}

#if(!debug_and_release|build_pass):CONFIG(release, debug|release) {
#        DESTDIR = ../../lib/
#}

