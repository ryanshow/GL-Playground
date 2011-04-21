cmake_minimum_required (VERSION 2.6)

project (Example1)

include_directories(${SDL_INCLUDE_DIR})

link_libraries (
    ${PLATFORM_LIBS}
    ${SDL_LIBRARY} SDLMain
    ${OPENGL_LIBS}
)

add_executable(Example1 main.cpp)
