TEMPLATE	= lib
TARGET		= platform_gnome
CONFIG		+= plugin qt_warn debug_and_release
VPATH 		+= ../../src/ ../unix
INCLUDEPATH     += ../../src/ ../unix
SOURCES		= platform_x11_hotkey.cpp platform_base_hotkey.cpp \
                  platform_gnome.cpp platform_gnome_util.cpp \
                  platform_unix.cpp platform_unix_util.cpp

HEADERS		= platform_base.h platform_gnome.h \
                  platform_base_hotkey.h platform_base_hottrigger.h \
                  platform_gnome_util.h platform_unix.h \
                  platform_unix_util.h

CONFIG		-= embed_manifest_dll
CONFIG          += link_pkgconfig
PKGCONFIG       += gtk+-2.0 libgnomeui-2.0 gnome-desktop-2.0
LIBS += -lX11 -lXext -lXrender

debug {
        CONFIG -= release
	DESTDIR = ../../debug/
}

release {
        CONFIG -= debug
	DESTDIR = ../../release/
}

unix {
}
