 TEMPLATE      = lib
 CONFIG       += plugin release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 HEADERS       = plugin_interface.h weby.h
 SOURCES       = weby.cpp
 TARGET		   = weby
 
 *:debug {
	DESTDIR = ../../debug/plugins/
 }
 *:release {
	DESTDIR = ../../release/plugins/
 }

