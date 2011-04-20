#include <SDL.h>
#include <GL/glew.h>

int main(int argc, char **argv) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    // Set up the OpenGL context version (3.2)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Enable double bufferign in our to-be window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // The SDL Window
    SDL_Window *main_window = SDL_CreateWindow(
            "Hello, OpenGL 3.2 Core Profile",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480,
            SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

    // Create the OpenGL Context and assign it to the main window
    SDL_GLContext main_context = SDL_GL_CreateContext(main_window);

    // Enable Vertical Sync
    SDL_GL_SetSwapInterval(1);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glew_err = glewInit();
    if (GLEW_OK != glew_err) {
        std::cout << "Error: " << glewGetErrorString(glew_err) << std::endl;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // The main game loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_CLOSE:
                            running = false;
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                    }
                    break;
            }
        }

        SDL_GL_SwapWindow(main_window);
        SDL_Delay(10);
    }

    //Deinit SDL
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

