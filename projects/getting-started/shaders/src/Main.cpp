/*
* LearnOpenGL Tutorial - Getting Started > Shaders
* https://learnopengl.com/Getting-started/Shaders
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

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"out vec3 ourColor;\n"
	"void main() {\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"   ourColor = aColor;\n"
	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main() {\n"
	"   FragColor = vec4(ourColor, 1.0);\n"
	"}\0";

int main(int argc, char *argv[]) {
	// Debug variables.
#ifndef NDEBUG
	std::wostringstream debugString;
	int success;
	char infoLog[512];
#endif

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
		debugString << "Failed to create GLFW window." << std::endl;
		debugOut(debugString);
	#endif
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize glad.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	#ifndef NDEBUG
		debugString << "Failed to initialize GLAD." << std::endl;
		debugOut(debugString);
	#endif

		return -1;
	}

	// Print number of vertex attributes supported.
#ifndef NDEBUG
	int numAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
	debugString << "Maximum number of vertex attributes supported: " << numAttributes << std::endl;
	debugOut(debugString);
#endif

	// Vertex shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
#ifndef NDEBUG
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
	}
#endif

	// Fragment shader.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
#ifndef NDEBUG
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
	}
#endif

	// Shader program.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
#ifndef NDEBUG
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		debugString << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		debugOut(debugString);
	}
#endif

	// Shader cleanup.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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

	glUseProgram(shaderProgram);

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
	glDeleteProgram(shaderProgram);
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

// Debug output.
#ifndef NDEBUG
void debugOut(std::wostringstream &debugString) {
	OutputDebugStringW(debugString.str().c_str());
	debugString.str(std::wstring());
	debugString.clear();
}
#endif