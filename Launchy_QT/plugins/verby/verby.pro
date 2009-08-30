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
    % LIBS += user32.lib
    % LIBS += Gdi32.lib
    % LIBS += comctl32.lib
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
