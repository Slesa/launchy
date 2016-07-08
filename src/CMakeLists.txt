cmake_minimum_required (VERSION 2.8.12)
project (LaunchyApp)

# // http://www.executionunit.com/blog/2014/01/22/moving-from-qmake-to-cmake/

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set (CMAKE_PREFIX_PATH $ENV{QTDIR})
    set(__WIN__ TRUE)
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(__LIN__ TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(__MAC__ TRUE)
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(__MAC__)
	set(BUNDLE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
	set(BUNDLE_NAME "Launchy.app")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
	set(BIN_INSTALL_DIR "${BUNDLE_NAME}/Contents/MacOS")
    set(RES_INSTALL_DIR "${BUNDLE_NAME}/Contents/Resources")

	#set(CMAKE_FIND_FRAMEWORK ONLY)
	find_library(CARBON_FRAMEWORK Carbon)
else()
	set(BIN_INSTALL_DIR "bin")
    set(RES_INSTALL_DIR "bin/Resources")
endif()
	

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${BIN_INSTALL_DIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_INSTALL_DIR})
#set(BIN_PLUGINS_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/plugins")


if(__LIN__)
    add_definitions(-DPREFIX="/usr")
    add_definitions(-DSKINS_PATH="$$PREFIX/share/launchy/skins/")
    add_definitions(-DPLUGINS_PATH="$$PREFIX/lib/launchy/plugins/")
    add_definitions(-DPLATFORMS_PATH="$$PREFIX/lib/launchy/")
endif()

# Qt
find_package(Qt5Core REQUIRED)
find_package(Qt5 COMPONENTS Gui Network Qml Quick Test Svg)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/common)
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libqxt)

add_subdirectory(common)
add_subdirectory(libqxt)
add_subdirectory(plugins)
add_subdirectory(launchy)
