TARGET			= launchy.specs
QT				+= widgets testlib

MOC_DIR 		= .moc
INCLUDEPATH		+= ../launchy

HEADERS 		= \
                commandlineparser.specs.h \
                ../launchy/commandlineparser.h

SOURCES 		= main.cpp \
                commandlineparser.specs.cpp \
                ../launchy/commandlineparser.cpp

DESTDIR 		= ../../specs/
