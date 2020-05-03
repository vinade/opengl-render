#ifndef IMGUI_CONTROLLER_HPP
#define IMGUI_CONTROLLER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./3rd/imgui/imgui.h"
#include "./3rd/imgui/imgui_impl_glfw.h"
#include "./3rd/imgui/imgui_impl_opengl3.h"

#include <vector>
#include <cassert>

template <typename T>
struct imgui_item
{
	std::string title;
	T *variable;
	T min;
	T max;
};

class ImGuiController
{

private:
	std::vector<struct imgui_item<float>> f_variables;
	std::vector<struct imgui_item<int>> i_variables;

public:
	ImGuiController(){};
	~ImGuiController();

	void init(GLFWwindow *window);
	void display(GLFWwindow *window);

	void observef(const std::string &title, float *variable, float min, float max);
	void observei(const std::string &title, int *variable, int min, int max);
};

#endif