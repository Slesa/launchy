
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PLUGINS_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_PLUGINS_DIR})

add_subdirectory(runner)
add_subdirectory(verby)
add_subdirectory(weby)

if(__WIN__)
	add_subdirectory(controly)
endif()