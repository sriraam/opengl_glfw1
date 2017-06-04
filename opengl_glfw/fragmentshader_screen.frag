#version 330 core

out vec4 color;
in vec2 texture_coord;

uniform sampler2D screentexture;

void main(){
//color = texture(screentexture,texture_coord);
//gl_FragColor = vec4(color,1);
//color = vec4(col,0);
//color = vec4(1,0,0,1);
color = vec4(vec3(1.0 - texture(screentexture, texture_coord)), 1.0);
}
