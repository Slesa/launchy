cmake_minimum_required (VERSION 2.8.12)
project (common)

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
