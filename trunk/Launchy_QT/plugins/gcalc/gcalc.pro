 TEMPLATE      = lib
 QT			+= network
 CONFIG       += plugin debug_and_release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 INCLUDEPATH += c:/boost/
 UI_DIR		   = ../../plugins/gcalc/
 HEADERS       = plugin_interface.h gcalc.h 
 SOURCES       = plugin_interface.cpp gcalc.cpp 
 TARGET		   = gcalc

 win32 {
	CONFIG -= embed_manifest_dll
	LIBS += shell32.lib
%	LIBS += user32.lib
%	LIBS += Gdi32.lib
%	LIBS += comctl32.lib
}
 

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    DESTDIR = ../../debug/plugins
}

if(!debug_and_release|build_pass):CONFIG(release, debug|release) {
    DESTDIR = ../../release/plugins
}




unix {
 PREFIX = /usr
 target.path = $$PREFIX/lib/launchy/plugins/
# icon.path = $$PREFIX/lib/launchy/plugins/icons/
# icon.files = gcalc.png
 INSTALLS += target
}
