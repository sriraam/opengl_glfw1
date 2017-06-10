#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include<vector>
#include<map>
#include <iostream>

#include"texture.h"
#include"shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void render();
void init();

// settings
 unsigned int SCR_WIDTH = 800;
 unsigned int SCR_HEIGHT = 600;


GLuint VertexArrayID;

glm::mat4 View, Model, Projection;

GLint modelLoc, viewLoc, projLoc;

//glm::vec3 grass_pos;
std::vector<glm::vec3> grass_pos;
std::map<float, glm::vec3> sorted_glass;

GLuint lightcolor_loc, materialcolor_loc;
GLuint lightposLoc;
GLuint MatrixID;

GLuint VertexBuffer;
GLuint VertexBuffer2;

std::vector<std::string> faces;

GLuint VAO_2, vao_floor, vao_grass,vao_skybox;
GLuint VBO_2, vbo_floor, vbo_grass,vbo_skybox;

GLuint texture1;
GLuint texture_floor;
GLuint texture_grass;
GLuint texture_sky;
GLuint texture_sky1;

GLuint renderbuf;

texture Wood_texture, floor_texture,grass_texture,cube_texture;

shader shader_main;
shader shader_grass;
shader shader_floor;
shader shader_triangle;
shader shader_skybox;


//GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

// Camera Position

float camX, camY, camZ;
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse Tracking Variables
int startX, startY, tracking = 0;

int new_height,new_width;


// Camera Spherical Coordinates
float alpha = 40.0f, beta = 45.0f;
float r = 5.25f;

GLFWwindow* window;

int main()
{
	
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	// --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Blending", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	shader_main.loadshader("vertex_shader.vert", "fragment_shader.frag");
	shader_floor.loadshader("vertexshader_floor.vert", "fragmentshader_floor.frag");
	shader_grass.loadshader("vertexshader_grass.vert", "fragmentshader_grass.frag");
	shader_triangle.loadshader("vertex_tri.vert", "fragment_tri.frag");
	shader_skybox.loadshader("vertexshader_sky.vert", "fragmentshader_sky.frag");
	init();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----

		processInput(window);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

GLuint load_skybox(std::vector<std::string>) {

	glGenTextures(1, &texture_sky);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_sky);

	for (int i = 0; i < faces.size(); i++)  {
		cube_texture.loadtexture(faces[i]);
		if (cube_texture.Data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cube_texture.Width, cube_texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, cube_texture.Data);
		}
		else {
			std::cout << "cube map loading is failed";
		}
	}
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	

	return texture_sky;
}


void init() {
	// Generate 1 buffer, put the resulting identifier in vertexbuffer

	/*
	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);
	*/

	Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//for semitransparent material to compute the destination obj color with src alpha 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	


	//So we need three 3D points in order to make a triangle
	static const GLfloat g_Vertex_Buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	GLfloat cubeVertices[] = {
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	   	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLfloat floor_vertices[] = {
		7.0f,  -0.5f,  7.0f,  2.0f, 0.0f,
		-7.0f, -0.5f,  7.0f,  0.0f, 0.0f,
		-7.0f, -0.5f, -7.0f,  0.0f, 2.0f,

		7.0f,  -0.5f,  7.0f,  2.0f, 0.0f,
		-7.0f, -0.5f, -7.0f,  0.0f, 2.0f,
		7.0f,  -0.5f, -7.0f,  2.0f, 2.0f
	};

	GLfloat Quad[] = {

		// positions   // texCoords
		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,
		1.0f,  1.0f,0.0f,  1.0f, 1.0f

	};

  
	    grass_pos.push_back(glm::vec3(-2, 0, 0));
	    grass_pos.push_back(glm::vec3(-1, 0, 2)); 
		grass_pos.push_back(glm::vec3(3, 0, 0));
		grass_pos.push_back(glm::vec3(1.3, 0, -3));
		grass_pos.push_back(glm::vec3(-3, 0, -1));
		grass_pos.push_back(glm::vec3(2, 0, 3));
		grass_pos.push_back(glm::vec3(2, 0, 3));
		grass_pos.push_back(glm::vec3(0, 0, 4));
		grass_pos.push_back(glm::vec3(1.5, 0, 3));
		//grass_pos.push_back(glm::vec3(0, 0, 2));
		

		//arranging in the order from large distance to small
		for (int i = 0; i < grass_pos.size(); i++) {

			float distance = glm::length(cameraPos - grass_pos[i]);
			sorted_glass[distance] = grass_pos[i];

		}


	//grass texture
	grass_texture.loadtexture("red_glass.png");
	
	glGenTextures(1, &texture_grass);
	glBindTexture(GL_TEXTURE_2D, texture_grass);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grass_texture.Width, grass_texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, grass_texture.Data);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);


	//wooden texture
	Wood_texture.loadtexture("wooden_texture.jpg");

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Wood_texture.Width, Wood_texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Wood_texture.Data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//floor texture
	floor_texture.loadtexture("floor.jpg");
	glGenTextures(1, &texture_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floor_texture.Width, floor_texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, floor_texture.Data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);



	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &VertexBuffer);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertex_Buffer_data), g_Vertex_Buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);



	//Cube VAO
	glGenVertexArrays(1, &VAO_2);
	glGenBuffers(1, &VBO_2);
	glBindVertexArray(VAO_2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//floor VAO
	glGenVertexArrays(1, &vao_floor);
	glGenBuffers(1, &vbo_floor);
	glBindVertexArray(vao_floor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	//Quad VAO
	glGenVertexArrays(1, &vao_grass);
	glGenBuffers(1, &vbo_grass);
	glBindVertexArray(vao_grass);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_grass);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	//sky_box VAO
	glGenVertexArrays(1, &vao_skybox);
	glGenBuffers(1, &vbo_skybox);
	glBindVertexArray(vao_skybox);
	glBindBuffer(GL_ARRAY_BUFFER,vbo_skybox);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	 faces={
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};

	texture_sky1 = load_skybox(faces);


	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

}

