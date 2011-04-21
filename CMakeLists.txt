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

add_subdirectory(Example1)
