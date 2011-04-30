#version 150 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
in vec3 Vertex;
in vec2 TexCoord0;
out vec2 TexCoord;

void main(void) {
    gl_Position = Projection * ((View * Model) * vec4(Vertex, 1.0));
    TexCoord = TexCoord0; 
}

