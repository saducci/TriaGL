#version 330 core
layout(location = 0) in vec3 aPos;

layout(location = 1) in vec3 aColor;
layout(location = 2 ) in vec2 aTexCord;

out vec3 vertexColor;
uniform float u_localyoffset;
uniform float u_localxoffset;


out vec2 TexCord;

void main() {
    gl_Position = vec4(aPos.x +u_localxoffset, aPos.y+u_localyoffset, aPos.z, 1.0);
    vertexColor = aColor;
    TexCord = aTexCord; 
}
