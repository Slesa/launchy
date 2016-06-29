cmake_minimum_required (VERSION 2.8.12)
project (libqxt)

set(launchy_qxt_srcs
	qxtglobalshortcut.cpp
   )
if(WIN32)
set(launchy_qxt_extras
	qxtglobalshortcut_win.cpp
   )
elseif(APPLE)
set(launchy_qxt_extras
	qxtglobalshortcut_mac.cpp
   )
elseif(UNIX)
set(launchy_qxt_extras
	qxtglobalshortcut_x11.cpp
   )
endif()

add_library(libqxt ${launchy_qxt_srcs} ${launchy_qxt_extras})
