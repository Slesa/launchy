TEMPLATE		= app

TEMPLATE		= subdirs
SUBDIRS			=  \
#          plugins/calcy \
#          plugins/gcalc \
                plugins/runner \
                plugins/weby \
                plugins/verby \
                launchy.specs \
                launchy
          
win32 {
    SUBDIRS		+= plugins/controly
}
