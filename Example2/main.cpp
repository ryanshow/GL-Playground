#include <iostream>
#include <vector>

#include <SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_util.h"
#include "vertex.h"

namespace {
    SDL_Window *main_window;
    SDL_GLContext main_context;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 model_matrix;
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

    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::translate(model_matrix, glm::vec3(0.2f, 0.0f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
}

void updateWindow() {

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    int win_x, win_y;

    SDL_GetWindowSize(main_window, &win_x, &win_y);

    glViewport(0, 0, win_x, win_y);

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

    std::vector<Vertex> vert_list;
    std::vector<GLushort> index_list;

    Vertex v1, v2, v3;
    v1.x = 0.0f; v1.y = 0.0f; v1.z = 0.0f;
    v2.x = 0.0f; v2.y = 0.3f; v2.z = 0.0f;
    v3.x = 0.3f; v3.y = 0.3f; v3.z = 0.0f;

    vert_list.push_back(v1);
    vert_list.push_back(v2);
    vert_list.push_back(v3);

    index_list.push_back(0);
    index_list.push_back(1);
    index_list.push_back(2);

    GLuint vao, vbo_vertices, vbo_indices;
    GLhandleARB shader_program;

    // Create a new vertex array object for the cube
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a vertex buffer object for this entity
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vert_list.size(), &(vert_list)[0], GL_STATIC_DRAW);

    // Set up the vertex attribute for the vertex position (xyz)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // Create an index buffer object for the cube
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex)*index_list.size(), &(index_list)[0], GL_STATIC_DRAW);

    shader_program = glCreateProgramObjectARB();

    // Create the shader objects
    GLuint vert_shader_id = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLuint frag_shader_id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    const char* vert_shader_source = loadShader("shaders/simple_shader.vert");
    const char* frag_shader_source = loadShader("shaders/simple_shader.frag");

    // Assign the shader source files to the shader objects
    glShaderSource(vert_shader_id, 1, &vert_shader_source, NULL);
    glShaderSource(frag_shader_id, 1, &frag_shader_source, NULL);

    // Compile the source files
    glCompileShader(vert_shader_id);
    glCompileShader(frag_shader_id);

    glBindAttribLocation(shader_program, 0, "Vertex");
    glBindFragDataLocation(shader_program, GL_DRAW_BUFFER0, "FragColor");

    // Attach the vert/frag shader objects to the shader program
    glAttachObjectARB(shader_program, vert_shader_id);
    glAttachObjectARB(shader_program, frag_shader_id);

    glLinkProgram(shader_program);

    // Check to see if there were any errors in compiling the shaders 
    int infologLength = 0;
    glGetObjectParameterivARB(shader_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

    // If there were errors, write them to glslinfo.txt
    if (infologLength > 0) {
        int charsWritten = 0;
        char *infoLog;
        infoLog = new char[infologLength];
        glGetInfoLogARB(shader_program, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }   

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

        updateWindow();

        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "View"),
                1,  
                false,
                glm::value_ptr(view_matrix));

        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "Projection"),
                1,  
                false,
                glm::value_ptr(projection_matrix));

        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "Model"),
                1,  
                false,
                glm::value_ptr(model_matrix));

        glBindVertexArray(vao);
        glUseProgramObjectARB(shader_program);
        glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, (void*)0);
        SDL_GL_SwapWindow(main_window);
        SDL_Delay(10);
    }

    //Deinit SDL
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

