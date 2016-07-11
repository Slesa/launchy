cmake_minimum_required (VERSION 2.8.12)
project (verby)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets)
find_package(Qt5LinguistTools)

#include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

set(launchy_verby_trans
    ../../../translations/verby_fr.ts
    ../../../translations/verby_nl.ts
    ../../../translations/verby_zh.ts
    ../../../translations/verby_es.ts
    ../../../translations/verby_de.ts
    ../../../translations/verby_ja.ts
    ../../../translations/verby_zh_TW.ts
    ../../../translations/verby_rus.ts   
    )
set(launchy_verby_srcs
	gui.cpp
	Verby.cpp
	../../launchy/plugin_interface.cpp
	)
set(launchy_verby_hdrs
	gui.h
	Verby.h
	../../launchy/plugin_interface.h
	)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/launchy)

QT5_WRAP_UI(launchy_verby_form dlg.ui)
QT5_ADD_TRANSLATION(launchy_verby_qms ${launchy_verby_trans})

add_library(verby SHARED ${launchy_verby_srcs} ${launchy_verby_hdrs} ${launchy_verby_form} ${launchy_verby_qms})
set_target_properties(verby PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/plugins
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/plugins
    )
qt5_use_modules(verby Core Gui Widgets)
if(EXISTS "${PROJECT_BINARY_DIR}/verby_fr.qm")
    file(COPY ${launchy_verby_qms} DESTINATION ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/tr)
endif()
