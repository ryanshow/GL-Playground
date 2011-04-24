#include <iostream>

#include <SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "shader_util.h"

namespace {
    SDL_Window *main_window;
    SDL_GLContext main_context;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
}

void initWindow(int win_x, int win_y) {
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit(1);
    }

    // Set up the OpenGL context version (3.2)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Enable double buffering in our to-be window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // The SDL Window
    main_window = SDL_CreateWindow(
            "Hello, OpenGL 3.2 Core Profile",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            win_x, win_y,
            SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

    // Create the OpenGL Context and assign it to the main window
    main_context = SDL_GL_CreateContext(main_window);

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

    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void updateWindow() {

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    int win_x, win_y;

    SDL_GetWindowSize(main_window, &win_x, &win_y);

    glViewPort(0, 0, win_x, win_y);

    projection_matrix = glm::mat4(1.0f);
    projection_matrix *= glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);

    view_matrix = glm::mat4(1.0f);
    view_matrix *= glm::lookAt(
            glm::vec3(0.0, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
}


int main(int argc, char **argv) {

    initWindow(640, 480);

    GLuint vao, vbo_vertices, vbo_indices;
    GLhandleARB shader_program;

    // Create a new vertex array object for the cube
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a vertex buffer object for this entity
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, SIZE_OF_ARRAY, FIRST_INDEX, GL_STATIC_DRAW);

    // Set up vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    // Create an index buffer object for the cube
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE_OF_ARRAY, FIRST_INDEX, GL_STATIC_DRAW);

    // Load the shader resources
    vert_shader = bleh;
    frag_shader = bleh;

    glAttachObjectARB(shader_program, vert_shader_id);
    glAttachObjectARB(shader_program, frag_shader_id);

    glBindFragDataLocation(shader_program, 0, "FragColor");
    glBindAttribLocation(shader_program, 0, "Vertex");

    glLinkProgram(shader_program);

    const char* vert_shader_source = loadShader("shaders/simple_shader.vert");
    const char* frag_shader_source = loadShader("shaders/simple_shader.frag");


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

        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(main_window);
        SDL_Delay(10);
    }

    //Deinit SDL
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

