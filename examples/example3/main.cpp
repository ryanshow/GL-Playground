#include <fstream>
#include <iostream>
#include <vector>

#include <SDL.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lodepng.h"

#include "yaml-cpp/yaml.h"

#include "light.h"
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
            glm::vec3(1.0, 0.0f, 3.0f),    // The eye's position in 3d space
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
    model_matrix = glm::rotate(model_matrix, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
}


int main(int argc, char **argv) {

    // Initialize our window
    initWindow(640, 480);

    // Create our vertex and index vectors
    std::vector<Vertex> vert_list;
    std::vector<GLushort> index_list;

    // Load up our model file
    std::ifstream model_file("data/models/cube.yml");
    YAML::Parser yaml_parser(model_file);

    YAML::Node doc;
    yaml_parser.GetNextDocument(doc);
    const YAML::Node &mesh = doc["mesh"];

    // Populate vertices (we assume xyz and uv are always there)
    const YAML::Node &vertices = mesh["vertices"];
    for (int i=0; i < vertices.size(); i++) {
        int index;
        vertices[i]["index"] >> index;

        Vertex vert;
        vertices[i]["pos"][0] >> vert.x;
        vertices[i]["pos"][1] >> vert.y;
        vertices[i]["pos"][2] >> vert.z;
        
        vertices[i]["tex"][0] >> vert.u0;
        vertices[i]["tex"][1] >> vert.v0;

        vert_list.push_back(vert);
    }

    // Populate the indices (We assume triangles everywhere)
    const YAML::Node &indices = mesh["indices"];
    for (int i=0; i < indices.size(); i++) {
        int i0, i1, i2;
        indices[i][0] >> i0;
        indices[i][1] >> i1;
        indices[i][2] >> i2;

        index_list.push_back(i0);
        index_list.push_back(i1);
        index_list.push_back(i2);
    }

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

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u0));

    // Create an index buffer object for the cube, bind it, and populate it with index data
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index_list.size(), &(index_list)[0], GL_STATIC_DRAW);

    // Create our shader program
    shader_program = glCreateProgramObjectARB();

    // Create the vertex/fragment shader
    GLuint vert_shader_id = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLuint frag_shader_id = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Load the source for our vert/frag shader
    const char* vert_shader_source = loadShader("data/shaders/1rgba_norm.vert");
    const char* frag_shader_source = loadShader("data/shaders/1rgba_norm.frag");

    // Assign the shader source files to the shader objects
    glShaderSource(vert_shader_id, 1, &vert_shader_source, NULL);
    glShaderSource(frag_shader_id, 1, &frag_shader_source, NULL);

    // Compile the source files
    glCompileShader(vert_shader_id);
    glCompileShader(frag_shader_id);

    // Bind the attribute at position 0 (see line 154) to the "Vertex" name for the vertex shader
    glBindAttribLocation(shader_program, 0, "Vertex");
    glBindAttribLocation(shader_program, 1, "TexCoord0");

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

    // Load up image textures 
    GLuint brick_tex, brick_normal_tex;
    std::vector<unsigned char> buffer, image;
    LodePNG::Decoder decoder;

    LodePNG::loadFile(buffer, "data/images/brick.png");
    decoder.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());
    glGenTextures(1, &brick_tex);
    glBindTexture(GL_TEXTURE_2D, brick_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decoder.getWidth(), decoder.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image)[0]);

    buffer.clear();
    image.clear();

    LodePNG::Decoder decoder2;

    LodePNG::loadFile(buffer, "data/images/brick_normal.png");
    decoder2.decode(image, buffer.empty() ? 0 : &buffer[0], (unsigned)buffer.size());
    glGenTextures(1, &brick_normal_tex);
    glBindTexture(GL_TEXTURE_2D, brick_normal_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, decoder2.getWidth(), decoder2.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &(image)[0]);

    buffer.clear();
    image.clear();

    // Create the lights in the scene
    Light light0;
    light0.pos[0] = 1.0f; light0.pos[1] = 0.6f; light0.pos[2] = 0.6f;
    light0.color[0] = 0.9f; light0.color[1] = 1.0f; light0.color[2] = 1.0f; light0.color[3] = 1.0f;
    light0.intensity = 0.5f;


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

        // Active our brick texture and bind it to the "texture1" variable in the shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brick_tex);
        glUniform1i(glGetUniformLocation(shader_program, "texture_0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, brick_normal_tex);
        glUniform1i(glGetUniformLocation(shader_program, "texture_1"), 1);

        glUniform3fv(glGetUniformLocation(shader_program, "light0_pos"), 1, glm::value_ptr(light0.pos));
        glUniform4fv(glGetUniformLocation(shader_program, "light0_col"), 1, glm::value_ptr(light0.color));
        glUniform1f(glGetUniformLocation(shader_program, "light0_int"), light0.intensity);

        // Make our vertex array active
        glBindVertexArray(vao);

        // Tell the renderer to use our shader program when rendering our object
        glUseProgramObjectARB(shader_program);

        // Render the vao on the screen using "GL_LINE_LOOP"
        glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, (void*)0);

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

