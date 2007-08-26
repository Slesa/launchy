 TEMPLATE      = lib
 CONFIG       += plugin release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 HEADERS       = plugin_interface.h weby.h
 SOURCES       = weby.cpp
 TARGET		   = weby
 
 win32 {
	LIBS += shell32.lib
%	LIBS += user32.lib
%	LIBS += Gdi32.lib
%	LIBS += comctl32.lib
}
 
 *:debug {
	DESTDIR = ../../debug/plugins/
 }
 *:release {
	DESTDIR = ../../release/plugins/
 }

