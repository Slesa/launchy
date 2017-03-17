cmake_minimum_required (VERSION 3.5.1)
project (LaunchyApp)

# // http://www.executionunit.com/blog/2014/01/22/moving-from-qmake-to-cmake/

set(PROJECT_VERSION_MAJOR 3)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set (CMAKE_AUTOMOC OFF)

configure_file (
	"${PROJECT_SOURCE_DIR}/win/launchy.rc.in"
	"${PROJECT_SOURCE_DIR}/win/launchy.rc"
	)
configure_file (
	"${PROJECT_SOURCE_DIR}/common/version.h.in"
	"${PROJECT_SOURCE_DIR}/common/version.h"
	)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set (CMAKE_PREFIX_PATH $ENV{QTDIR})
	set(__WIN__ TRUE)
	ADD_DEFINITIONS(-DUNICODE)
	ADD_DEFINITIONS(-D_UNICODE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(__LIN__ TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message (STATUS "In Mac mode")
	set(__MAC__ TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "BSD")
	set(__BSD__ TRUE)
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
elseif(__LIN__ OR __BSD__)
	set(BIN_INSTALL_DIR "bin")
    set(RES_INSTALL_DIR "bin/Resources")
endif()
	

if(__LIN__)
	add_definitions(-DPREFIX="/usr")
	add_definitions(-DSKINS_PATH="$$PREFIX/share/launchy/skins/")
	add_definitions(-DPLUGINS_PATH="$$PREFIX/lib/launchy/plugins/")
	add_definitions(-DPLATFORMS_PATH="$$PREFIX/lib/launchy/")
	set(PREFIX "/usr")
	set(PREFIX_BIN ${PREFIX}/bin)
	set(PREFIX_PLUGINS ${PREFIX}/lib/launchy)
	set(PREFIX_SKINS ${PREFIX}/share/launchy)
	set(PREFIX_ICONS ${PREFIX}/share/launchy/plugins)
	set(PREFIX_DESKTOP ${PREFIX}/share/applications)
	set(PREFIX_DOC ${PREFIX}/share/doc/launchy)
	set(PREFIX_MENU ${PREFIX}/share/menu)
	set(PREFIX_PIXMAPS ${PREFIX}/share/pixmaps)
elseif(__BSD__)
	add_definitions(-DPREFIX="/usr/local")
	add_definitions(-DSKINS_PATH="$$PREFIX/share/launchy/skins/")
	add_definitions(-DPLUGINS_PATH="$$PREFIX/lib/launchy/plugins/")
	add_definitions(-DPLATFORMS_PATH="$$PREFIX/lib/launchy/")
	set(PREFIX "/usr/local")
	set(PREFIX_BIN ${PREFIX}/bin)
	set(PREFIX_PLUGINS ${PREFIX}/lib/launchy)
	set(PREFIX_SKINS ${PREFIX}/share/launchy)
	set(PREFIX_ICONS ${PREFIX}/share/launchy/plugins)
	set(PREFIX_DESKTOP ${PREFIX}/share/applications)
	set(PREFIX_DOC ${PREFIX}/share/doc/launchy)
	set(PREFIX_MENU ${PREFIX}/share/menu)
	set(PREFIX_PIXMAPS ${PREFIX}/share/pixmaps)
else()
	message (STATUS "Windows or Mac")
	set(PREFIX_BIN .)
	set(PREFIX .)
	set(PREFIX_PLUGINS ${PREFIX})
	set(PREFIX_SKINS ${PREFIX})
	set(PREFIX_ICONS ${PREFIX})
	set(PREFIX_DESKTOP ${PREFIX})
	set(PREFIX_DOC ${PREFIX}/doc)
	set(PREFIX_MENU ${PREFIX})
	set(PREFIX_PIXMAPS ${PREFIX})
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

enable_testing()
add_subdirectory(launchy.specs)


set(CPACK_PACKAGE_NAME, "Launchy")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Launchy application starter")
set(CPACK_PACKAGE_VENDOR "Code Jelly")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Readme.pdf")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Launchy")
# set(CPACK_COMPONENTS_GROUPING "IGNORE")
set(CPACK_COMPONENTS_IGNORE_GROUPS 1)
set(CPACK_BUNDLE_NAME, "Launchy")
set(CPACK_BUNDLE_ICON, "${CMAKE_CURRENT_SOURCE_DIR}/Launchy.ico")
if(__WIN__)
	# There is a bug in NSI that does not handle full unix paths properly. Make
	# sure there is at least one set of four (4) backlasshes.
	set(CPACK_PACKAGE_FILE_NAME, "Launchy-Setup-${PROJECT_VERSION}")
	set(CPACK_PACKAGE_ICON, "${CMAKE_CURRENT_SOURCE_DIR}/Launchy.ico")
#    set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "launchy.exe")
    set(CPACK_NSIS_DISPLAY_NAME "Launchy")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.launchy.net")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.launchy.net\\\\about.php")
    set(CPACK_NSIS_CONTACT "joerg.preiss@slesa.de")
    set(CPACK_NSIS_MODIFY_PATH OFF)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "")
    SET(CPACK_NSIS_CREATE_ICONS_EXTRA 
        "
            CreateShortcut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Launchy.lnk\\\" \\\"$INSTDIR\\\\launchy.exe\\\"
        " )
    SET(CPACK_NSIS_DELETE_ICONS_EXTRA 
        "
            Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Launchy.lnk\\\"
        " )
elseif(__MAC__)
    message (STATUS "Bundle name is" ${CPACK_BUNDLE_NAME})
    message (STATUS "Package name is" ${CPACK_PACKAGE_NAME})
	set(CPACK_GENERATOR "Bundle")
    set(CPACK_BUNDLE_NAME, "Launchy")
	set(CPACK_PACKAGE_VERSION, "1.0")
	set(CPACK_PACKAGE_FILE_NAME, "Launchy-Setup")
	set(CPACK_PACKAGE_ICON, "${CMAKE_CURRENT_SOURCE_DIR}/Launchy.ico")
	set(CPACK_BUNDLE_PLIST, "${CMAKE_CURRENT_SOURCE_DIR}/../mac/launchy.plist")
	set(CPACK_BUNDLE_STARTUP_COMMAND, "Contents/MacOS/Launchy")
elseif(__LIN__)
	set(CPACK_GENERATOR "DEB;RPM")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "David Doria") 
	set(CPACK_PACKAGE_CONTACT "joerg.preiss@slesa.de")
	set(CPACK_DEB_COMPONENT_INSTALL ON)
	set(CPACK_DEB_USE_DISPLAY_NAME_IN_FILENAME, on)
	set(CPACK_RPM_PACKAGE_NAME "launchy")
	set(CPACK_RPM_PACKAGE_SUMMARY "Launchy is a free cross-platform utility designed to help you forget about your start menu, the icons on your desktop, and even your file manager. Launchy indexes the programs in your start menu and can launch your documents, project files, folders, and bookmarks with just a few keystrokes!")
	set(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VERSION})
	set(CPACK_RPM_PACKAGE_RELEASE 1)
	set(CPACK_RPM_PACKAGE_LICENSE "unknown")
	set(CPACK_RPM_PACKAGE_GROUP "unknown")
	#set(CPACK_RPM_PACKAGE_REQUIRES "qt5" )
	set(CPACK_PACKAGE_FILE_NAME "launchy-${PROJECT_VERSION}")
	#required 
	#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5gui5, libqt5core libqt5network5 libqt5widgets5 libqt5x11extras5")
elseif(__BSD__)
	#set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
	#set(CPACK_STRIP_FILES "bin/Launchy")
	set(CPACK_SOURCE_STRIP_FILES "")
endif()
# set(CPACK_PACKAGE_EXECUTABLES launchy "Launchy")
set(CPACK_COMPONENT_launchy_DISPLAY_NAME "launchy")
set(CPACK_COMPONENT_skins_DISPLAY_NAME "launchy-skins")
set(CPACK_COMPONENT_skins_DEPENDS launchy)
set(CPACK_COMPONENT_plugins_DISPLAY_NAME "launchy-plugins")
set(CPACK_COMPONENT_plugins_DEPENDS launchy)
set(CPACK_COMPONENTS_ALL launchy plugins skins)

include(CPack)

# http://yanivresearch.info/software/CPackWindowsTutorial/CPackForWindows.html
# https://schneide.wordpress.com/2013/02/11/build-a-rpm-package-using-cmake/
# http://blog.beuc.net/posts/Build_a_SFML_project_with_CMake/
# https://github.com/biometrics/openbr/blob/master/share/openbr/example/CMakeLists.txt