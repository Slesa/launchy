TEMPLATE = subdirs
SUBDIRS = src \
          plugins/calcy \
          plugins/gcalc \
          plugins/runner \
          plugins/weby \
          plugins/verby

unix {
  SUBDIRS += platforms/unix
}

win32 {
 SUBDIRS += platforms/win
}
