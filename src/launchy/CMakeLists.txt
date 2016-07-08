cmake_minimum_required (VERSION 2.8.12)
project (launchy)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets Network)
find_package(Qt5LinguistTools)
if(__LIN__)
    find_package(X11 REQUIRED)
elseif(__MAC__)
    find_package(X11 REQUIRED)
endif()

set(launchy_trans
    ../../translations/launchy_fr.ts
    ../../translations/launchy_nl.ts
    ../../translations/launchy_zh.ts
    ../../translations/launchy_es.ts
    ../../translations/launchy_de.ts
    ../../translations/launchy_ja.ts
    ../../translations/launchy_zh_TW.ts
    ../../translations/launchy_rus.ts)
set(launchy_srcs
	AnimationLabel.cpp
	catalog_builder.cpp
	catalog_types.cpp
	CharLineEdit.cpp
	CharListWidget.cpp
	CommandHistory.cpp
	commandlineparser.cpp
	dsingleapplication.cpp
	Fader.cpp
	FileSearch.cpp
	icon_delegate.cpp
	icon_extractor.cpp
	InputDataList.cpp
	launchywidget.cpp
	LineEditMenu.cpp
	main.cpp
	optionsdialog.cpp
	platform_util.cpp
	plugin_handler.cpp
	plugin_interface.cpp
	singleapplication.cpp
   )

QT5_WRAP_UI(launchy_form_hdr options.ui)
QT5_ADD_TRANSLATION(launchy_qms ${launchy_trans})

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/libqxt)

add_executable(launchy ${launchy_srcs} ${launchy_form_hdr} ${launchy_common_hdrs} ${launchy_hdr_moc} ${launchy_qms})

qt5_use_modules(launchy Core Gui Widgets Network)

target_link_libraries(launchy common libqxt)
if(__LIN__)
    target_link_libraries(launchy ${X11_LIBRARIES})
elseif (__MAC__)
    target_link_libraries(launchy ${X11_LIBRARIES})
	target_link_libraries(launchy ${CARBON_FRAMEWORK})
endif()

if(EXISTS ${PROJECT_BINARY_DIR}/launchy_fr.qm)
    file(COPY ${launchy_qms} DESTINATION ${PROJECT_BINARY_DIR}/../${BIN_INSTALL_DIR}/tr)
endif()
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../skins DESTINATION ${PROJECT_BINARY_DIR}/../${RES_INSTALL_DIR})
