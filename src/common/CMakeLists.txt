cmake_minimum_required (VERSION 2.8.12)
project (common)

set (Launchy_VERSION_MAJOR 3)
set (Launchy_VERSION_MINOR 0)
set (CMAKE_AUTOMOC OFF)

configure_file (
	"${PROJECT_SOURCE_DIR}/version.h.in"
	"${PROJECT_SOURCE_DIR}/version.h"
	)

find_package(Qt5Core)
find_package(Qt5Widgets)
#include_directories(${Qt5Widgets_INCLUDES})
#add_definitions(${Qt5Widgets_DEFINITIONS})
#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

if(__LIN__)
	find_package(Qt5 COMPONENTS X11Extras)
    set(launchy_common_mods
      X11Extras
      )
elseif(__WIN__)
	find_package(Qt5 COMPONENTS winextras)
    set(launchy_common_mods
      WinExtras
      )
elseif(__MAC__)
	find_package(Qt5 COMPONENTS macextras)
    set(launchy_common_mods
      MacExtras
      )
endif()

set(launchy_common_moc_hdrs
    DropListWidget.h
    DropTableWidget.h
    FileBrowser.h
    FileBrowserDelegate.h
    globalshortcutmanager.h
    globalshortcuttrigger.h
    settingsmanager.h
  )
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
set(launchy_common_hdrs
    catalog.h
    directory.h
    globals.h
    platform_base.h
    version.h
   )
if(__WIN__)
set(launchy_common_extras
	win.cpp
	win_Iconprovider.cpp
    win_iconprovider.h
    win_files.h
	win_keytrigger.cpp
	win_minidump.cpp
    win_minidump.h
	win_platform.cpp
    win_platform.h
	win_util.cpp
    win_util.h
   )
elseif(__MAC__)
set(launchy_common_extras
	mac.cpp
	mac_keytrigger.cpp
    mac_keytrigger.h
	mac_platform.cpp
    mac_platform.h
   )
elseif(__LIN__)
set(launchy_common_extra_moc_hdrs
  x11_keytrigger.h
  x11_platform.h
  )
set(launchy_common_extras
	x11.cpp
	x11_iconprovider.cpp
  x11_iconprovider.h
	x11_keytrigger.cpp
	x11_platform.cpp
   )
endif()

qt5_wrap_cpp(launchy_common_mocs ${launchy_common_moc_hdrs} ${launchy_common_extra_moc_hdrs})

add_library(common STATIC ${launchy_common_srcs} ${launchy_common_hdrs} ${launchy_common_extras} ${launchy_common_mocs} ${launchy_common_mochdrs} ${launchy_common_extra_mochdrs})
qt5_use_modules(common Core Widgets ${launchy_common_mods})

