#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	Shader();
	Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
	~Shader();

	void useProgram();
	void compileProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
	// Uniform setters.
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	unsigned int program;

	std::string getShaderSource(const char *shaderPath);
	void compileShader(unsigned int shader, std::string type, const char *shaderCode);
#ifndef NDEBUG
	void checkCompileErrors(unsigned int shader, std::string type);
#endif
};
#endif