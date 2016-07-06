include(../global.pri)
TEMPLATE        = app
linux:TARGET    = launchy
win32:TARGET    = Launchy
macx:TARGET     = Launchy
VERSION         = 2.7.0

CONFIG          += debug_and_release
QT              += network widgets gui-private
win32:QT        += winextras
linux:QT        += x11extras

UI_DIR          = .ui
MOC_DIR         = .moc

RESOURCES       += launchy.qrc
FORMS           = options.ui

SOURCES         = main.cpp \
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
                $${PWD}/../../translations/launchy_fr.ts \
                $${PWD}/../../translations/launchy_nl.ts \
                $${PWD}/../../translations/launchy_zh.ts \
                $${PWD}/../../translations/launchy_es.ts \
                $${PWD}/../../translations/launchy_de.ts \
                $${PWD}/../../translations/launchy_ja.ts \
                $${PWD}/../../translations/launchy_zh_TW.ts \
                $${PWD}/../../translations/launchy_rus.ts

DESTDIR         = $${DESTDIR}app/

CONFIG(debug, debug|release) {
    LIBS        += -L$${PWD}/../../bin/debug/lib/
}
CONFIG(release, debug|release) {
    LIBS        += -L$${PWD}/../../bin/release/lib/
}
LIBS            += \
                -llaunchy.qxt \
                -llaunchy.common

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

deployqt.target = deployqt
#deployqt.output = $${DESTDIR}/
moveqtqm.target = moveqtqm
moveqtqm.depends = deployqt

CONFIG(release, debug|release) {
    deployqt.commands = %QTDIR%\bin\windeployqt --release --no-plugins --no-webkit $${DESTDIR}/
}
#CONFIG(debug, debug|release) {
#    deployqt.commands = %QTDIR%\bin\windeployqt --no-plugins --no-webkit $${DESTDIR}/
#}

CONFIG(release, debug|release) {

    setup.target    = setup
#    setup.depends   = copytr skins lupdate
    setup.path     = $${PWD}/../../bin/release/app

    moveqtqm.commands = $$quote(@move $${setup.path}\*.qm $${setup.path}\tr)

    win32 {
        outfile = $${PWD}/../../setup/win/setup.nsi
        outfile = $$absolute_path($${outfile})
        outfile = $$system_path($${outfile})
        setupexe = $$absolute_path($${PWD}/../../bin/$${TARGET}-$${VERSION}.exe)
        setupexe = $$system_path($${setupexe})
        setup.output = $${PWD}/../../bin/launchy-$${VERSION}.exe
        setup.commands = \
            $$system(del $${outfile}) \
            $$system(echo "!include \"MUI2.nsh\"" > $${outfile}) \
            $$system(echo "Name \"$${TARGET}\"" >> $${outfile}) \
            $$system(echo "OutFile \"$${setupexe}\"" >> $${outfile}) \
            $$system(type $${outfile}.template >> $${outfile})
#            $${PWD}/../../tools/nsis/Bin/makensis.exe /DVERSION=$${VERSION} $${PWD}\..\..\setup\win\setup.nsi
    }

    macx {
        outfile = $${PWD}/../../setup/mac/create-setup.sh
        outfile = $$absolute_path($${outfile})
        outfile = $$system_path($${outfile})
        setupdmg = $$absolute_path($${PWD}/../../bin/$${TARGET}-$${VERSION}.dmg)
        setupdmg = $$system_path($${setupdmg})
        setup.output    =$${outfile}
        source = $${DESTDIR}
        source = $$absolute_path($${source})
        source = $$system_path($${source})
#        setup.commands = hdiutil create -size 200m -volname LaunchySetup -srcfolder $${PWD}/../../bin/app/ -ov -format UDZO $${PWD}/../../bin/launchy-$${VERSION}.dmg
        setup.commands = \
            $$system(echo "hdiutil create -size 200m -volname LaunchySetup -srcfolder $${source} -ov -format UDZO $${setupdmg}" > $${outfile})
    }

    POST_TARGETDEPS    += setup
}

QMAKE_EXTRA_TARGETS += lupdate makeqm skins copytr setup
PRE_TARGETDEPS	+= lupdate makeqm skins copytr

linux {
    ICON		= Launchy.ico
    LIBS		+= \
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
    LIBS		+= \
                shell32.lib \
                user32.lib \
                gdi32.lib \
                ole32.lib \
                comctl32.lib \
                advapi32.lib \
                userenv.lib \
                netapi32.lib
    DEFINES		+= VC_EXTRALEAN \
                WIN32 \
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
    ICON        = ../misc/Launchy_Icon/launchy_icon_mac.icns
    LIBS        += -framework \
                Carbon
#                ../../lib/liblaunchy.common.a

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
