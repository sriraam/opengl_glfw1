#version 330 core
layout(location = 1) in vec3 vertex_position;
layout(location = 2) in vec2 Tex_coords;

out vec2 texture_coords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(){
gl_Position = projection * view * model * vec4(vertex_position,1);
texture_coords= Tex_coords;

}

