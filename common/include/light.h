#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

#include "glm/glm.hpp"

typedef struct {
    glm::vec3 pos;

    glm::vec4 color;
    GLfloat intensity;
    
} Light;

#endif
