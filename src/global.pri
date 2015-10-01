CONFIG          += qt_warn debug_and_release

win32:DEFINES   += UNICODE _UNICODE
INCLUDEPATH     += $${PWD}/

CONFIG(debug, debug|release) {
    DESTDIR     = $${PWD}/../bin/debug/
#    LIBPATH        = -L$${PWD}/../../bin/debug/lib/
}
CONFIG(release, debug|release) {
    DESTDIR     = $${PWD}/../bin/release/
#    LIBPATH        = -L$${PWD}/../../bin/release/lib/
}
