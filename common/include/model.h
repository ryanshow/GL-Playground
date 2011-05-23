#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "vertex.h"

class Model {
    public:
        Model() {};
        Model(const char *filename);

        void fromYAML(const char *filename);

        std::vector<Vertex> *getVerts();
        std::vector<unsigned int> *getIndices();
    
    protected:
        void cleanUp(); 

        std::vector<Vertex> vertex_list;
        std::vector<unsigned int> index_list;
};

#endif
