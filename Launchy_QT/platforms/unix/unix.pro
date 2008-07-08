TEMPLATE	= lib
TARGET		= platform_unix
CONFIG		+= plugin qt_warn debug_and_release
VPATH 		+= ../../src/
INCLUDEPATH     += ../../src/
SOURCES		= platform_x11_hotkey.cpp platform_base_hotkey.cpp platform_unix.cpp platform_unix_util.cpp
HEADERS		= platform_base.h platform_unix.h platform_base_hotkey.h platform_base_hottrigger.h  platform_unix_util.h
CONFIG		-= embed_manifest_dll
LIBS += -lX11 -lXext -lXrender



DEFINES += SKINS_PATH=$(SKINS_PATH) PLUGINS_PATH=$(PLUGINS_PATH)\
           PLATFORMS_PATH=$(PLATFORMS_PATH)

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
     DESTDIR = ../../debug/
}

if(!debug_and_release|build_pass):CONFIG(release, debug|release) {
     DESTDIR = ../../release/
}
