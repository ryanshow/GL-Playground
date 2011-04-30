#version 150 core

uniform sampler2D texture1;
in vec2 TexCoord;
out vec4 FragColor;

void main(void) {
    FragColor = texture2D(texture1, TexCoord).rgba;
}

