/*
* LearnOpenGL Tutorial - Getting Started > Hello Triangle > Exercise 2
* https://learnopengl.com/Getting-started/Hello-Triangle
* Now create the same 2 triangles using two different VAOs and VBOs for their data.
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef NDEBUG
#include <debugout.hpp>
#endif

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const unsigned int OBJECT_COUNT = 2, VERTEX_COUNT = 3;

const char *vertexShaderCode = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentShaderCode = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main(int argc, char *argv[]) {
	// Initialize GLFW and configure OpenGL version and profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow OpenGL forward compatibility on macOS.
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create GLFW window and set to current context.
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to create GLFW window." << std::endl;
	#endif
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set GLFW window resize callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	#ifndef NDEBUG
		DEBUG_OUT << "Failed to initialize GLAD." << std::endl;
	#endif

		return -1;
	}

	// Debug variables.
#ifndef NDEBUG
	int success;
	char infoLog[512];
#endif

	// Create vertex shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	// Check for shader compile errors.
#ifndef NDEBUG
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		DEBUG_OUT << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#endif

	// Create fragment shader.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	// Check for shader compile errors.
#ifndef NDEBUG
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		DEBUG_OUT << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
#endif

	// Create shader program.
	unsigned int shaderProgram = glCreateProgram();
	// Attach shaders to program and link.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors.
#ifndef NDEBUG
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		DEBUG_OUT << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
#endif
	// Cleanup shaders.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
	unsigned int VBO[OBJECT_COUNT], VAO[OBJECT_COUNT];
	glGenBuffers(OBJECT_COUNT, VBO);
	glGenVertexArrays(OBJECT_COUNT, VAO);

	// Bind VAO and VBO for each object and copy respective vertex data.
	for (int i = 0; i < OBJECT_COUNT; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		// Calculate buffer size and array offset.
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(vertices) / OBJECT_COUNT,
			&vertices[i * 3 * VERTEX_COUNT],
			GL_STATIC_DRAW
		);
		// Specify how OpenGL should interpret VBO data.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_COUNT * sizeof(float), reinterpret_cast<void *>(0));
		glEnableVertexAttribArray(0);
	}

	// Unbind VBO and VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.1f, 0.2f, 0.6f, 1.0f); // Set screen clear color.

	glUseProgram(shaderProgram);

	// Main render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT); // Clear screen.

		// Render commands.
		for (int i = 0; i < OBJECT_COUNT; i++) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
		}

		glfwSwapBuffers(window);	// Swap window frame data buffers.
		glfwPollEvents();			// Poll for IO events.
	}

	// Optional: Manually free resources.
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(2, VAO);
	glDeleteProgram(shaderProgram);
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