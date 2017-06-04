#version 330 core

in vec2 texture_coords;
out vec4 color;

uniform sampler2D floor_texture;

void main(){
color = texture(floor_texture,texture_coords);
//color = vec4(1.0f,0.0f,0.0f,1.0f);
}