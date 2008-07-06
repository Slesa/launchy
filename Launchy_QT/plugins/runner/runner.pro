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
 
 *:debug {
        CONFIG -= release 
	DESTDIR = ../../debug/plugins/
 }
 *:release {
	DESTDIR = ../../release/plugins/
 }

unix {
 target.path = /usr/lib/launchy/plugins/
 INSTALLS += target

 icon.path = /usr/lib/launchy/plugins/icons/
 icon.files = runner.ico
 INSTALLS += icon
}
