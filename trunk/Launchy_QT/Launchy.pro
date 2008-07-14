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





unix {
	DEFINES += SKINS_PATH=$(SKINS_PATH) PLUGINS_PATH=$(PLUGINS_PATH)\
			   PLATFORMS_PATH=$(PLATFORMS_PATH)

	if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
		DESTDIR = debug/
	}

	if(!debug_and_release|build_pass):CONFIG(release, debug|release) {
		DESTDIR = release/
	}
}


win32 {
	RC_FILE = win/launchy.rc
	LIBS += shell32.lib
	if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
		DESTDIR = ../debug/
	}

	if(!debug_and_release|build_pass):CONFIG(release, debug|release) {
		CONFIG += embed_manifest_exe
		DESTDIR = ../release/
	}
}



TRANSLATIONS = tr/launchy_fr.ts \
               tr/launchy_nl.ts \
               tr/launchy_zh.ts 
               
                
