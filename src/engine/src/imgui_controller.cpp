#ifndef IMGUI_CONTROLLER_CPP
#define IMGUI_CONTROLLER_CPP

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <cassert>
#include "imgui_controller.hpp"

ImGuiController::~ImGuiController()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiController::init(GLFWwindow *window)
{
	const char *glsl_version = "#version 130";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiController::display(GLFWwindow *window)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{

		ImGui::Begin("debug");

		for (auto &item : this->f_variables)
		{
			this->display_number_item(item);
			if (!item.read_only)
			{
				ImGui::SliderFloat(item.title.c_str(), item.variable, item.min, item.max);
			}
		}

		for (auto &item : this->i_variables)
		{
			this->display_number_item(item);
			if (!item.read_only)
			{
				ImGui::SliderInt(item.title.c_str(), item.variable, item.min, item.max);
			}
		}

		for (auto &item : this->radios)
		{
			ImGui::Text("%s", item.title.c_str());

			for (int i; i < item.values; i++)
			{
				ImGui::RadioButton(std::to_string(i).c_str(), item.variable, i);

				if (i < item.values - 1)
				{
					ImGui::SameLine();
				}
			}
		}

		for (auto &item : this->buttons)
		{
			if (ImGui::Button(item.title.c_str()))
			{
				(*item.fn)();
			}
		}

		ImGui::End();
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiController::radio(const std::string &title, int *variable, int values)
{
	std::string observed_item_title(title);
	struct imgui_radio item =
		{
			observed_item_title,
			variable,
			values};

	this->radios.push_back(item);
}

void ImGuiController::button(const std::string &title, void (*fn)())
{
	std::string observed_item_title(title);
	struct imgui_button item =
		{
			observed_item_title,
			fn};

	this->buttons.push_back(item);
}

void ImGuiController::observef(const std::string &title, float *variable)
{
	this->observe(this->f_variables, title, variable);
}

void ImGuiController::observei(const std::string &title, int *variable)
{
	this->observe(this->i_variables, title, variable);
}

void ImGuiController::observef(const std::string &title, float *variable, float min, float max)
{
	this->observe(this->f_variables, title, variable, min, max);
}

void ImGuiController::observei(const std::string &title, int *variable, int min, int max)
{
	this->observe(this->i_variables, title, variable, min, max);
}

#endif