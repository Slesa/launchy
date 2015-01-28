TARGET			= launchy.specs
QT				+= widgets testlib

MOC_DIR 		= .moc
INCLUDEPATH		+= ../launchy

HEADERS 		= \
                commandlineparser.specs.h \
                singleapplication.specs.h \
                ../launchy/commandlineparser.h \
                ../launchy/singleapplication.h

SOURCES 		= main.cpp \
                commandlineparser.specs.cpp \
                singleapplication.specs.cpp \
                ../launchy/commandlineparser.cpp \
                ../launchy/singleapplication.cpp

DESTDIR 		= $${PWD}../../../bin/specs/
