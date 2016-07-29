cmake_minimum_required (VERSION 3.5.1)
project (runner)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets)
find_package(Qt5LinguistTools)

set(launchy_runner_trans
    ../../../translations/runner_fr.ts
    ../../../translations/runner_nl.ts
    ../../../translations/runner_zh.ts
    ../../../translations/runner_es.ts
    ../../../translations/runner_de.ts
    ../../../translations/runner_ja.ts
    ../../../translations/runner_zh_TW.ts
    ../../../translations/runner_rus.ts   
    )
set(launchy_runner_srcs
	gui.cpp
	runner.cpp
    ../../common/FileBrowser.cpp
    ../../common/FileBrowserDelegate.cpp
    ../../common/DropTableWidget.cpp
    ../../launchy/plugin_interface.cpp
   )
set(launchy_runner_hdrs
	globals.h
	gui.h
	runner.h
	winfiles.h
    ../../common/FileBrowserDelegate.h
    ../../common/FileBrowser.h
    ../../common/DropTableWidget.h
    ../../launchy/plugin_interface.h
   )
set(runnery_pngs
   runner.png)


include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/launchy)

QT5_WRAP_UI(launchy_runner_form dlg.ui)
QT5_ADD_TRANSLATION(launchy_runner_qms ${launchy_runner_trans})

add_library(runner SHARED ${launchy_runner_srcs} ${launchy_runner_hdrs} ${launchy_runner_form} ${launchy_runner_qms})
set_target_properties(runner PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/fplugins
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/plugins
    )
qt5_use_modules(runner Core Gui Widgets)
if(EXISTS "${PROJECT_BINARY_DIR}/runner_fr.qm")
    file(COPY ${launchy_runner_qms} DESTINATION ${PROJECT_BINARY_DIR}/../../${BIN_INSTALL_DIR}/tr)
endif()


# *** Installation ***
install(TARGETS runner DESTINATION ${PREFIX_PLUGINS}/plugins COMPONENT plugins)
install(FILES ${runner_pngs} DESTINATION ${PREFIX_ICONS}/icons COMPONENT plugins)
if(__WIN__ OR __MAC__)
    install(FILES ${launchy_runner_qms} DESTINATION ${TR_OUT_PATH}/tr COMPONENT plugins)
endif()