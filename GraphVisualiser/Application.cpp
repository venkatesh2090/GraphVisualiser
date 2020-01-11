#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "plot.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>



void key_callback(GLFWwindow* window, int kay, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void drawTicks(GLFWwindow* window, Shader program, int VAO);
void drawAxes(GLFWwindow* window, Shader program, int VAO, const float& leftX, const float& rightX);
void processInput(GLFWwindow* window);
void change_offsets(GLFWwindow* window, double xPos, double yPos);
void reset(GLFWwindow*);
void scroll_zoom(GLFWwindow* window, double x_offset, double y_offset);

double offsetX = 0.0f, offsetY = 0.0f;
float prevXPos = 0.0f, prevYPos = 0.0f;
bool firstClick = true;
unsigned int plotVBO, plotVAO;
double zoom = 1.0f;

const float verticesLine[] = {
	 0.0f, 0.0f,
	 0.0f, 1.0f
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "3D Letter E", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_zoom);

	unsigned int lineVBO, lineVAO;

	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glGenBuffers(1, &plotVBO);
	glBindBuffer(GL_ARRAY_BUFFER, plotVBO);

	glGenVertexArrays(1, &plotVAO);
	glBindVertexArray(plotVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	Plot plot1(window, PlotColor(110, 144, 22)), plot2(window, PlotColor(183, 66, 168)), plot3(window, PlotColor(231, 186, 24)), plot4(window, PlotColor(189, 208, 57)), plot5(window);

	Shader program("../Shaders/vertexShader.vert", "../Shaders/fragmentShader.frag");

	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(3.0f);



		int width, height;
		glfwGetWindowSize(window, &width, &height);

		try {
			float leftX = -10.0f;
			float rightX = 10.0f;
			drawTicks(window, program, lineVAO);
			drawAxes(window, program, lineVAO, leftX, rightX);
			plot1.draw_plot(program, "4*sin(x)/x", leftX, rightX);
			plot2.draw_plot(program, "floor(4*sin(x)/x)", leftX, rightX);
			plot3.draw_plot(program, "sqrt(1-x*x)", leftX, rightX);
			plot4.draw_plot(program, "log(x)", leftX, rightX);
		}
		catch (const char* msg) {
			std::cout << msg << std::endl;
			return -1;
		}
		catch (std::string msg) {
			std::cout << msg << std::endl;
			return -1;
		}
		 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		reset(window);
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetCursorPosCallback(window, change_offsets);
	}
	else {
		firstClick = true;
		glfwSetCursorPosCallback(window, NULL);
	}
	return;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void change_offsets(GLFWwindow* window, double xPos, double yPos) {
	int width, height;

	glfwGetWindowSize(window, &width, &height);

	double currentX = xPos - (width / 2);
	double currentY = (height / 2) - yPos;

	if (firstClick) {
		prevXPos = currentX;
		prevYPos = currentY;
		firstClick = false;
	}
	else {
		offsetX -= (prevXPos - currentX);
		offsetY -= (prevYPos - currentY);
		prevXPos = currentX;
		prevYPos = currentY;
	}
}

void scroll_zoom(GLFWwindow* window, double x_offset, double y_offset) {
	zoom += 0.01f * y_offset;
}

void drawTicks(GLFWwindow* window, Shader program, int VAO) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	for (float i = 1; i * 50 <= height; i += 1.0f) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::scale(view, glm::vec3(25.0F));
		view = glm::translate(view, glm::vec3(0.0f, 4 * i, 0.0f));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

		program.use();
		program.setMatrix4("projection", projection);
		program.setMatrix4("view", view);
		program.setMatrix4("model", model);
		program.setFloats("plotColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAO);

		glDrawArrays(GL_LINES, 0, sizeof(verticesLine));
	}

	for (float i = 1; i * 50 <= width; i += 1.0f) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, 0.0f, glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::scale(view, glm::vec3(25.0f));
		view = glm::translate(view, glm::vec3(4 * i, 0.0f, 0.0f));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

		program.use();
		program.setMatrix4("projection", projection);
		program.setMatrix4("view", view);
		program.setMatrix4("model", model);
		program.setFloats("plotColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAO);

		glDrawArrays(GL_LINES, 0, sizeof(verticesLine));
	}
}

void drawAxes(GLFWwindow* window, Shader program, int VAO, const float& leftX, const float& rightX) {
	if (rightX < leftX) {
		throw "Invalid values for leftX and rightX\nrightX should be greater than leftX at all times";
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	/*************************** Y Axis ***************************/
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(offsetX, 0.0f, 0.0f));
	view = glm::scale(view, glm::vec3((float)height, (float)height, 0.0f));
	float totalXUnits = rightX - leftX;
	if (leftX != rightX) {
		float multiplier = (float)width / ((float)height);
		float units = (leftX / totalXUnits);
		view = glm::translate(view, glm::vec3(-units * multiplier, 0.0f, 0.0f));
	}

	glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	program.use();
	program.setMatrix4("projection", projection);
	program.setMatrix4("view", view);
	program.setMatrix4("model", model);
	program.setFloats("plotColor", 1.0f, 1.0f, 1.0f);

	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, 2);



	/*************************** X Axis ***************************/
	model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, offsetY, 0.0f));
	view = glm::scale(view, glm::vec3((float)width, (float)width, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, (float)height / (2.0f * (float)width), 0.0f));

	program.use();
	program.setMatrix4("projection", projection);
	program.setMatrix4("view", view);
	program.setMatrix4("model", model);
	program.setFloats("plotColor", 1.0f, 1.0f, 1.0f);

	glBindVertexArray(VAO);

	glDrawArrays(GL_LINES, 0, 2);
}

void reset(GLFWwindow* window) {
	glfwSetWindowSize(window, 800, 800);
	offsetX = 0;
	offsetY = 0;
	zoom = 1.0f;
}