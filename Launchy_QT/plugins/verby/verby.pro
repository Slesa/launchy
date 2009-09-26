TEMPLATE = lib
CONFIG += plugin \
    release
VPATH += ../../src/
INCLUDEPATH += ../../src/
UI_DIR = ../../plugins/Verby/
FORMS = dlg.ui
HEADERS = plugin_interface.h \
    gui.h \
    Verby.h
SOURCES = plugin_interface.cpp \
    gui.cpp \
    Verby.cpp
TARGET = verby
win32 { 
    CONFIG -= embed_manifest_dll
    LIBS += shell32.lib
}
win32:debug {
	%QMAKE_CXXFLAGS += /ZI
}
*:debug:DESTDIR = ../../debug/plugins/
*:release { 
    DESTDIR = ../../release/plugins/
    %QMAKE_CXXFLAGS += /Ox \
        /Ob2 \
        /Oi \
        /Oy \
        /GT \
        /GA \
        /WX
}
