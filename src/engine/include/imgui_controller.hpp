#ifndef IMGUI_CONTROLLER_HPP
#define IMGUI_CONTROLLER_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

template <typename T>
struct imgui_item
{
	std::string title;
	T *variable;
	T min;
	T max;
};

struct imgui_radio
{
	std::string title;
	int *variable;
	int values;
};

struct imgui_button
{
	std::string title;
	void (*fn)();
};

class ImGuiController
{

private:
	std::vector<struct imgui_item<float>> f_variables;
	std::vector<struct imgui_item<int>> i_variables;
	std::vector<struct imgui_radio> radios;
	std::vector<struct imgui_button> buttons;

public:
	ImGuiController(){};
	~ImGuiController();

	void init(GLFWwindow *window);
	void display(GLFWwindow *window);

	void observef(const std::string &title, float *variable, float min, float max);
	void observei(const std::string &title, int *variable, int min, int max);
	void radio(const std::string &title, int *variable, int values);
	void button(const std::string &title, void (*fn)());
};

#endif