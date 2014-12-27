TEMPLATE = subdirs
SUBDIRS =  \
#          plugins/calcy \
#          plugins/gcalc \
          plugins/runner \
          plugins/weby \
          plugins/verby \
          launchy
          
win32 {
	SUBDIRS += plugins/controly
}
