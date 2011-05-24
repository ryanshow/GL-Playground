cmake_minimum_required (VERSION 2.6)

project (Example4)

add_executable(Example4 main.cpp)

target_link_libraries (
    Example4
    GLPlayground
    lodepng
    yaml-cpp
    ${PLATFORM_LIBS}
    ${SDL_LIBRARY} SDLmain
    ${OPENGL_LIBS}
)
