include(../global.pri)
TEMPLATE        = lib

UI_DIR          = .ui
MOC_DIR         = .moc

DESTDIR         = $${DESTDIR}app/plugins/

# DESTDIR = $${PWD}/../../../bin/app/plugins
#if(!debug_and_release|build_pass):CONFIG(debug, debug|release):DESTDIR = $${PWD}/../../../bin/app/plugins
#if(!debug_and_release|build_pass):CONFIG(release, debug|release):DESTDIR = $${PWD}/../../../bin/app/plugins


#macx {
#    DESTDIR = $${PWD}/../../../bin/app/Launchy.app/Contents/MacOS/plugins
#    if(!debug_and_release|build_pass):CONFIG(debug, debug|release):DESTDIR = ../../../debug/Launchy.app/Contents/MacOS/plugins
#    if(!debug_and_release|build_pass):CONFIG(release, debug|release):DESTDIR = ../../../release/Launchy.app/Contents/MacOS/plugins
#}



#linux-g++{
    #...
#    EXTRA_BINFILES += \
#        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstrtp.so \
#        $${THIRDPARTY_PATH}/gstreamer-0.10/linux/plugins/libgstvideo4linux2.so
#    for(FILE,EXTRA_BINFILES){
#        QMAKE_POST_LINK += $$quote(cp $${FILE} $${DESTDIR}$$escape_expand(\n\t))
#    }
#}

#win32 {
    #...
#    EXTRA_BINFILES += \
#        $${THIRDPARTY_PATH}/glib-2.0/win32/bin/libglib-2.0.dll \
#        $${THIRDPARTY_PATH}/glib-2.0/win32/bin/libgmodule-2.0.dll
#    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
#    EXTRA_BINFILES_WIN ~= s,/,\\,g
#        DESTDIR_WIN = $${DESTDIR}
#    DESTDIR_WIN ~= s,/,\\,g
#    for(FILE,EXTRA_BINFILES_WIN){
#                QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
#    }
#}

