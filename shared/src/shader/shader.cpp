#include <fstream>
#include <sstream>
#include <glad\glad.h>
#ifndef NDEBUG
#include <debugout.hpp>
#endif

#include "shader.hpp"

Shader::Shader() {
	program = glCreateProgram();
}

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) : Shader() {
	compileProgram(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::useProgram() {
	glUseProgram(program);
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

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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