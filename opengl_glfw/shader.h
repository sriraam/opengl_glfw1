#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class shader
{
public:
	GLuint program;
	//shader();
	void loadshader(const std::string&, const std::string&);
	void Use();

	~shader() {
	}
};

void shader::loadshader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;


	try {
		// Load the shader.
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
	}



	catch (std::ifstream::failure e) {
		// ensures ifstream objects can throw exceptions:
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	std::string vsource(std::istreambuf_iterator<char>(vShaderFile), (std::istreambuf_iterator<char>()));
	vShaderFile.close();
	std::string fsource(std::istreambuf_iterator<char>(fShaderFile), (std::istreambuf_iterator<char>()));
	fShaderFile.close();

	// Create a shader object.
	GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
	// Load the shader source for each shader object.
	const GLchar* vsources[] = { vsource.c_str() };
	glShaderSource(v_shader, 1, vsources, NULL);

	// Compile the shader.
	glCompileShader(v_shader);

	// Check for errors
	GLint compileStatus;
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		std::cout << "entered  compiler";
		GLint logLength = 0;
		//	glGetShaderiv(v_shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(v_shader, logLength, NULL, infoLog);

		//#ifdef _WIN32
		//	OutputDebugString(infoLog);
		//#else
		std::cerr << infoLog << std::endl;
		//#endif
		//delete infoLog;
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		//return 0;
	}

	// Create a shader object.
	GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
	// Load the shader source for each shader object.
	const GLchar* fsources[] = { fsource.c_str() };
	glShaderSource(f_shader, 1, fsources, NULL);

	// Compile the shader.
	glCompileShader(f_shader);
	// Check for errors
	//GLint compileStatus;
	glGetShaderiv(v_shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)
	{
		GLint logLength = 0;
		//	glGetShaderiv(v_shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(f_shader, logLength, NULL, infoLog);

		//#ifdef _WIN32
		//	OutputDebugString(infoLog);
		//#else
		std::cerr << infoLog << std::endl;
		//#endif
		//delete infoLog;
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		//return 0;
	}


	this->program = glCreateProgram();
	glAttachShader(this->program, v_shader);
	glAttachShader(this->program, f_shader);
	glLinkProgram(this->program);

	// Check the link status.
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(program, logLength, NULL, infoLog);

		//#ifdef _WIN32
		//	OutputDebugString(infoLog);
		//#else
		// std::cerr << infoLog << std::endl;
		//#endif
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		delete infoLog;
		// return 0;
	}



}
// Uses the current shader

void shader::Use() {

	glUseProgram(this->program);
}