
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PLUGINS_DIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_PLUGINS_DIR})
set(CMAKE_MACOSX_RPATH 0)

add_subdirectory(runner)
add_subdirectory(verby)
add_subdirectory(weby)

if(__WIN__)
	add_subdirectory(controly)
endif()