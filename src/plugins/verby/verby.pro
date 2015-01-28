include(../plugins.pri)
TARGET			= verby

QT				+= widgets

CONFIG			+= plugin debug_and_release

VPATH			+= ../../launchy/
INCLUDEPATH		+= ../../launchy/ \
                ../../common

FORMS			= dlg.ui

HEADERS			= plugin_interface.h \
                gui.h \
                Verby.h

SOURCES			= plugin_interface.cpp \
                gui.cpp \
                Verby.cpp

DESTDIR			= $${PWD}/../../../bin/app/plugins

TRANSLATIONS	= \
                ../../../translations/verby_fr.ts \
                ../../../translations/verby_nl.ts \
                ../../../translations/verby_zh.ts \
                ../../../translations/verby_es.ts \
                ../../../translations/verby_de.ts \
                ../../../translations/verby_ja.ts \
                ../../../translations/verby_zh_TW.ts \
                ../../../translations/verby_rus.ts

win32 {
    CONFIG		-= embed_manifest_dll
    LIBS		+= user32.lib shell32.lib
	QMAKE_CXXFLAGS_RELEASE += /Zi
	QMAKE_LFLAGS_RELEASE += /DEBUG
}

linux {
    PREFIX		= /usr
    target.path	= $$PREFIX/lib/launchy/plugins/
    icon.path	= $$PREFIX/lib/launchy/plugins/icons/
    icon.files	= verby.png copy.png opencontainer.png properties.png run.png
    INSTALLS	+= target icon
}

macx {
    DESTDIR		= $${PWD}/../../../bin/app/Launchy.app/Contents/MacOS/plugins

    icons.path	= ../../../app/Launchy.app/Contents/MacOS/plugins/icons/
    icons.files	= verby.png copy.png opencontainer.png properties.png run.png
    INSTALLS	+= icons
}
