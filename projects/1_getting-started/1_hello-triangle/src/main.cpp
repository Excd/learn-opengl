/*
* LearnOpenGL Tutorial - Getting Started > Hello Triangle
* https://learnopengl.com/Getting-started/Hello-Triangle
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#ifndef NDEBUG
#include <debugout.hpp>
#endif

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

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

// Define vertex data.
const float vertices[] = {
	 0.5f,  0.5f, 0.0f, // Top right.
	 0.5f, -0.5f, 0.0f, // Bottom right.
	-0.5f, -0.5f, 0.0f, // Bottom left.
	-0.5f,  0.5f, 0.0f  // Top left.
};
// Define indices.
const unsigned int indices[] = {
	0, 1, 3, // First triangle.
	1, 2, 3  // Second triangle.
};

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

	glUseProgram(shaderProgram); // Use shader program.

	// Create buffer objects.
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);		// Generate 1 buffer and store ID in VBO.
	glGenVertexArrays(1, &VAO);	// Generate 1 VAO and store ID in VAO.
	glGenBuffers(1, &EBO);		// Generate 1 buffer and store ID in EBO.

	glBindVertexArray(VAO);		// Bind VAO.

	// Bind VBO to GL_ARRAY_BUFFER target and copy vertex data to buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind EBO to GL_ELEMENT_ARRAY_BUFFER target and copy indices to buffer.
	// EBO is bound within currently bound VAO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specify how OpenGL should interpret VBO data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);

	// Do not unbind the EBO while a VAO is active as the bound EBO is stored within the VAO.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Unbind VBO, and VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw in wireframe mode.
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);			// Set screen clear color.

	// Main render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT); // Clear screen.

		// Render commands.
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);	// Swap window frame data buffers.
		glfwPollEvents();			// Poll for IO events.
	}

	// Optional: Manually free resources.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
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