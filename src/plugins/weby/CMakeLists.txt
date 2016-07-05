cmake_minimum_required (VERSION 2.8.12)
project (weby)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets Network)
find_package(Qt5LinguistTools)

#include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

set(launchy_weby_trans
    ../../../translations/weby_fr.ts
    ../../../translations/weby_nl.ts
    ../../../translations/weby_zh.ts
    ../../../translations/weby_es.ts
    ../../../translations/weby_de.ts
    ../../../translations/weby_ja.ts
    ../../../translations/weby_zh_TW.ts
    ../../../translations/weby_rus.ts   
    )
set(launchy_weby_srcs
	gui.cpp
	IconCache.cpp
	weby.cpp
    ../../launchy/plugin_interface.cpp
    ../../common/DropTableWidget.cpp
   )
set(launchy_weby_hdrs
globals.h
	gui.h
	IconCache.h
	weby.h
    ../../launchy/plugin_interface.h
    ../../common/DropTableWidget.h
   )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/launchy)

QT5_WRAP_UI(launchy_weby_form dlg.ui)
QT5_ADD_TRANSLATION(launchy_weby_qms ${launchy_weby_trans})

add_library(weby SHARED ${launchy_weby_srcs} ${launchy_weby_hdrs} ${launchy_weby_form} ${launchy_weby_qms})
#target_link_libraries(weby common)
qt5_use_modules(weby Core Gui Widgets Network)
