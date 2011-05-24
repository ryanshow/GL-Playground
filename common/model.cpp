#include <cstring>
#include <fstream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "model.h"
#include "shader_util.h"
#include "vertex.h"

// Basic constructor that populates the object contents from a YAML file
Model::Model(const char *filename) {
    fromYAML(filename);
}

// Load up this object with the contents from a YAML model file
void Model::fromYAML(const char *filename) {

    // Make sure this object is clean
    cleanUp();

    // Load up the YAML file
    std::ifstream model_file(filename);
    YAML::Parser yaml_parser(model_file);

    YAML::Node doc;
    yaml_parser.GetNextDocument(doc);
    const YAML::Node &mesh = doc["mesh"];

    // Load in the vertices
    const YAML::Node &vertices = mesh["vertices"];
    for (int i=0; i < vertices.size(); i++) {
        // FIXME: Ensure the index order is correct
        int index;
        vertices[i]["index"] >> index;

        Vertex vert;
        vertices[i]["pos"][0] >> vert.x;
        vertices[i]["pos"][1] >> vert.y;
        vertices[i]["pos"][2] >> vert.z;

        vertices[i]["tex"][0] >> vert.u0;
        vertices[i]["tex"][1] >> vert.v0;

        vertex_list.push_back(vert);
    }

    // Load in the indices
    const YAML::Node &indices = mesh["indices"];
    for (int i=0; i < indices.size(); i++) {
        // XXX: Shorten this
        int i0, i1, i2;
        indices[i][0] >> i0;
        indices[i][1] >> i1;
        indices[i][2] >> i2;

        index_list.push_back(i0);
        index_list.push_back(i1);
        index_list.push_back(i2);
    }

    // Load the shader filenames
    const YAML::Node &shaders = mesh["shaders"];

    std::string vert_shader_filename = "simple_shader.vert";
    std::string frag_shader_filename = "simple_shader.frag";
    std::string geom_shader_filename = "simple_shader.geom";

    if (const YAML::Node *vert_shader = shaders.FindValue("vertex")) {
        *vert_shader >> vert_shader_filename;
    }
    if (const YAML::Node *frag_shader = shaders.FindValue("fragment")) {
        *frag_shader >> frag_shader_filename;
    }
    if (const YAML::Node *geom_shader = shaders.FindValue("geometry")) {
        *geom_shader >> geom_shader_filename;
    }

    // Create the shader program
    shader_program = glCreateProgramObjectARB();

    // Create the vertex/fragment shaders
    shader_map[VERTEX] = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    shader_map[FRAGMENT] = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    // Load the source for the vertex/fragment shaders
    const char *shader_path = "data/shaders/";

    char *vert_shader_fullpath = new char[vert_shader_filename.size()+strlen(shader_path)+1];
    char *frag_shader_fullpath = new char[frag_shader_filename.size()+strlen(shader_path)+1];
    char *geom_shader_fullpath = new char[geom_shader_filename.size()+strlen(shader_path)+1];

    strcpy(vert_shader_fullpath, shader_path);
    strcat(vert_shader_fullpath, vert_shader_filename.c_str());

    strcpy(frag_shader_fullpath, shader_path);
    strcat(frag_shader_fullpath, frag_shader_filename.c_str());

    strcpy(geom_shader_fullpath, shader_path);
    strcat(geom_shader_fullpath, geom_shader_filename.c_str());

    const char *vert_shader_source = loadShader(vert_shader_fullpath);
    const char *frag_shader_source = loadShader(frag_shader_fullpath);
    //const char *geom_shader_source = loadShader(geom_shader_fullpath);
    
    // Assign the shader source to the shader objects
    glShaderSource(shader_map[VERTEX], 1, &vert_shader_source, NULL);
    glShaderSource(shader_map[FRAGMENT], 1, &frag_shader_source, NULL);

    // Compile the shader files
    glCompileShader(shader_map[VERTEX]);
    glCompileShader(shader_map[FRAGMENT]);

    // Bind vertex attributes to the vertex shader
    glBindAttribLocation(shader_program, 0, "Vertex");
    glBindAttribLocation(shader_program, 1, "TexCoord0");

    // Bind the first draw buffer to the "FragColor" out variable for the frag shader
    glBindFragDataLocation(shader_program, GL_DRAW_BUFFER0, "FragColor");

    // Attach the vert/frag shader objects to the shader program
    glAttachObjectARB(shader_program, shader_map[VERTEX]);
    glAttachObjectARB(shader_program, shader_map[FRAGMENT]);

    // Link the shader program to finalize the attachment process
    glLinkProgram(shader_program);

    // Create the Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a vertex buffer object for this entity, bind it, and populate it with vertex data
    glGenBuffers(2, buffer_ids);
    buffer_map[VERTEX_BUFFER] = buffer_ids[0];
    buffer_map[INDEX_BUFFER] = buffer_ids[1];

    glBindBuffer(GL_ARRAY_BUFFER, buffer_map[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertex_list.size(), &(vertex_list)[0], GL_STATIC_DRAW);

    // Set up the vertex attribute for the vertex position (xyz)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // Set up the texcoord attribute for the vertex texture position (uv)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u0));

    // Create an index buffer object for the cube, bind it, and populate it with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_map[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*index_list.size(), &(index_list)[0], GL_STATIC_DRAW);

}

// Erase the contents of this object, essentially making it a blank slate
void Model::cleanUp() {
    glUseProgramObjectARB(shader_program);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Clean up the shaders
    std::map<shader_types,GLuint>::iterator shader_map_it;
    for (shader_map_it = shader_map.begin(); shader_map_it != shader_map.end(); shader_map_it++) {
        glDetachShader(shader_program, shader_map_it->second);
        glDeleteShader(shader_map_it->second);
    }
    shader_map.clear();

    // Delete the shader program
    glDeleteProgram(shader_program);

    // Delete our buffer objects
    glDeleteBuffers(buffer_map.size(), buffer_ids);

    // Delete our array objects
    glDeleteVertexArrays(1, &vao);

    // Clear the vertex/index vectors
    vertex_list.clear();
    index_list.clear();
    
    // Delete the textures
    glDeleteTextures(texture_map.size(), texture_ids);
    texture_map.clear();
    texture_ids = NULL;
}
