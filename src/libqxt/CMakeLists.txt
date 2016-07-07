cmake_minimum_required (VERSION 2.8.12)
project (libqxt)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets)
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})


set(launchy_qxt_srcs
	qxtglobalshortcut.cpp
   )
if(__WIN__)
set(launchy_qxt_extras
	qxtglobalshortcut_win.cpp
   )
elseif(__MAC__)
set(launchy_qxt_extras
	qxtglobalshortcut_mac.cpp
   )
elseif(__LIN__)
set(launchy_qxt_extras
	qxtglobalshortcut_x11.cpp
   )
elseif(__BSD__)
set(launchy_qxt_extras
	qxtglobalshortcut_x11.cpp
   )
endif()

add_library(libqxt STATIC ${launchy_qxt_srcs} ${launchy_qxt_extras})
qt5_use_modules(libqxt Core Gui Widgets)
