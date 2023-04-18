/*
* LearnOpenGL Tutorial - Getting started > Hello Triangle > Exercise 3:
* Create two shader programs where the second program uses a different fragment shader that outputs the color yellow.
* Draw both triangles again where one outputs the color yellow.
* https://learnopengl.com/Getting-started/Hello-Triangle
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Vertex shader code.
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
// Array of each fragment shader's code.
const char *fragmentShaderSources[] = {
	// Orange fragment shader.
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0",
	// Yellow fragment shader.
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\n\0"
};

int main(int argc, char *argv[]) {
	// Initialize GLFW and configure OpenGL version and profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create GLFW window and set to current context.
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set GLFW window resize callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Initialize glad.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;

		return -1;
	}

	// Helpful constants.
	const unsigned int objectCount = 2, vertexCount = 3;

	// Shader variables.
	unsigned int vertexShader, fragmentShaders[objectCount], shaderPrograms[objectCount];
	// Error check variables.
	int success;
	char infoLog[512];

	// Create vertex shader.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for shader compile errors.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create fragment shaders and shader programs.
	for (int i = 0; i < objectCount; i++) {
		// Create fragment shader.
		fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
		glCompileShader(fragmentShaders[i]);
		// Check for shader compile errors.
		glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Create shader program and link shaders.
		shaderPrograms[i] = glCreateProgram();
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShaders[i]);
		glLinkProgram(shaderPrograms[i]);
		// Check for linking errors.
		glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(fragmentShaders[i]);
	}

	glDeleteShader(vertexShader);

	// Define vertex data.
	const float vertices[] = {
		// Triangle 1.
		-0.9f, -0.5f, 0.0f, // Left
		-0.45f, 0.5f, 0.0f, // Top
		 0.0f, -0.5f, 0.0f, // Right
		// Triangle 2.
		 0.0f, -0.5f, 0.0f, // Left
		 0.45f, 0.5f, 0.0f, // Top
		 0.9f, -0.5f, 0.0f  // Right
	};

	// Create buffer object arrays.
	unsigned int VBO[objectCount], VAO[objectCount];
	glGenBuffers(objectCount, VBO);
	glGenVertexArrays(objectCount, VAO);

	// Bind VAO and VBO for each object and copy respective vertex data.
	for (int i = 0; i < objectCount; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		// Calculate buffer size and array offset.
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(vertices) / objectCount,
			&vertices[i * 3 * vertexCount],
			GL_STATIC_DRAW
		);
		// Specify how OpenGL should interpret VBO data.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexCount * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
	}

	// Unbind VBO and VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Screen clear color.
	glClearColor(0.0f, 0.4f, 1.0f, 1.0f);

	// Main render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT); // Clear screen.

		// Render commands.
		for (int i = 0; i < objectCount; i++) {
			glUseProgram(shaderPrograms[i]);
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}

		glfwSwapBuffers(window);	// Swap window frame data buffers.
		glfwPollEvents();			// Poll for IO events.
	}

	// Optional: Manually free resources.
	for (int i = 0; i < objectCount; i++) {
		glDeleteBuffers(1, &VBO[i]);
		glDeleteVertexArrays(1, &VAO[i]);
		glDeleteProgram(shaderPrograms[i]);
	}
	glfwDestroyWindow(window);

	glfwTerminate(); // Terminate GLFW and automatically free resources.

	return 0;
}

// Input handling.
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Callback function for GLFW window resize.
// Ensures viewport matches window dimensions.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}