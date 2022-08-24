#version 330 core
out vec4 color;

uniform float uMoveX;

void main(){
    color = vec4(abs(1.0f + uMoveX), abs(1.0f - uMoveX), 1.0f, 1.0f);
}
