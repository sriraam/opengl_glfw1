#pragma once
#include<IL/il.h>
#include<IL/ilu.h>
#include<string.h>
#include<string>
#include<iostream>
#include<GLFW/glfw3.h>

class texture
{

	GLuint imageID;

	std::string filename;
public:
	GLint Width, Height;
	unsigned char* Data;
	texture();
	void loadtexture(std::string file);

	~texture();
};