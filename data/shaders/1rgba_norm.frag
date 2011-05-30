#version 150 core

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform vec3 light0_pos;
uniform vec4 light0_col;
uniform float light0_int;
in vec2 TexCoord;
out vec4 FragColor;
in vec4 vPos;
in vec4 vNorm;

void main(void) {
    vec3 normal = normalize(texture2D(texture_1, TexCoord.st).rgb * 2.0 - 1.0);
    float diffuse = max(dot(normal, light0_pos), 0.0);
    vec3 color = diffuse * texture2D(texture_0, TexCoord).rgb * light0_col.rgb;
    float r = pow(pow(vPos.x-light0_pos.x, 2)+pow(vPos.y-light0_pos.y, 2)+pow(vPos.z-light0_pos.z,2), 0.5);
    float intensity = (3.0*750)/(4.0*3.14159*r*r*r*r*r);
    FragColor = vec4(color, 1.0) * intensity;
}

