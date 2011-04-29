cmake_minimum_required (VERSION 2.6)

project (Example3)

add_executable(Example3 main.cpp)

target_link_libraries (
    Example3
    GLPlayground
    lodepng
    ${PLATFORM_LIBS}
    ${SDL_LIBRARY} SDLmain
    ${OPENGL_LIBS}
)
