TEMPLATE = lib
CONFIG += plugin \
    release
VPATH += ../../src/
INCLUDEPATH += ../../src/
PRECOMPILED_HEADER = precompiled.h
UI_DIR = ../../plugins/Verby/
FORMS = dlg.ui
HEADERS = plugin_interface.h \
    gui.h \
    Verby.h \
    precompiled.h
SOURCES = plugin_interface.cpp \
    gui.cpp \
    Verby.cpp
TARGET = verby
win32 { 
    CONFIG -= embed_manifest_dll
    LIBS += shell32.lib user32.lib
}
win32:debug:%QMAKE_CXXFLAGS += /ZI
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
unix:!macx {
    PREFIX = /usr
    target.path = $$PREFIX/lib/launchy/plugins/
    icon.path = $$PREFIX/lib/launchy/plugins/icons/
    icon.files = verby.png copy.png opencontainer.png properties.png run.png
    INSTALLS += target \
        icon
}
macx {
  INCLUDEPATH += /opt/local/include/
}
