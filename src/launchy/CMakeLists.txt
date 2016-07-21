cmake_minimum_required (VERSION 2.8.12)
project (launchy)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets Network)
find_package(Qt5LinguistTools)
if(__LIN__)
    find_package(X11 REQUIRED)
elseif(__MAC__)
    find_package(X11 REQUIRED)
elseif(__BSD__)
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
if(__WIN__)
	set(launchy_win_srcs
		win_launchywidget.cpp
		win_launchywidget.h
	)
    find_package(Qt5 COMPONENTS winextras)
    set(launchy_mods
        WinExtras
    )
endif()

QT5_WRAP_UI(launchy_form_hdr options.ui)
QT5_ADD_TRANSLATION(launchy_qms ${launchy_trans})

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/libqxt)
#if (__LIN__)
	find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
#endif(__LIN__)

add_executable(launchy ${launchy_srcs} ${launchy_form_hdr} ${launchy_common_hdrs} ${launchy_hdr_moc} ${launchy_qms} ${launchy_win_srcs})

qt5_use_modules(launchy Core Gui Widgets Network ${launchy_mods})

target_link_libraries(launchy common libqxt)
if(__LIN__)
    target_link_libraries(launchy ${X11_LIBRARIES})
elseif (__BSD__)
    target_link_libraries(launchy ${X11_LIBRARIES})
elseif (__MAC__)
	target_link_libraries(launchy ${CARBON_FRAMEWORK})
endif()

if(EXISTS ${PROJECT_BINARY_DIR}/launchy_fr.qm)
    file(COPY ${launchy_qms} DESTINATION ${PROJECT_BINARY_DIR}/../${BIN_INSTALL_DIR}/tr)
endif()
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../skins DESTINATION ${PROJECT_BINARY_DIR}/../${RES_INSTALL_DIR})

get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtGui_location Qt5::Gui LOCATION)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
get_target_property(QtNetwork_location Qt5::Network LOCATION)

target_link_libraries(launchy common libqxt)
install(TARGETS launchy RUNTIME DESTINATION bin)
install(FILES 
	${QtCore_location}
	${QtGui_location}
	${QtWidgets_location}
	${QtNetwork_location}
	DESTINATION bin)
install(FILES ${launchy_qms} DESTINATION tr)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../skins DESTINATION skins)
