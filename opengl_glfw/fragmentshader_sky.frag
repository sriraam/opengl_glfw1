#version 330 core
in vec3 texcoord;
out vec4 color;

uniform samplerCube skytexture;

void main(){
color = texture(skytexture,texcoord);
}

