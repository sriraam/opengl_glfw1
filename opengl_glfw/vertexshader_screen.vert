#version 330 core
layout(location = 1)in vec3 vertex_position;
layout(location = 2)in vec2 Tex_coord;


out vec2 texture_coord;

void main(){

gl_Position = vec4(vertex_position,1);
texture_coord = Tex_coord;
}
