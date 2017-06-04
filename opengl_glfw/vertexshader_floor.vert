#version 330 core
layout(location = 1) in vec3 floor_vertex;
layout(location = 2) in vec2 tex_coords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 texture_coords;

void main(){
gl_Position = projection * view * model * vec4(floor_vertex,1);
//gl_Position =  vec4(floor_vertex,1);
texture_coords = tex_coords;

}
