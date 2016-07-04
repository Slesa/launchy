cmake_minimum_required (VERSION 2.8.12)
project (runner)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets)
#include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})


set(launchy_runner_srcs
	gui.cpp
	runner.cpp
    ../../common/FileBrowser.cpp
    ../../common/FileBrowserDelegate.cpp
    ../../common/DropTableWidget.cpp
    ../../launchy/plugin_interface.cpp
   )
set(launchy_runner_srcs
	globals.h
	gui.h
	runner.h
	winfiles.h
    ../../common/FileBrowserDelegate.h
    ../../common/FileBrowser.h
    ../../common/DropTableWidget.h
    ../../launchy/plugin_interface.h
   )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/launchy)

QT5_WRAP_UI(launchy_runner_form dlg.ui)

add_library(runner ${launchy_runner_srcs} ${launchy_runnrer_hdrs} ${launchy_runnrer_form})
target_link_libraries(runner common)
qt5_use_modules(runner Core Gui Widgets)
