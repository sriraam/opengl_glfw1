#version 330 core
layout(location = 1)in vec3 vertex_position;
layout(location = 2)in vec2 Tex_coord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 texture_coord;

void main(){

gl_Position = projection * view * model * vec4(vertex_position,1);
texture_coord = Tex_coord;
}
