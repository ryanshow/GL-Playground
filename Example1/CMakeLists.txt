cmake_minimum_required (VERSION 2.6)

project (Example1)

include_directories(${SDL_INCLUDE_DIR})

link_libraries (${SDL_LIBRARY} SDLmain GLEW)

add_executable(Example1 main.cpp)
