cmake_minimum_required (VERSION 2.8.12)
project (common)

set (Launchy_VERSION_MAJOR 3)
set (Launchy_VERSION_MINOR 0)

configure_file (
	"${PROJECT_SOURCE_DIR}/version.h.in"
	"${PROJECT_SOURCE_DIR}/version.h"
	)

find_package(Qt5Core)
find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

if(__LIN__)
	find_package(Qt5 COMPONENTS X11Extras)
elseif(__WIN__)
	find_package(Qt5 COMPONENTS winextras)
elseif(__MAC__)
	find_package(Qt5 COMPONENTS macextras)
endif()

set(launchy_common_srcs
	catalog.cpp
	DropListWidget.cpp
	DropTableWidget.cpp
	FileBrowser.cpp
	FileBrowserDelegate.cpp
	globals.cpp
	globalshortcutmanager.cpp
	platform_base.cpp
	settingsmanager.cpp
   )
if(WIN32)
set(launchy_common_extras
	win.cpp
	win_Iconprovider.cpp
	win_keytrigger.cpp
	win_minidump.cpp
	win_platform.cpp
	win_util.cpp
   )
elseif(APPLE)
set(launchy_common_extras
	mac.cpp
	mac_keytrigger.cpp
	mac_platform.cpp
   )
elseif(UNIX)
set(launchy_common_extras
	x11.cpp
	x11_iconprovider.cpp
	x11_keytrigger.cpp
	x11_platform.cpp
   )
endif()
add_library(common ${launchy_common_srcs} ${launchy_common_extras})
qt5_use_modules(common Core Widgets)

