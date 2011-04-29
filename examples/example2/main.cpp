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

    // Set the clear color for when we re-draw the scene
    glClearColor(0.1, 0.1, 0.1, 1.0);

    // Initialize our model's transformation matrix
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::rotate(model_matrix, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // Initialize our projection matrix (Gives the world a perspective feel rather than orthographic)
    projection_matrix = glm::mat4(1.0f);
    projection_matrix *= glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);

    // Initialize our view matrix, essentially the camera or "eye" orientation in space
    view_matrix = glm::mat4(1.0f);
    view_matrix *= glm::lookAt(
            glm::vec3(1.0, 3.0f, 5.0f),    // The eye's position in 3d space
            glm::vec3(0.0f, 0.0f, 0.0f),   // What the eye is looking at
            glm::vec3(0.0f, 1.0f, 0.0f));  // The eye's orientation vector (which way is up)

}

void updateWindow() {

    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Get the size of our SDL window
    int win_x, win_y;
    SDL_GetWindowSize(main_window, &win_x, &win_y);

    // Set the viewport dimentions
    glViewport(0, 0, win_x, win_y);

    // Apply a slight rotation to our cube's model matrix
    model_matrix = glm::rotate(model_matrix, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}


int main(int argc, char **argv) {

    // Initialize our window
    initWindow(640, 480);

    // Create our vertex and index vectors
    std::vector<Vertex> vert_list;
    std::vector<GLushort> index_list;

    // The cube's vertices
    Vertex v0, v1, v2, v3, v4, v5, v6, v7;
    v0.x = -0.5f; v0.y =  0.5f; v0.z =  0.5f; vert_list.push_back(v0);
    v1.x =  0.5f; v1.y =  0.5f; v1.z =  0.5f; vert_list.push_back(v1);
    v2.x =  0.5f; v2.y = -0.5f; v2.z =  0.5f; vert_list.push_back(v2);
    v3.x = -0.5f; v3.y = -0.5f; v3.z =  0.5f; vert_list.push_back(v3);

    v4.x = -0.5f; v4.y =  0.5f; v4.z = -0.5f; vert_list.push_back(v4);
    v5.x =  0.5f; v5.y =  0.5f; v5.z = -0.5f; vert_list.push_back(v5);
    v6.x =  0.5f; v6.y = -0.5f; v6.z = -0.5f; vert_list.push_back(v6);
    v7.x = -0.5f; v7.y = -0.5f; v7.z = -0.5f; vert_list.push_back(v7);

    // The cube's faces (not represented by quads, but rather triangles)
    index_list.push_back(1); index_list.push_back(5); index_list.push_back(6); // Right Face Tri 1
    index_list.push_back(6); index_list.push_back(2); index_list.push_back(1); // Right Face Tri 2

    index_list.push_back(5); index_list.push_back(4); index_list.push_back(7); // Back Face Tri 1
    index_list.push_back(7); index_list.push_back(6); index_list.push_back(5); // Back Face Tri 2

    index_list.push_back(4); index_list.push_back(0); index_list.push_back(3); // Left Face Tri 1
    index_list.push_back(3); index_list.push_back(7); index_list.push_back(4); // Left Face Tri 2

    index_list.push_back(0); index_list.push_back(4); index_list.push_back(5); // Top Face Tri 1
    index_list.push_back(5); index_list.push_back(1); index_list.push_back(0); // Top Face Tri 2

    index_list.push_back(3); index_list.push_back(0); index_list.push_back(1); // Front Face Tri 1
    index_list.push_back(1); index_list.push_back(2); index_list.push_back(3); // Front Face Tri 2

    index_list.push_back(2); index_list.push_back(3); index_list.push_back(7); // Bottom Face Tri 1
    index_list.push_back(7); index_list.push_back(6); index_list.push_back(2); // Bottom Face Tri 2

    // Define some variables for the vao/vbo/shaders
    GLuint vao, vbo_vertices, vbo_indices;
    GLhandleARB shader_program;

    // Create a new vertex array object for the cube and bind it to make it active
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a vertex buffer object for this entity, bind it, and populate it with vertex data
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vert_list.size(), &(vert_list)[0], GL_STATIC_DRAW);

    // Set up the vertex attribute for the vertex position (xyz)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // Create an index buffer object for the cube, bind it, and populate it with index data
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex)*index_list.size(), &(index_list)[0], GL_STATIC_DRAW);

    // Create our shader program
    shader_program = glCreateProgramObjectARB();

    // Create the vertex/fragment shader
    GLuint vert_shader_id = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLuint frag_shader_id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Load the source for our vert/frag shader
    const char* vert_shader_source = loadShader("data/shaders/simple_shader.vert");
    const char* frag_shader_source = loadShader("data/shaders/simple_shader.frag");

    // Assign the shader source files to the shader objects
    glShaderSource(vert_shader_id, 1, &vert_shader_source, NULL);
    glShaderSource(frag_shader_id, 1, &frag_shader_source, NULL);

    // Compile the source files
    glCompileShader(vert_shader_id);
    glCompileShader(frag_shader_id);

    // Bind the attribute at position 0 (see line 154) to the "Vertex" name for the vertex shader
    glBindAttribLocation(shader_program, 0, "Vertex");

    // Bind the first draw buffer to the "FragColor" out variable for the fragment shader
    glBindFragDataLocation(shader_program, GL_DRAW_BUFFER0, "FragColor");

    // Attach the vert/frag shader objects to the shader program
    glAttachObjectARB(shader_program, vert_shader_id);
    glAttachObjectARB(shader_program, frag_shader_id);

    // Link the shader program to finalize attachment process
    glLinkProgram(shader_program);

    // Check to see if there were any errors in compiling the shaders 
    int infologLength = 0;
    glGetObjectParameterivARB(shader_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

    // If there were errors, write them to the console
    if (infologLength > 0) {
        int charsWritten = 0;
        char *infoLog;
        infoLog = new char[infologLength];
        glGetInfoLogARB(shader_program, infologLength, &charsWritten, infoLog);
        std::cout << infoLog << std::endl;
        delete [] infoLog;
    } 

    // Delete our shader source files since we no longer need them
    delete [] vert_shader_source;
    delete [] frag_shader_source;

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

        // Call some generic window update functions
        updateWindow();

        // Bind the "view_matrix" variable in our C++ program to the "View" variable in the shader
        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "View"),
                1,  
                false,
                glm::value_ptr(view_matrix));

        // Bind the "projection_matrix" variable in our C++ program to the "Projection" variable in the shader
        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "Projection"),
                1,  
                false,
                glm::value_ptr(projection_matrix));

        // Bind the "model_matrix" variable in our C++ program to the "Model" variable in the shader
        glUniformMatrix4fv(
                glGetUniformLocation(shader_program, "Model"),
                1,  
                false,
                glm::value_ptr(model_matrix));

        // Make our vertex array active
        glBindVertexArray(vao);

        // Tell the renderer to use our shader program when rendering our object
        glUseProgramObjectARB(shader_program);

        // Render the vao on the screen using "GL_LINE_LOOP"
        glDrawElements(GL_LINE_LOOP, index_list.size(), GL_UNSIGNED_SHORT, (void*)0);

        // All the previous rendering was done on a buffer that's not being displayed on the screen.
        // SDL_GL_SwapWindow displays that buffer in our window.
        SDL_GL_SwapWindow(main_window);

        // Pause the application for a few milliseconds
        SDL_Delay(10);
    }

    // Clean up the stuff we created
    glUseProgramObjectARB(shader_program);
    glDisableVertexAttribArray(0);
    glDetachShader(shader_program, vert_shader_id);
    glDetachShader(shader_program, frag_shader_id);
    glDeleteProgram(shader_program);
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_indices);
    glDeleteVertexArrays(1, &vao);

    //Deinit SDL
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

