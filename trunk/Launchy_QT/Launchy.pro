TEMPLATE = subdirs
SUBDIRS = src \
          plugins/calcy \
          plugins/gcalc \
          plugins/runner \
          plugins/weby 

unix {
  SUBDIRS += platforms/unix
}

win32 {
 SUBDIRS += platforms/win32
}

CONFIG += ordered debug_and_release
