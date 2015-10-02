include(../plugins.pri)
TARGET          = verby

QT              += widgets

CONFIG          += plugin

VPATH           += ../../launchy/
INCLUDEPATH     += ../../launchy/

FORMS           = dlg.ui

HEADERS         = plugin_interface.h \
                gui.h \
                Verby.h

SOURCES         = plugin_interface.cpp \
                gui.cpp \
                Verby.cpp

TRANSLATIONS    = \
                $${PWD}/../../../translations/verby_fr.ts \
                $${PWD}/../../../translations/verby_nl.ts \
                $${PWD}/../../../translations/verby_zh.ts \
                $${PWD}/../../../translations/verby_es.ts \
                $${PWD}/../../../translations/verby_de.ts \
                $${PWD}/../../../translations/verby_ja.ts \
                $${PWD}/../../../translations/verby_zh_TW.ts \
                $${PWD}/../../../translations/verby_rus.ts

lupdate.target  = lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/verby.pro)
makeqm.target   = makeqm
makeqm.depends  = $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/verby.pro)

QMAKE_EXTRA_TARGETS += lupdate makeqm
PRE_TARGETDEPS	+= lupdate makeqm


win32 {
    CONFIG      -= embed_manifest_dll
    LIBS        += user32.lib shell32.lib
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
}

linux {
    PREFIX      = /usr
    target.path	= $$PREFIX/lib/launchy/plugins/
    icon.path   = $$PREFIX/lib/launchy/plugins/icons/
    icon.files  = verby.png copy.png opencontainer.png properties.png run.png
    INSTALLS    += target icon
}

macx {
    icons.path   = $${DESTDIR}icons/
    icons.files	= verby.png copy.png opencontainer.png properties.png run.png
    INSTALLS    += icons
}
