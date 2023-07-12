﻿/*
* LearnOpenGL Tutorial - Getting Started > Camera > Exercise 1
* https://learnopengl.com/Getting-started/Camera
* See if you can transform the camera class in such a way that it becomes a true fps
* camera where you cannot fly; you can only look around while staying on the xz plane.
*/
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef NDEBUG
#include <debugout.hpp>
#endif

#include "shader/shader.hpp"
#include "camera/camera.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const unsigned int TEXTURE_COUNT = 2, CUBE_COUNT = 10;

const char *texturePaths[TEXTURE_COUNT] = {
	"resources/textures/container.jpg",
	"resources/textures/awesomeface.png"
};

// Cube vertex data. 6 faces * 2 triangles * 3 vertices = 36 vertices
const float vertexData[] = {
	 // Positions         // Texture Coords
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

// Cube positions in world space.
const glm::vec3 cubePositions[CUBE_COUNT] = {
	glm::vec3( 0.0f,  0.0f,  0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f,  2.0f, -2.5f),
	glm::vec3( 1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Timing.
float deltaTime = 0.0f; // Time between current frame and last frame.
float lastFrame = 0.0f;

// Input variables.
float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

// Camera.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor and capture it.

	// Initialize glad.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to initialize GLAD." << std::endl;
	#endif

		return -1;
	}

	// Configure OpenGL.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.6f, 1.0f);

	// Create and use shader program.
	Shader myShader("resources/shaders/myShader.vert", "resources/shaders/myShader.frag");
	myShader.useProgram();

	// Generate buffers and set vertex attributes.
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create textures.
	unsigned int textures[TEXTURE_COUNT];
	glGenTextures(TEXTURE_COUNT, textures);
	for (int i = 0; i < TEXTURE_COUNT; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, numChannels;
		stbi_set_flip_vertically_on_load(true);
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

	// Render loop.
	while (!glfwWindowShouldClose(window)) {
		// Calculate delta time.
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update projection matrix.
		glm::mat4 projection = glm::perspective(
			glm::radians(camera.fovY),
			static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
			0.1f, 100.0f
		);
		myShader.setMat4("projection", projection);
		// Update view matrix based on camera state.
		glm::mat4 view = camera.getViewMatrix();
		myShader.setMat4("view", view);

		// Render cubes.
		glBindVertexArray(VAO);
		for (int i = 0; i < CUBE_COUNT; i++) {
			// Translate and rotate each cube's model matrix.
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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

	// Accelerate camera when holding shift.
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.accelerate();
	// Camera movement (locked to xz plane).
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.translate(glm::vec3(camera.zAxis.x, 0.0f, camera.zAxis.z), deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.translate(-glm::vec3(camera.zAxis.x, 0.0f, camera.zAxis.z), deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.translate(-camera.xAxis, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.translate(camera.xAxis, deltaTime);
}

// Callback function for GLFW window resize.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Callback function for mouse movement.
void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	// Prevent sudden camera jump on first mouse movement.
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	// Rotate camera based on mouse movement.
	camera.rotate(xPos - lastX, lastY - yPos);

	// Update last mouse position.
	lastX = xPos;
	lastY = yPos;
}

// Callback function for mouse scroll.
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	camera.zoom(static_cast<float>(yOffset));
}