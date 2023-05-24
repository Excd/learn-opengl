/*
* LearnOpenGL Tutorial - Getting Started > Shaders > Exercise 1
* https://learnopengl.com/Getting-started/Shaders
* Adjust the vertex shader so that the triangle is upside down.
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef NDEBUG
#include <debugout.h>
#endif

#include "shader.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main(int argc, char *argv[]) {
	// Initialize GLFW.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create GLFW window.
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to create GLFW window." << std::endl;
	#endif
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to initialize GLAD." << std::endl;
	#endif

		return -1;
	}

#ifndef NDEBUG
	// Print number of vertex attributes supported.
	int numAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
	DEBUG_OUT << "Maximum number of vertex attributes supported: " << numAttributes << std::endl;
#endif

	const float vertexData[] = {
		// Positions		 // Colors
		 0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,	// Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	// Left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f	// Top
	};

	// Generate buffers.
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// Bind buffers and copy data.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Color attribute.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Create and use shader program.
	Shader myShader("shaders/myShader.vert", "shaders/myShader.frag");
	myShader.useProgram();

	// Render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// Input handling.
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Callback function for GLFW window resize.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}