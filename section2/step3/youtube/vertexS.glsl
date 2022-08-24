#version 330 core
layout (location = 0) in vec3 inPos;

uniform float uMoveX;

void main(){
    gl_Position = vec4(inPos + vec3(uMoveX, 0.0f, 0.0f), 1.0f);
}
