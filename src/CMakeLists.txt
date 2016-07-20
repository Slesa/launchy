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
elseif (${CMAKE_SYSTEM_NAME} MATCHES "BSD")
	set(__BSD__ TRUE)
endif()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
enable_testing()

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
elseif(__BSD__)
	add_definitions(-DPREFIX="/usr/local")
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
add_subdirectory(launchy.specs)


set(CPACK_PACKAGE_NAME, "Launchy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Launchy application starter")
set(CPACK_PACKAGE_VENDOR "joerg.preiss@slesa.de")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Readme.pdf")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_BUNDLE_NAME, "Launchy")
set(CPACK_BUNDLE_ICON, "${CMAKE_CURRENT_SOURCE_DIR}/Launchy.ico")
if(__WIN__)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
    set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(__MAC__)
    set(CPACK_GENERATOR "Bundle")
    set(CPACK_PACKAGE_VERSION, "1.0")
    set(CPACK_PACKAGE_FILE_NAME, "Launchy-Setup")
    set(CPACK_PACKAGE_ICON, "${CMAKE_CURRENT_SOURCE_DIR}/Launchy.ico")
    set(CPACK_BUNDLE_PLIST, "${CMAKE_CURRENT_SOURCE_DIR}/../mac/launchy.plist")
    set(CPACK_BUNDLE_STARTUP_COMMAND, "Contents/MacOS/Launchy")
elseif(__LIN__)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Doria") #required 
elseif(__BSD__)
    set(CPACK_STRIP_FILES "bin/MyExecutable")
    set(CPACK_SOURCE_STRIP_FILES "")
endif()
#set(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")


#install(PROGRAMS launchy/launchy ${CMAKE_CURRENT_BINARY_DIR}/launchy/launchy
#  DESTINATION bin)

#set(CPACK_COMPONENTS_ALL ${PROGRAMS})

include(CPack)

# http://yanivresearch.info/software/CPackWindowsTutorial/CPackForWindows.html
# https://schneide.wordpress.com/2013/02/11/build-a-rpm-package-using-cmake/
# http://blog.beuc.net/posts/Build_a_SFML_project_with_CMake/