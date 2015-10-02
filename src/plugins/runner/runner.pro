include(../plugins.pri)
TARGET          = runner

QT              += widgets

CONFIG          += plugin

VPATH           += ../../launchy \
                ../../common
INCLUDEPATH     += ../../launchy

FORMS           = dlg.ui

HEADERS         = plugin_interface.h \
                runner.h \
                gui.h \
                globals.h \
                ../../common/FileBrowserDelegate.h \
                ../../common/FileBrowser.h \
                ../../common/DropTableWidget.h

SOURCES         = plugin_interface.cpp \
                runner.cpp \
                gui.cpp \
                ../../common/FileBrowser.cpp \
                ../../common/FileBrowserDelegate.cpp \
                ../../common/DropTableWidget.cpp

TRANSLATIONS    = \
                $${PWD}/../../../translations/runner_fr.ts \
                $${PWD}/../../../translations/runner_nl.ts \
                $${PWD}/../../../translations/runner_zh.ts \
                $${PWD}/../../../translations/runner_es.ts \
                $${PWD}/../../../translations/runner_de.ts \
                $${PWD}/../../../translations/runner_ja.ts \
                $${PWD}/../../../translations/runner_zh_TW.ts \
                $${PWD}/../../../translations/runner_rus.ts

lupdate.target  = lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/runner.pro)
makeqm.target   = makeqm
makeqm.depends  = $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/runner.pro)

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
    icon.files  = runner.png
    INSTALLS    += target icon
}

macx {
    icons.path   = $${DESTDIR}icons/
    icons.files	= runner.png
    INSTALLS    += icons
}
