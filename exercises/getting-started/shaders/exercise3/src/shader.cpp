#include <glad\glad.h>
#include <fstream>
#include <sstream>

#include "shader.h"

Shader::Shader() {
	program = glCreateProgram();
}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) : Shader() {
	compileProgram(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::compileProgram(const char *vertexShaderPath, const char *fragmentShaderPath) {
	std::string vertexShaderSource = getShaderSource(vertexShaderPath);
	std::string fragmentShaderSource = getShaderSource(fragmentShaderPath);
	const char *vertexShaderCode = vertexShaderSource.c_str();
	const char *fragmentShaderCode = fragmentShaderSource.c_str();

	// Compile and link shaders.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, "VERTEX", vertexShaderCode);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, "FRAGMENT", fragmentShaderCode);
	glLinkProgram(program);
#ifndef NDEBUG
	checkCompileErrors(program, "PROGRAM");
#endif

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::useProgram() {
	glUseProgram(program);
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, int value) const {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::compileShader(unsigned int shader, std::string type, const char *shaderCode) {
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
#ifndef NDEBUG
	checkCompileErrors(shader, type);
#endif
	glAttachShader(program, shader);
}

std::string Shader::getShaderSource(const char *shaderPath) {
	std::ifstream shaderFile;
	std::string shaderSource;

	// Ensure ifstream can throw exceptions.
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		std::stringstream sourceStream;

		// Read file contents into stream, store as string.
		shaderFile.open(shaderPath);
		sourceStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderSource = sourceStream.str();
	}
	catch (std::ifstream::failure e) {
	#ifndef NDEBUG
		DEBUG_OUT << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
	#endif
	}

	return shaderSource;
}

#ifndef NDEBUG
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
	int success;
	char infoLog[512];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			DEBUG_OUT << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			DEBUG_OUT << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}
#endif