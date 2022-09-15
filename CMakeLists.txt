cmake_minimum_required(VERSION 3.22)
project(MEDGraphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(LIBS
        libglfw3.a
        OpenGL32.Lib
        glew/Release/x64/glew32.lib
        glew/Release/x64/glew32s.lib)
list(TRANSFORM LIBS PREPEND ${CMAKE_CURRENT_LIST_DIR}/libs/)
file(
        COPY ${CMAKE_CURRENT_LIST_DIR}/bin/Release/x64/glew32.dll
        DESTINATION ${CMAKE_BINARY_DIR}/build)

include_directories(include)

add_subdirectory(src)