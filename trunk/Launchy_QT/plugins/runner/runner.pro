 TEMPLATE      = lib
 CONFIG       += plugin release
 VPATH 		  += ../../src/
 INCLUDEPATH += ../../src/
 UI_DIR		   = ../../plugins/runner/
 FORMS		   = dlg.ui
 HEADERS       = plugin_interface.h runner.h gui.h globals.h
 SOURCES       = plugin_interface.cpp runner.cpp gui.cpp
 TARGET		   = runner
 
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
	QMAKE_CXXFLAGS += /Ox /Ob2 /Oi /Oy /GT /GA  /WX
 }

