#ifndef VERTEX_H
#define VERTEX_H

#include <GL/glew.h>

typedef struct {
    GLfloat x, y, z;
    GLfloat nx, ny, nz;
    GLfloat r, g, b, a;
    GLfloat u0, v0;
    GLfloat u1, v1;
    GLfloat u2, v2;
} Vertex;

#endif
