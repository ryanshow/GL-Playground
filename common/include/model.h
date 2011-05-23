#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <vector>

#include <GL/glew.h>

#include "vertex.h"

enum shader_types {VERTEX, FRAGMENT, GEOMETRY};
enum texture_types {TEX_RGBA_1, TEX_RGBA_2, TEX_RGBA_3, TEX_NORM};

class Model {
    public:
        Model() {};
        Model(const char *filename);

        void fromYAML(const char *filename);

        std::vector<Vertex> vertex_list;
        std::vector<unsigned int> index_list;

        // Shader indices
        GLhandleARB shader_program;
        std::map<shader_types,GLuint> shader_map;

        // Texture indices
        GLuint *texture_ids;
        std::map<texture_types,GLuint> texture_map;

        // Buffer/Array Object 
        GLuint vao;
        GLuint *buffer_ids;
        std::map<buffer_types,GLuint> buffer_map;

    protected:
        void cleanUp();

};

#endif
