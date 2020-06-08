#ifndef IMGUI_CONTROLLER_HPP
#define IMGUI_CONTROLLER_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include <vector>

template <typename T>
struct imgui_item
{
	std::string title;
	T *variable;
	T min;
	T max;
	bool read_only;
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

	void observef(const std::string &title, float *variable);
	void observei(const std::string &title, int *variable);
	void observef(const std::string &title, float *variable, float min, float max);
	void observei(const std::string &title, int *variable, int min, int max);
	void radio(const std::string &title, int *variable, int values);
	void button(const std::string &title, void (*fn)());

	template <typename T>
	void observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable);

	template <typename T>
	void observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable, T min, T max);

	template <typename T>
	void observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable, T min, T max, bool read_only);

	template <typename T>
	void display_number_item(struct imgui_item<T> &item);
};

template <typename T>
void ImGuiController::observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable)
{
	this->observe(target_list, title, variable, T(0), T(0), true);
}

template <typename T>
void ImGuiController::observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable, T min, T max)
{
	this->observe(target_list, title, variable, min, max, false);
}

template <typename T>
void ImGuiController::observe(std::vector<struct imgui_item<T>> &target_list, const std::string &title, T *variable, T min, T max, bool read_only)
{
	std::string observed_item_title(title);
	struct imgui_item<T> item =
	{
		observed_item_title,
			variable,
			min,
			max,
			read_only
	};

	target_list.push_back(item);
}

template <typename T>
void ImGuiController::display_number_item(struct imgui_item<T> &item)
{
	T value = *(item.variable);

	if (item.read_only)
	{
		ImGui::Text("%s: %s", item.title.c_str(), std::to_string(value).c_str());
	}
	else
	{

		if (value < item.min)
		{
			std::cerr << "[DEBUG] valor mínimo desrespeitado." << std::endl;
			std::cerr << "\tvalor mínimo: " << item.min << std::endl;
			std::cerr << "\t" << item.title.c_str() << ": " << value << std::endl;
		}

		if (value > item.max)
		{
			std::cerr << "[DEBUG] valor máximo desrespeitado." << std::endl;
			std::cerr << "\tvalor máximo: " << item.min << std::endl;
			std::cerr << "\t" << item.title.c_str() << ": " << value << std::endl;
		}

		assert(value >= item.min);
		assert(value <= item.max);
	}
}
#endif