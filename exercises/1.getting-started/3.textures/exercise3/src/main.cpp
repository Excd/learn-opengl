/*
* LearnOpenGL Tutorial - Getting Started > Textures > Exercise 3
* https://learnopengl.com/Getting-started/Textures
* Try to display only the center pixels of the texture image on the rectangle
* in such a way that the individual pixels are getting visible by changing the
* texture coordinates. Try to set the texture filtering method to GL_NEAREST to
* see the pixels more clearly.
*/
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#ifndef NDEBUG
#include <debugout.hpp>
#endif

#include "shader/shader.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const unsigned int TEXTURE_COUNT = 2;

const char *texturePaths[TEXTURE_COUNT] = {
	"resources/textures/container.jpg",
	"resources/textures/awesomeface.png"
};

const float vertexData[] = {
	 // Positions		 // Texture coordinates
	 0.5f,  0.5f, 0.0f,	 0.55f, 0.55f, // Top right
	 0.5f, -0.5f, 0.0f,  0.55f, 0.45f, // Bottom right
	-0.5f, -0.5f, 0.0f,  0.45f, 0.45f, // Bottom left
	-0.5f,  0.5f, 0.0f,	 0.45f, 0.55f  // Top left
};
const unsigned int indices[] = {
	0, 1, 3, // First triangle
	1, 2, 3	 // Second triangle
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

	// Create and use shader program.
	Shader myShader("resources/shaders/myShader.vert", "resources/shaders/myShader.frag");
	myShader.useProgram();

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
	// Texture coordinate attribute.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create texture objects.
	unsigned int textures[TEXTURE_COUNT];
	glGenTextures(TEXTURE_COUNT, textures);
	for (int i = 0; i < TEXTURE_COUNT; i++) {
		// Activate texture unit and bind texture.
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		// Set texture wrapping and filtering options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Load image, create texture, and generate mipmaps.
		int width, height, numChannels;
		stbi_set_flip_vertically_on_load(true); // Flip texture vertically.
		unsigned char *data = stbi_load(texturePaths[i], &width, &height, &numChannels, 0);
		if (data) {
			switch (numChannels) {
				case 3:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				case 4:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					break;
				default:
				#ifndef NDEBUG
					DEBUG_OUT << "Unsupported number of texture channels. Index: " << i << std::endl;
				#endif
					break;
			}
			glGenerateMipmap(GL_TEXTURE_2D);
			// Assign texture unit to sampler uniform.
			char uniformName[16];
			std::snprintf(uniformName, sizeof(uniformName), "textures[%d]", i);
			myShader.setInt(uniformName, i);
		}
		else {
		#ifndef NDEBUG
			DEBUG_OUT << "Failed to load texture." << std::endl;
		#endif
		}
		stbi_image_free(data);
	}

	glClearColor(0.0f, 0.6f, 0.6f, 1.0f);

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