cmake_minimum_required (VERSION 2.6)

project (Example2)

add_executable(Example2 main.cpp)

target_link_libraries (
    Example2
    GLPlayground
    ${PLATFORM_LIBS}
    ${SDL_LIBRARY} SDLmain
    ${OPENGL_LIBS}
)
