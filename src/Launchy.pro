TEMPLATE        = subdirs
CONFIG          += ordered
SUBDIRS         =  \
                libqxt \
                common \
#          plugins/calcy \
#          plugins/gcalc \
                plugins/runner \
                plugins/verby \
                plugins/weby \
#                launchy.specs \
                launchy
          
win32 {
    SUBDIRS     += plugins/controly
}
