#pragma once

#include <glm/mat4x4.hpp>

class Shader {
public:
	int ID;

	Shader(const char* vertexShader, const char* fragmentShader);

	void use();

	void setFloat(const char* name, float value);
	void setInt(const char* name, int value);
	void setMatrix4(const char* name, glm::mat4 value);
	void setFloats(const char* name, float r, float g, float b);
};