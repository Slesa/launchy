cmake_minimum_required (VERSION 2.8.12)
project (launchy)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Gui Widgets Network)

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

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/libqxt)

add_executable(launchy ${launchy_srcs} ${launchy_form_hdr} ${launchy_common_hdrs} ${launchy_hdr_moc})

qt5_use_modules(launchy Core Gui Widgets Network)

target_link_libraries(launchy common libqxt)
if (__MAC__)
	target_link_libraries(launchy ${CARBON_FRAMEWORK})
endif()