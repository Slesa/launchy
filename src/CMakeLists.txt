cmake_minimum_required (VERSION 2.8.12)
project (Launchy)
set (Launchy_VERSION_MAJOR 3)
set (Launchy_VERSION_MINOR 0)

configure_file (
	"${PROJECT_SOURCE_DIR}/version.h.in"
	"${PROJECT_SOURCE_DIR}/version.h"
	)

add_subdirectory(common)
add_subdirectory(libqxt)
add_subdirectory(launchy)
# add_executable(Launchy main.cpp)
