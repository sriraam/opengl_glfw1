#version 330 core
out vec4 color;

in vec2 texture_coords;

uniform sampler2D texture_1;

void main(){
color = texture(texture_1,texture_coords);

//color= vec4(1.0,0.0,1.0,1.0);
}