#version 330 core

out vec4 color;
in vec2 texture_coord;


uniform sampler2D grasstexture;


void main(){
color = texture(grasstexture,texture_coord);

}

