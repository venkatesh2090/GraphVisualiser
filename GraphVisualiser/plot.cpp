#include <glad/glad.h>
#include "plot.h"
#include <iostream>
#include <string>
#include <exprtk.hpp>
#include <glm/gtc/matrix_transform.hpp>

float totalXUnits;

void Plot::calculate_coordinates(float leftX, float rightX, float step) {
	plot.clear();
	float x;

	typedef exprtk::symbol_table<float> symbol_table;
	typedef exprtk::expression<float> expression;
	typedef exprtk::parser<float> parser;

	symbol_table expression_symbols;
	expression_symbols.add_variable("x", x);
	expression_symbols.add_constants();

	expression expression_evaluation;
	expression_evaluation.register_symbol_table(expression_symbols);

	parser expression_parser;
	if (!expression_parser.compile(expression_string, expression_evaluation)) {
		std::string msg = "Expression Compilation Error ";
		msg += expression_string;
		throw msg;
	}

	for (x = leftX; x < rightX; x += step) {
		plot.push_back(Coordinates(x, expression_evaluation.value()));
	}
}

Plot::~Plot() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Plot::set_plot_range(float leftX, float rightX) {
	this->leftX = leftX;
	this->rightX = rightX;

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	const float& scale = zoom;
	float currentOffset = (offsetX / (float)width * (1 / scale));

	float step = 10.0f / (float)width;
	totalXUnits = rightX - leftX;
	leftX = (leftX * (1 / scale)) - (totalXUnits * currentOffset);
	rightX = (rightX * (1 / scale)) - (totalXUnits * currentOffset);

	calculate_coordinates(leftX, rightX, step);
}

void Plot::prepare_buffer() {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Plot::draw_plot(Shader program) {

	if (rightX < leftX) {
		throw "Invalid values for leftX and rightX\nrightX should be greater than leftX at all times";
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height); 

	const float& scale = zoom;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, plot.size() * sizeof(Coordinates), plot.data(), GL_STREAM_DRAW);

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(offsetX, offsetY, 0.0f));
	view = glm::scale(view, glm::vec3((float)width/ 2.0f, (float)height / 2.0f, 0.0f));
	if (leftX != rightX) {
		float units = (leftX / totalXUnits);
		view = glm::translate(view, glm::vec3(-2.0f * units, 1.0f, 0.0f));
		view = glm::scale(view, glm::vec3(2.0f / totalXUnits));
	}
	view = glm::scale(view, glm::vec3(scale));

	glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	program.use();
	program.setMatrix4("view", view);
	program.setMatrix4("projection", projection);
	program.setMatrix4("model", model);
	program.setFloats("plotColor", plot_color.getR(), plot_color.getG(), plot_color.getB());

	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_STRIP, 0, plot.size());
}

void Plot::draw_plot(Shader program, const char* expression_string, float leftX, float rightX) {
	set_expression_string(expression_string);
	set_plot_range(leftX, rightX);
	draw_plot(program);
}

void Plot::set_plot_color(float r, float g, float b) {
	plot_color.setR(r);
	plot_color.setG(g);
	plot_color.setB(b);
}