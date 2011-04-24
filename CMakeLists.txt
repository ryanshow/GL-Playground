cmake_minimum_required (VERSION 2.6)

project (GLPlayground)

find_package (SDL REQUIRED)

add_definitions (-DSDL_NO_COMPAT -DGLEW_STATIC)

if (WIN32)
    set (PLATFORM_LIBS msvcrt.lib msimg32.lib version.lib winmm.lib imm32.lib)
    set (OPENGL_LIBS glew32 opengl32 glu32)
endif (WIN32)

if (MSYS)
    set (PLATFORM_LIBS mingw32)
    set (SDL_LIBRARY SDLmain SDL)
    set (SDL_INCLUDE_DIR /usr/local/include/SDL)
endif (MSYS)

if (UNIX)
    set (OPENGL_LIBS GLEW GL GLU)
endif()

include_directories ("${PROJECT_SOURCE_DIR}/common")

set (
    COMMON_SOURCE_FILES
    common/shader_util.cpp
)

set (
    COMMON_HEADER_FILES
    common/shader_util.h
)

file (
    COPY shaders
    DESTINATION ./
)

add_library (GLPlayground STATIC ${COMMON_SOURCE_FILES} ${COMMON_HEADER_FILES})

add_subdirectory(Example1)
add_subdirectory(Example2)
