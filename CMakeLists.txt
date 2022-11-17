cmake_minimum_required(VERSION 3.22)
project(MEDGraphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(LIBS
        libglfw3.a
        OpenGL32.Lib
        win64/glew32.lib
        win64/glew32s.lib
        win64/freetype.lib)
list(TRANSFORM LIBS PREPEND ${CMAKE_CURRENT_LIST_DIR}/libs/)
set(BINS glew32.dll freetype.dll)
list(TRANSFORM BINS PREPEND bin/win64/)
file(
        COPY ${BINS}
        DESTINATION ${CMAKE_BINARY_DIR}/build)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_BINARY_DIR}/build/assets
        )

include_directories(include)

add_subdirectory(view-lib/src)
add_subdirectory(src)
add_subdirectory(tools)