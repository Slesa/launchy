TEMPLATE		= app
linux:TARGET 	= launchy
win32:TARGET	= Launchy
macx:TARGET		= Launchy

CONFIG			+= debug_and_release
QT				+= network widgets
win32:QT		+= winextras gui-private
#linux:QT		+= x11extras

INCLUDEPATH		+= ../

UI_DIR			= .ui
MOC_DIR			= .moc

RESOURCES		+= launchy.qrc
FORMS			= options.ui

SOURCES			= main.cpp \
                AnimationLabel.cpp \
                catalog_builder.cpp \
                catalog_types.cpp \
                CharLineEdit.cpp \
                CharListWidget.cpp \
                CommandHistory.cpp \
                commandlineparser.cpp \
                Fader.cpp \
                FileSearch.cpp \
                icon_delegate.cpp \
                icon_extractor.cpp \
                InputDataList.cpp \
                launchywidget.cpp \
                plugin_handler.cpp \
                plugin_interface.cpp \
                singleapplication.cpp \
                ../common/DropListWidget.cpp \
                ../common/FileBrowserDelegate.cpp \
                ../common/FileBrowser.cpp \
                optionsdialog.cpp
win32:SOURCES	+= win_launchywidget.cpp

HEADERS			= main.h \
                AnimationLabel.h \
                catalog_builder.h \
                catalog_types.h \
                CharLineEdit.h \
                CharListWidget.h \
                CommandHistory.h \
                commandlineparser.h \
                Fader.h \
                FileSearch.h \
                icon_delegate.h \
                icon_extractor.h \
                InputDataList.h \
                launchywidget.h \
                plugin_handler.h \
                plugin_interface.h \
                singleapplication.h \
                ../common/FileBrowserDelegate.h \
                ../common/FileBrowser.h \
                ../common/DropListWidget.h \
                optionsdialog.h
win32:HEADERS	+= win_launchywidget.h

TRANSLATIONS	= \
                ../../translations/launchy_fr.ts \
                ../../translations/launchy_nl.ts \
                ../../translations/launchy_zh.ts \
                ../../translations/launchy_es.ts \
                ../../translations/launchy_de.ts \
                ../../translations/launchy_ja.ts \
                ../../translations/launchy_zh_TW.ts \
                ../../translations/launchy_rus.ts

DESTDIR 		= $${PWD}/../../bin/app/
LIBS	 		= -L$${PWD}/../../bin/lib/ \
                -llaunchy.qxt

lupdate.target	= lupdate
lupdate.depends	= $${OBJECTS}
lupdate.commands = $$quote(lupdate $${PWD}/launchy.pro)
makeqm.target	= makeqm
makeqm.depends	= $${OBJECTS}
makeqm.commands	= $$quote(lrelease $${PWD}/launchy.pro)
skins.target	= skins
skins.depends	= $${OBJECTS}
skins.output	= $${DESTDIR}/skins
copytr.target	= copytr
copytr.depends	= $${OBJECTS}
copytr.output	= $${DESTDIR}/tr

QMAKE_EXTRA_TARGETS += lupdate makeqm skins copytr
PRE_TARGETDEPS	+= lupdate makeqm skins copytr

linux {
    ICON		= Launchy.ico
    LIBS		+= -L$${PWD}/../../bin/lib \
                -llaunchy.common \
                -lX11

    skins.commands	= $$quote(cp -r $${PWD}/../../skins $${DESTDIR}/ $$escape_expand(\n\t))
    copytr.commands	= $$quote(mkdir -p $${DESTDIR}/tr $$escape_expand(\n\t) \
                        cp $${PWD}/../../translations/*.qm $${DESTDIR}/tr/ $$escape_expand(\n\t))

#    target.path	= $$PREFIX/bin/
#    skins.path	= $$PREFIX/share/launchy/skins/
#    skins.files	= ../../skins/*
#    icon.path	= $$PREFIX/share/pixmaps
#    icon.files	= ../misc/Launchy_Icon/launchy_icon.png
#    desktop.path	= $$PREFIX/share/applications/
#    desktop.files	= ../linux/launchy.desktop
    INSTALLS	+= target \
                skins \
                icon \
                desktop
}

win32 { 
    ICON		= Launchy.ico
    Debug:CONFIG += console
    CONFIG		+= embed_manifest_exe
    RC_FILE		= ../win/launchy.rc
    LIBS		+= -L$${PWD}/../../bin/lib \
                launchy.common.lib \
                shell32.lib \
                user32.lib \
                gdi32.lib \
                ole32.lib \
                comctl32.lib \
                advapi32.lib \
                userenv.lib \
                netapi32.lib
    DEFINES		= VC_EXTRALEAN \
                WIN32 \
                _UNICODE \
                UNICODE \
                WINVER=0x0600 \
                _WIN32_WINNT=0x0600 \
                _WIN32_WINDOWS=0x0600 \
                _WIN32_IE=0x0700
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG

    skins.commands	= xcopy /I /E /Y $$shell_path($${PWD}/../../skins) $$shell_path($${DESTDIR}/skins/)
    copytr.commands	= xcopy /I /Y $$shell_path($${PWD}/../../translations/*.qm) $$shell_path($${DESTDIR}/tr/ )

}

macx { 
    ICON		= ../misc/Launchy_Icon/launchy_icon_mac.icns
    LIBS		+= -framework \
                Carbon \
                ../../lib/liblaunchy.common.a

    skins.commands	= $$quote(cp -r $${PWD}/../../skins $${DESTDIR}/Launchy.app/Contents/Resources/ $$escape_expand(\n\t))
    copytr.commands	= $$quote(mkdir -p $${DESTDIR}/Launchy.app/Contents/MacOS/tr $$escape_expand(\n\t) \
                        cp $${PWD}/../../translations/*.qm $${DESTDIR}/Launchy.app/Contents/MacOS/tr/ $$escape_expand(\n\t))

#    skins.path	= $${DESTDIR}/Launchy.app/Contents/Resources/skins/
#    skins.files	=
#    skins.extra	= rsync -arvz ../../../skins/   ../../app/Launchy.app/Contents/Resources/skins/   --exclude=\".svn\"

#    trans.path	= ../../app/Launchy.app/Contents/MacOS/tr/
#    trans.files = ../../../translations/*.qm
#    trans.extra = lupdate \
#        src.pro \
#        ; \
#        lrelease \
#        src.pro
#    dmg.path 			= ../../bin
#    dmg.files 			=
#    dmg.extra 			= cd \
#        ../mac \
#        ; \
#        bash \
#        deploy; \
#        cd \
#        ../src
#    INSTALLS += skins \
#        trans \
#        dmg
}
