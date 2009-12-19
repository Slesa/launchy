 TEMPLATE      = lib
 CONFIG       += plugin debug_and_release
 FORMS		   = 
 HEADERS       = plugin_interface.h myplugin.h
 SOURCES       = plugin_interface.cpp myplugin.cpp 
 TARGET		   = myplugin
 
 win32 {
 	CONFIG -= embed_manifest_dll
	LIBS += shell32.lib
%	LIBS += user32.lib
%	LIBS += Gdi32.lib
%	LIBS += comctl32.lib
}

