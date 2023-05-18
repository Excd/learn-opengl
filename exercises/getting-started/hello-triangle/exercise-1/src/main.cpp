/*
* LearnOpenGL Tutorial - Getting Started > Hello Triangle > Exercise 1
* Try to draw 2 triangles next to each other using glDrawArrays by adding more vertices to your data.
* https://learnopengl.com/Getting-started/Hello-Triangle
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
#ifndef NDEBUG
void debugOut(std::wostringstream &debugString);
#endif

const unsigned int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

// Vertex shader code.
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
// Fragment shader code.
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main(int argc, char *argv[]) {
	// Debug variables.
#ifndef NDEBUG
	std::wostringstream debugString;
	int success;
	char infoLog[512];
#endif
	
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
		debugString << "Failed to create GLFW window." << std::endl;
		debugOut(debugString);
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
		debugString << "Failed to initialize GLAD." << std::endl;
		debugOut(debugString);
	#endif

		return -1;
	}

	// Create vertex shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for shader compile errors.
#ifndef NDEBUG
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
	}
#endif

	// Create fragment shader.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for shader compile errors.
#ifndef NDEBUG
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
	}
#endif

	// Create shader program.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Attach shaders to program and link.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors.
#ifndef NDEBUG
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
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

	// Create buffer objects.
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);		// Generate 1 buffer and store ID in VBO.
	glGenVertexArrays(1, &VAO);	// Generate 1 VAO and store ID in VAO.

	glBindVertexArray(VAO);		// Bind VAO.

	// Bind VBO to GL_ARRAY_BUFFER target and copy vertex data to buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify how OpenGL should interpret VBO data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO and VAO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.1f, 0.4f, 1.0f); // Set screen clear color.

	// Main render loop.
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT); // Clear screen.

		// Render commands.
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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

// Debug output.
#ifndef NDEBUG
void debugOut(std::wostringstream &debugString) {
	OutputDebugStringW(debugString.str().c_str());
	debugString.str(std::wstring());
	debugString.clear();
}
#endif