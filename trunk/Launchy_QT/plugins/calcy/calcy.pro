 TEMPLATE      = lib
 CONFIG       += plugin debug_and_release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 INCLUDEPATH += c:/boost/
 UI_DIR		   = ../../plugins/calcy/
 HEADERS       = plugin_interface.h calcy.h 
 SOURCES       = plugin_interface.cpp calcy.cpp 
 TARGET		   = calcy
 
 win32 {
	CONFIG -= embed_manifest_dll
	LIBS += shell32.lib
%	LIBS += user32.lib
%	LIBS += Gdi32.lib
%	LIBS += comctl32.lib
}
 
 *:debug {
        CONFIG -= release
	DESTDIR = ../../debug/plugins/
 }
 *:release {
        CONFIG -= debug
	DESTDIR = ../../release/plugins/
	%QMAKE_CXXFLAGS += /Ox /Ob2 /Oi /Oy /GT /GA  /WX
 }

unix {
 target.path = /usr/lib/launchy/plugins/
 INSTALLS += target

 icon.path = /usr/lib/launchy/plugins/icons/
 icon.files = calcy.ico
 INSTALLS += icon
}
