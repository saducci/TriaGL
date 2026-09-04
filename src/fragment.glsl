#version 330 core
in vec3 vertexColor;
in vec2 TexCord;
out vec4 FragColor;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;

void main() {
    FragColor = mix(texture(texture0, TexCord), texture(texture1, TexCord),mixValue );
}
