#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include <iostream>

#include"texture.h"
#include"shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void render();
void init();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


GLuint VertexArrayID;

glm::mat4 View, Model, Projection;

GLint modelLoc, viewLoc, projLoc;

GLuint lightcolor_loc, materialcolor_loc;
GLuint lightposLoc;
GLuint MatrixID;

GLuint VertexBuffer;
GLuint VertexBuffer2;

GLuint framebuffer;

GLuint VAO_2, vao_floor, vao_screen;
GLuint VBO_2, vbo_floor, vbo_screen;

GLuint texture1;
GLuint texture_floor;
GLuint texture_framebuf;

GLuint renderbuf;


texture Wood_texture, floor_texture;

shader shader_main;
shader shader_floor;
shader shader_screen;

//GLuint g_ShaderProgram = 0;
//glGenVertexArrays(1, &VertexArrayID);

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//shader shader_main;
	shader_main.loadshader("vertex_shader.vert", "fragment_shader.frag");
	shader_floor.loadshader("vertexshader_floor.vert", "fragmentshader_floor.frag");
	shader_screen.loadshader("vertexshader_screen.vert", "fragmentshader_screen.frag"); 
	init();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----

		processInput(window);
		render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		//glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteFramebuffers(1, &framebuffer);
	glfwTerminate();
	return 0;
}


void render() 

	{
		// draw as wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Camera matrix
		View = glm::lookAt(
			glm::vec3(camX, camY, camZ), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		Model = glm::mat4();

		glClearColor(0.0f, 1.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		
		glEnable(GL_DEPTH_TEST);

		shader_main.Use();

		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "view"), 1, GL_FALSE, glm::value_ptr(View));
		glUniformMatrix4fv(glGetUniformLocation(shader_main.program, "projection"), 1, GL_FALSE, glm::value_ptr(Projection));


		//simple triangle


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


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glDisable(GL_DEPTH);



		shader_screen.Use();

		glUniform1i(glGetUniformLocation(shader_screen.program, "screentexture"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_framebuf);
		glBindVertexArray(vao_screen);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
		//glClear(GL_COLOR_BUFFER_BIT);
		//glfwSwapBuffers(window);
		


}

void init() {
	// Generate 1 buffer, put the resulting identifier in vertexbuffer


	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Projection = glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 100.0f);
	// some GL settings

	glEnable(GL_CULL_FACE);
	//glEnable(GL_MULTISAMPLE);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);



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

	GLfloat floor_vertices[] = {
		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
	};

	GLfloat Quad[] = {
		/*
		-1, 1, 0, 0,1,
		1, 1, 0, 1,1,
		1,-1, 0, 1,0,
		1,-1, 0, 1,0,
		-1,-1, 0, 0,0,
		-1, 1, 0, 0,1*/

		// positions   // texCoords
		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
		1.0f, -1.0f,0.0f,  1.0f, 0.0f,
		1.0f,  1.0f,0.0f,  1.0f, 1.0f

	};

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



	/*
	glGenVertexArrays(1, &VertexArrayID);
	glGenBuffers(1, &VertexBuffer);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_Vertex_Buffer_data), g_Vertex_Buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	*/


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
	glGenVertexArrays(1, &vao_screen);
	glGenBuffers(1, &vbo_screen);
	glBindVertexArray(vao_screen);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_screen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Quad), Quad, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	//Frame Buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//texture attachment(it samples)
	glGenTextures(1, &texture_framebuf);
	glBindTexture(GL_TEXTURE_2D, texture_framebuf);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_framebuf, 0);

	//

	//render attachment(has depth values)

	glGenRenderbuffers(1, &renderbuf);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuf);



	//checking whether the frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glDeleteFramebuffers(1, &framebuffer);


	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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