#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <vector>

#include <GL/glew.h>

#include "vertex.h"

enum shader_types {VERTEX, FRAGMENT, GEOMETRY};
enum buffer_types {VERTEX_BUFFER, INDEX_BUFFER};

class Model {
    public:
        Model() {};
        Model(const char *filename);

        void fromYAML(const char *filename);

        std::vector<Vertex> vertex_list;
        std::vector<GLushort> index_list;

        // Shader indices
        GLhandleARB shader_program;
        std::map<shader_types,GLuint> shader_map;

        // Texture indices
        GLuint *texture_ids;
        GLsizei texture_count;

        // Buffer/Array Object 
        GLuint vao;
        GLuint *buffer_ids;
        std::map<buffer_types,GLuint> buffer_map;

    protected:
        void cleanUp();
};

#endif
