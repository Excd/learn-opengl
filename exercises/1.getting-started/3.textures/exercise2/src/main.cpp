/*
* LearnOpenGL Tutorial - Getting Started > Textures > Exercise 2
* https://learnopengl.com/Getting-started/Textures
* Experiment with the different texture wrapping methods by specifying texture
* coordinates in the range 0.0f to 2.0f instead of 0.0f to 1.0f. See if you can
* display 4 smiley faces on a single container image clamped at its edge.
*/
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#ifndef NDEBUG
#include <debugout.h>
#endif

#include "shader/shader.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const unsigned int TEXTURE_COUNT = 2;

const char *TEXTURE_PATHS[TEXTURE_COUNT] = {
	"resources/textures/container.jpg",
	"resources/textures/awesomeface.png"
};

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

	const float vertexData[] = {
		// Positions		 // Texture coordinates
		 0.5f,  0.5f, 0.0f,	 2.0f, 2.0f, // Top right
		 0.5f, -0.5f, 0.0f,  2.0f, 0.0f, // Bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom left
		-0.5f,  0.5f, 0.0f,	 0.0f, 2.0f	 // Top left
	};
	const unsigned int indices[] = {
		0, 1, 3, // First triangle
		1, 2, 3	 // Second triangle
	};

	// Generate buffers.
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind buffers and copy data.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Texture coordinate attribute.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create and use shader program.
	Shader myShader("resources/shaders/myShader.vert", "resources/shaders/myShader.frag");
	myShader.useProgram();

	// Create texture objects.
	int width, height, numChannels; // Texture data variables.
	unsigned int textures[TEXTURE_COUNT];
	glGenTextures(TEXTURE_COUNT, textures);

	// Activate first unit and bind texture (container.jpg).
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// Set texture wrapping and filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture, and generate mipmaps.
	stbi_set_flip_vertically_on_load(true); // Flip texture vertically.
	unsigned char *data = stbi_load(TEXTURE_PATHS[0], &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to load texture." << std::endl;
	#endif
	}
	stbi_image_free(data);
	myShader.setInt("textures[0]", 0);

	// Activate second unit and bind texture (awesomeface.png).
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// Set texture wrapping and filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture, and generate mipmaps.
	data = stbi_load(TEXTURE_PATHS[1], &width, &height, &numChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to load texture." << std::endl;
	#endif
	}
	stbi_image_free(data);
	myShader.setInt("textures[1]", 1);

	glClearColor(0.9f, 0.2f, 0.8f, 1.0f);

	// Render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(2, textures);
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