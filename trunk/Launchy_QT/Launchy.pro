TEMPLATE	= app
CONFIG		+= qt_warn debug_and_release
TARGET = launchy


QT			+= network
TARGET		= Launchy
VPATH		+= src/
UI_DIR		= ../src/
FORMS		= ../src/options.ui
SOURCES		= main.cpp globals.cpp  \ 
			  options.cpp catalog.cpp catalog_builder.cpp \ 
			   plugin_handler.cpp platform_util.cpp\
			  icon_delegate.cpp plugin_interface.cpp catalog_types.cpp
HEADERS		= platform_util.h platform_base.h globals.h \
			  globals.h main.h \ 
			   catalog.h \
			  catalog_builder.h  \ 
			  plugin_interface.h plugin_handler.h options.h catalog_types.h \
			  icon_delegate.h
ICON		= Launchy.ico


win32 {
	RC_FILE = win/launchy.rc
	LIBS += shell32.lib
}

debug {
    CONFIG -= release
	DESTDIR = debug/
}

release {
    CONFIG -= debug
	DESTDIR = release/
}

win32:debug {
%	CONFIG += console 
}

win32:release {
	CONFIG += embed_manifest_exe
}


unix {
target.path = /usr/bin
INSTALLS += target

skins.path = /usr/share/launchy/
skins.files = skins/
INSTALLS += skins

#docs.path = /usr/share/man/man1
#docs.files = 

#  include(platforms/gnome/gnome.pro)
#	SOURCES += dsingleapplication.cpp platform_x11_hotkey.cpp
#	HEADERS += dsingleapplication.h platform_x11.h
}

TRANSLATIONS = tr/launchy_fr.ts \
               tr/launchy_nl.ts \
               tr/launchy_zh.ts 
               
                
