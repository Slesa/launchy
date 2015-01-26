TEMPLATE		= lib
TARGET			= controly

QT				+= widgets
win32:QT		+= winextras

CONFIG			+= plugin debug_and_release
DEFINES			+= WITH_GUI

VPATH			+= ../../launchy/
INCLUDEPATH		+= ../../launchy/

UI_DIR			= .ui
MOC_DIR			= .moc

FORMS			= dlg.ui

HEADERS			= plugin_interface.h \
                winfiles.h \
                controly.h \
                gui.h \
                ControlPanelItemFinder.h \
                fhoicon.h \
                fhoenv.h \
                fhoreg.h \
                fhores.h

SOURCES			= plugin_interface.cpp \
                controly.cpp \
                gui.cpp \
                ControlPanelItemFinder.cpp \
                fhoicon.cpp \
                fhoenv.cpp \
                fhoreg.cpp \
                fhores.cpp

win32 { 
    CONFIG		-= embed_manifest_dll
    LIBS		+= shell32.lib \
                user32.lib \
                Gdi32.lib \
                comctl32.lib \
                Advapi32.lib \
                ole32.lib \
                shlwapi.lib
	QMAKE_CXXFLAGS_RELEASE += /Zi
	QMAKE_LFLAGS_RELEASE += /DEBUG
# disable optimizations to prevent crashes with certain third party control panel
# applets when Controly is built using VC++ 2005.
	QMAKE_CXXFLAGS_RELEASE -= -O2
}

# *:debug {
# DESTDIR = ../../debug/plugins/
# }
# *:release {
# DESTDIR = ../../release/plugins/
# %QMAKE_CXXFLAGS += /Ox /Ob2 /Oi /Oy /GT /GA /WX
# }
if(!debug_and_release|build_pass):CONFIG(debug, debug|release):DESTDIR = ../../debug/plugins
if(!debug_and_release|build_pass):CONFIG(release, debug|release):DESTDIR = ../../release/plugins
