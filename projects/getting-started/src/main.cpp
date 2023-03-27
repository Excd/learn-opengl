#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
	glfwInit();	// Initialize GLFW.

	// Configure GLFW version and profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 0;
}