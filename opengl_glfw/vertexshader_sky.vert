#version 330 core
layout(location =1 )in vec3 skybox_vertex;
out vec3 texcoord;

uniform mat4 view;
uniform mat4 projection;

void main(){
texcoord = skybox_vertex;
gl_Position = projection * view  * vec4(skybox_vertex,1);

}