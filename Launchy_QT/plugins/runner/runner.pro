 TEMPLATE      = lib
 CONFIG       += plugin debug_and_release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 UI_DIR		   = ../../plugins/runner/
 FORMS		   = dlg.ui
 HEADERS       = plugin_interface.h runner.h gui.h globals.h
 SOURCES       = plugin_interface.cpp runner.cpp gui.cpp
 TARGET		   = runner
 
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

}
