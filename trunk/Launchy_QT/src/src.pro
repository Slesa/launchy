TEMPLATE = app
unix {
  TARGET = launchy
}
win32 {
  TARGET = Launchy
}
CONFIG += debug_and_release
#CONFIG += qt release
QT += network

SOURCES = main.cpp \
 globals.cpp \
 options.cpp \
 catalog.cpp \
 catalog_builder.cpp \
 plugin_handler.cpp \
 platform_util.cpp \
 icon_delegate.cpp \
 plugin_interface.cpp \
 catalog_types.cpp

HEADERS = platform_util.h \
 platform_base.h \
 globals.h \
 globals.h \
 main.h \
 catalog.h \
 catalog_builder.h \
 plugin_interface.h \
 plugin_handler.h \
 options.h \
 catalog_types.h \
 icon_delegate.h

ICON = Launchy.ico

first.target = blah

unix {
 FORMS =  options.ui
 PREFIX = /usr
 DEFINES += SKINS_PATH=\\\"$$PREFIX/share/launchy/skins/\\\" \
           PLUGINS_PATH=\\\"$$PREFIX/lib/launchy/plugins/\\\" \
           PLATFORMS_PATH=\\\"$$PREFIX/lib/launchy/\\\"
           
 if(!debug_and_release|build_pass) {
  CONFIG(debug, debug|release) {
   DESTDIR =    ../debug/
  }
  CONFIG(release, debug|release) {
   DESTDIR =    ../release/
  }
 }

 target.path = $$PREFIX/bin/

 skins.path = $$PREFIX/share/launchy/skins/
 skins.files = ../skins/*

 icon.path = $$PREFIX/share/pixmaps
 icon.files = ../misc/Launchy_Icon/launchy_icon.png

 desktop.path = $$PREFIX/share/applications/
 desktop.files = ../linux/launchy.desktop

 INSTALLS += target skins icon desktop
}

win32 {
  INCLUDEPATH += c:/boost/
  FORMS =   options.ui
  RC_FILE =   ../win/launchy.rc
  LIBS +=   shell32.lib
  if(!debug_and_release|build_pass) {
   CONFIG(debug, debug|release) {
    DESTDIR =     ../debug/
   }
   CONFIG(release, debug|release) {
    CONFIG +=     embed_manifest_exe
   }
   DESTDIR =    ../release/
  }
}

TRANSLATIONS =  ../translations/launchy_fr.ts  ../translations/launchy_nl.ts  \
		    ../translations/launchy_zh.ts ../translations/launchy_es.ts \
		    ../translations/launchy_de.ts ../translations/launchy_ja.ts
OBJECTS_DIR = build
MOC_DIR = build
#UI_DIR = build

