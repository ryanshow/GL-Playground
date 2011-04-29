cmake_minimum_required (VERSION 2.6)

project (Example1)

link_libraries (
    ${PLATFORM_LIBS}
    ${SDL_LIBRARY} SDLmain
    ${OPENGL_LIBS}
)

add_executable(Example1 main.cpp)
