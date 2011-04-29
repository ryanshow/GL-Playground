cmake_minimum_required (VERSION 2.6)

project (GLPlayground)

find_package (SDL REQUIRED)

add_definitions (-DSDL_NO_COMPAT)

include_directories ("${PROJECT_SOURCE_DIR}/common/include")
include_directories ("${PROJECT_SOURCE_DIR}/extern/glm/include")
include_directories ("${PROJECT_SOURCE_DIR}/extern/glew/include")
include_directories ("${PROJECT_SOURCE_DIR}/extern/lodepng/include")

add_subdirectory (extern/glew)
add_subdirectory (extern/lodepng)

if (WIN32)
    set (PLATFORM_LIBS msvcrt.lib msimg32.lib version.lib winmm.lib imm32.lib)
    set (OPENGL_LIBS opengl32 glu32)
endif (WIN32)

if (MSYS)
    set (PLATFORM_LIBS mingw32)
    set (SDL_LIBRARY SDLmain SDL)
    set (SDL_INCLUDE_DIR /usr/local/include/SDL)
endif (MSYS)

if (UNIX)
    set (OPENGL_LIBS GL GLU)
endif()

set (OPENGL_LIBS ${OPENGL_LIBS} glew)

set (
    COMMON_SOURCE_FILES
    common/shader_util.cpp
)

set (
    COMMON_HEADER_FILES
    common/include/shader_util.h
)

file (
    COPY data
    DESTINATION ./
)

include_directories (${SDL_INCLUDE_DIR})

add_library (GLPlayground STATIC ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES})

add_subdirectory(examples)
