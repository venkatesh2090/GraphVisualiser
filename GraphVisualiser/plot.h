#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <glm/vec3.hpp>

extern double offsetX, offsetY;
extern double zoom;

struct Coordinates {
	Coordinates(float xCoord, float yCoord) : x(xCoord), y(yCoord) {}
	Coordinates() : x(0), y(0) {}
	float x, y;
};

class PlotColor {
public:
	PlotColor(int red, int green, int blue) : r((float) red / 255.0f), g((float) green / 255.0f), b((float) blue / 255.0f) {}
	PlotColor() : r(1.0f), g(1.0f), b(1.0f) {}

	void setR(int R) { r = (float) R / 255.0f; }
	void setG(int G) { g = (float) G / 255.0f; }
	void setB(int B) { b = (float) B / 255.0f; }

	float getR() { return r; }
	float getG() { return g; }
	float getB() { return b; }

	float getActualR() { return r * 255.0f; }
	float getActualG() { return g * 255.0f; }
	float getActualB() { return b * 255.0f; }

private:
	float r, g, b;
};

class Plot {
public:
	Plot(GLFWwindow* Window, const char* expressionString, PlotColor color = PlotColor()) : window(Window), expression_string(expressionString), plot_color(color) { prepare_buffer(); }
	Plot(GLFWwindow* Window, PlotColor color = PlotColor()) : expression_string(""), VAO(0), VBO(0), window(Window), plot_color(color) { prepare_buffer(); }
	~Plot();
	
	void set_expression_string(const char* expression_string) { this->expression_string = expression_string; };
	void set_plot_range(float leftX, float rightX);
	void set_plot_color(float r, float g, float b);
	void set_plot_color(PlotColor color) { plot_color = color; }

	unsigned int get_buffer_object() { return this->VBO; }
	unsigned int get_vertex_array_object() { return this->VAO; }
	std::vector<Coordinates> get_plot_coordinates() { return this->plot; }

	void draw_plot(Shader program);
	void draw_plot(Shader program, const char* expression_string, float leftX, float rightX);

private:
	unsigned int VAO, VBO;
	const char* expression_string;
	float leftX, rightX;
	std::vector<Coordinates> plot;
	GLFWwindow* window;
	PlotColor plot_color;

	void calculate_coordinates(float, float, float);
	void prepare_buffer();
};