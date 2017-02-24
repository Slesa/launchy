cmake_minimum_required (VERSION 3.5.1)
project (launchy_specs)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Core Gui Widgets Test)
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

enable_testing()

set(launchy_specs_hdrs
    commandlineparser.specs.h
    singleapplication.specs.h
    ../launchy/commandlineparser.h
    ../launchy/singleapplication.h
   )
set(launchy_specs_srcs
	main.cpp
    commandlineparser.specs.cpp
    singleapplication.specs.cpp
    ../launchy/commandlineparser.cpp
    ../launchy/singleapplication.cpp
   )
add_executable(launchy_specs ${launchy_specs_srcs} ${launchy_specs_hdrs})
qt5_use_modules(launchy_specs Core Gui Widgets Test)

# set_target_properties(launchy_specs PROPERTIES 
#    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../${BIN_INSTALL_DIR}/launchy.specs
#    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../${BIN_INSTALL_DIR}/launchy.specs
#    )

# get_target_property(OUTPUT_VALUE launchy_specs RUNTIME_OUTPUT_DIRECTORY)
# message(STATUS "This is the launchy_specs RUNTIME_OUTPUT_DIRECTORY:" ${OUTPUT_VALUE})


add_test( launchy_specs launchy_specs )
#add_test( NAME launchy_specs 
#    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/launchy_specs.exe
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_test( NAME launchy_specs COMMAND "$<TARGET_FILE:launchy_specs>" )
# add_test( NAME launchy_specs COMMAND "../bin/launchy.specs/launchy_specs" )
# add_test( NAME launchy_specs COMMAND ${RUNTIME_OUTPUT_DIRECTORY}/launcy.specs/launchy_specs )

if(__WIN__)
    get_target_property(QtCore_location Qt5::Core LOCATION)
    file(COPY ${QtCore_location} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    get_target_property(QtTest_location Qt5::Test LOCATION)
    file(COPY ${QtTest_location} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

## http://doc.qt.io/qt-5/cmake-manual.html