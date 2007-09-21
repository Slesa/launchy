TEMPLATE	= app
CONFIG		+= qt_warn release
QT			+= network
TARGET		= Launchy
VPATH		+= src/
UI_DIR		= ../src/
FORMS		= ../src/options.ui
SOURCES		= main.cpp globals.cpp platform_base_hotkey.cpp \ 
			  options.cpp catalog.cpp catalog_builder.cpp \ 
			  dsingleapplication.cpp plugin_handler.cpp \
			  icon_delegate.cpp
HEADERS		= platform_util.h platform_base.h globals.h \
			  globals.h main.h platform_base_hottrigger.h \ 
			  platform_base_hotkey.h catalog.h \
			  catalog_builder.h dsingleapplication.h \ 
			  plugin_interface.h plugin_handler.h options.h \
			  icon_delegate.h
ICON		= Launchy.ico


win32 {
	RC_FILE = win/launchy.rc
	SOURCES += platform_win.cpp platform_win_hotkey.cpp
	HEADERS += platform_win.h platform_win_hotkey.h
	LIBS += shell32.lib
	LIBS += user32.lib
	LIBS += Gdi32.lib
	LIBS += comctl32.lib
}

win32:debug {
%	CONFIG += console 
	DESTDIR = ../debug/
}

win32:release {
	CONFIG += embed_manifest_exe
	DESTDIR = ../release/
}
