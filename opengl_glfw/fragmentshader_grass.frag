#version 330 core

out vec4 color;
in vec2 texture_coord;


uniform sampler2D grasstexture;


void main(){
vec4 temp_texture = texture(grasstexture,texture_coord);
if(temp_texture.a < 0.1){
  discard;
}
color = temp_texture;


}

