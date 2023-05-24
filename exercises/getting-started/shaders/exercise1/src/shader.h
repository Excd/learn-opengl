#ifndef SHADER_H
#define SHADER_H

#include <string>
#ifndef NDEBUG
#include <debugout.h>
#endif

class Shader {
public:
	Shader();
	Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
	~Shader();

	void compileProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
	void useProgram();
	// Uniform setters.
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, int value) const;

private:
	unsigned int program;

	std::string getShaderSource(const char *shaderPath);
	void compileShader(unsigned int shader, std::string type, const char *shaderCode);
#ifndef NDEBUG
	void checkCompileErrors(unsigned int shader, std::string type);
#endif
};
#endif