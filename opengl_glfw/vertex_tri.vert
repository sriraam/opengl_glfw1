#version 330 core
layout(location = 1)in vec3 vertex_position;

void main(){

gl_Position = vec4(vertex_position,1);

}