#include <fstream>
#include <map>

#include "yaml-cpp/yaml.h"

#include "model.cpp"
#include "vertex.cpp"

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
        vertices[i]["tex"][0] >> vert.v0;

        vert_list.push_back(vert);
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
}

// Erase the contents of this object, essentially making it a blank slate
void Model::cleanUp() {
    glUseProgramObjectARB(shader_program);

    //glDisableVertexAttribArray(0);

    std::map<shader_type,GLuint>::iterator shader_map_it;
    for (shader_map_it = shader_map.begin(); shader_map_it != shader_map.end(); shader_map_it++) {
        glDetachShader(shader_program, shader_map_it.second);
        glDeleteShader(shader_map_it.second);
    }
    glDeleteProgram(shader_program);

    glDeleteBuffers(buffer_map.size(), &buffer_ids);

    glDeleteVertexArrays(1, &vao);

    vert_list.clear();
    index_list.clear();
    
    // Delete the textures
    glDeleteTextures(texture_map.size(), &texture_ids);
    texture_map.clear();
    texture_ids = NULL;

    // Delete the shaders
    std::map<shader_types,GLuint>::iterator shader_map_it;
    for (shader_map_it=shader_map.begin(); shader_map_it != shader_map.end(); shader_map_it++) {
        glDeleteShader(shader_map_it.first);
    }
    shader_map.clear();
}
