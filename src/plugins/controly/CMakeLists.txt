cmake_minimum_required (VERSION 2.8.12)
project (controly)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets Network)
find_package(Qt5LinguistTools)

#include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

set(launchy_controly_trans
    ../../../translations/controly_fr.ts
    ../../../translations/controly_nl.ts
    ../../../translations/controly_zh.ts
    ../../../translations/controly_es.ts
    ../../../translations/controly_de.ts
    ../../../translations/controly_ja.ts
    ../../../translations/controly_zh_TW.ts
    ../../../translations/controly_rus.ts   
    )
set(launchy_controly_srcs
	gui.cpp
	controly.cpp
    ControlPanelItemFinder.cpp
    fhoicon.cpp
    fhoenv.cpp
    fhoreg.cpp
    fhores.cpp
    ../../launchy/plugin_interface.cpp
    ../../common/DropTableWidget.cpp
   )
set(launchy_controly_hdrs
globals.h
	gui.h
	controly.h
    ControlPanelItemFinder.h
    fhoicon.h
    fhoenv.h
    fhoreg.h
    fhores.h
    ../../launchy/plugin_interface.h
    ../../common/DropTableWidget.h
   )
if(__WIN__)
    find_package(Qt5 COMPONENTS winextras)
    set(launchy_controly_mods
        WinExtras
    )
endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/launchy)

QT5_WRAP_UI(launchy_controly_form dlg.ui)
QT5_ADD_TRANSLATION(launchy_controly_qms ${launchy_controly_trans})

add_library(controly SHARED ${launchy_controly_srcs} ${launchy_controly_hdrs} ${launchy_controly_form} ${launchy_controly_qms})
#target_link_libraries(controly common)
qt5_use_modules(controly Core Gui Widgets Network ${launchy_controly_mods})

install(TARGETS controly DESTINATION plugins)
install(FILES ${launchy_controly_qms} DESTINATION tr)
