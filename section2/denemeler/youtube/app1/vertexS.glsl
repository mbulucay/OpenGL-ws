#version 330 core
layout (location = 0) in vec3 inPos;

uniform vec3 uPos;
void main(){
    gl_Position = vec4(inPos + uPos, 1.0f);
}