void render() 

	{

		View = glm::lookAt(cameraPos,glm::vec3(cameraPos.x,1.5,cameraPos.z) +  cameraFront,cameraUp);
		Model = glm::mat4();
		
		// draw as wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		


		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  

		/*shader_triangle.Use();

		//simple triangle
		glBindVertexArray(VertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		*/


		shader_skybox.Use();
		glDepthMask(GL_FALSE);
		glm::mat4 View_33 = glm::mat4(glm::mat3(View));
		glUniformMatrix4fv(glGetUniformLocation(shader_skybox.program, "view"), 1, GL_FALSE, glm::value_ptr(View_33));
		glUniformMatrix4fv(glGetUniformLocation(shader_skybox.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_sky1);
		glBindVertexArray(vao_skybox);
		
		//glActiveTexture(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
		glBindVertexArray(0);

		shader_main.Use();

		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));


		//cube_vao:
		glBindVertexArray(VAO_2);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(0);
		glUniform1i(glGetUniformLocation(shader_main.program, "texture_1"), 0);
		Model = glm::translate(Model, glm::vec3(1.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
		glDrawArrays(GL_TRIANGLES, 0, 36); // Starting from vertex 0; 3 vertices total -> 1 triangle
		Model = glm::translate(Model, glm::vec3(-2.0, 0.0f, 2.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		shader_floor.Use();

		glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));

		//floor_vao:
		glBindVertexArray(vao_floor);
		glBindTexture(GL_TEXTURE_2D, texture_floor);
		glUniform1i(glGetUniformLocation(shader_floor.program, "floor_texture"), 0);
		//Model = glm::translate(Model, glm::vec3(-1.0f, -1.0f, -1.0f));
		Model = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(shader_floor.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
		glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glBindVertexArray(0);

		shader_grass.Use();
		
		glUniformMatrix4fv(glGetUniformLocation(shader_grass.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(shader_grass.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));


		//grass vao
		glBindVertexArray(vao_grass);
		glBindTexture(GL_TEXTURE_2D, texture_grass);
		glUniform1i(glGetUniformLocation(shader_grass.program, "grasstexture"),0);
		
		//std::map<float, glm::vec3>::reverse_iterator it = sorted_glass.rbegin();
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted_glass.rbegin() ; it != sorted_glass.rend() ; ++it) {
			//std::cout << sorted_glass.count() << std::endl;
			Model = glm::mat4();
			//this "it" is an reverse iterator assigned to sorted_glass map
			//it point various index of sorted_glass map acc to loop
			//it can choose first or second val of the map
			Model = glm::translate(Model,it->second);
			
			glUniformMatrix4fv(glGetUniformLocation(shader_grass.program, "model"), 1, GL_FALSE, glm::value_ptr(Model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);

		



}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	float speed = .05f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += speed * (cameraFront);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= speed * (cameraFront);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	
	glViewport(0, 0, width, height);
	Projection = glm::perspective(glm::radians(45.0f), (float)width/ (float)height, 0.1f, 100.0f);

}
