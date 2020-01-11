#include <fstream>
#include <iostream>
#include <string>
#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

void compileWithErrors(GLuint shader);

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::ifstream vShaderFile(vertexShaderPath);
	std::ifstream fShaderFile(fragmentShaderPath);
	std::string vertexCode = "";
	std::string fragmentCode = "";

	if (!vShaderFile.is_open()) {
		std::cout << "Couldn't open " << vertexShaderPath << std::endl;
	}
	else {
		std::cout << "Opened " << vertexShaderPath << std::endl;
	}

	if (!fShaderFile.is_open()) {
		std::cout << "Couldn't open " << fragmentShaderPath << std::endl;
	}
	else {
		std::cout << "Opened " << fragmentShaderPath << std::endl;
	}

	while (vShaderFile) {
		char line[256];
		vShaderFile.getline(line, 256);
		vertexCode += line;
		vertexCode += '\n';
	}


	while (fShaderFile) {
		char line[256];
		fShaderFile.getline(line, 256);
		fragmentCode += line;
		fragmentCode += '\n';
	}

	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexCodeC = vertexCode.c_str();
	glShaderSource(vertexShader, 1, &vertexCodeC, NULL);
	const char* fragmentCodeC = fragmentCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentCodeC, NULL);

	compileWithErrors(vertexShader);
	compileWithErrors(fragmentShader);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setMatrix4(const char* name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setFloats(const char* name, float r, float g, float b)
{
	glUniform3f(glGetUniformLocation(ID, name), r, g, b);
}

void compileWithErrors(GLuint shader) {
	glCompileShader(shader);
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Compilation failed" << std::endl << infoLog << std::endl;
	}
}