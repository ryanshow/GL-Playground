#version 150 core

uniform sampler2D rgba_tex;
uniform sampler2D norm_tex;
in vec2 TexCoord;
out vec4 FragColor;

void main(void) {
    vec3 normal = normalize(texture2D(norm_tex, TexCoord).rgb * 2.0 - 1.0);
    vec3 light_pos = normalize(vec3(1.0, 1.0, 1.5));
    float diffuse = max(dot(normal, light_pos), 0.0);
    vec3 color = diffuse * texture2D(rgba_tex, TexCoord).rgb;
    FragColor = vec4(color, 1.0);
}

