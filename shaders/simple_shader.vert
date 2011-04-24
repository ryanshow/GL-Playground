#version 150 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
in vec3 Vertex;

void main(void) {
    gl_Position = Projection * ((Model * View) * vec4(Vertex, 1.0));
}
