TEMPLATE = subdirs
SUBDIRS = src \
          plugins/calcy \
          plugins/gcalc \
          plugins/runner \
          plugins/weby \
          plugins/verby
          
win32 {
	SUBDIRS += plugins/controly
}
