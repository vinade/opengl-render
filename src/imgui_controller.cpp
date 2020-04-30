#ifndef IMGUI_CONTROLLER_CPP
#define IMGUI_CONTROLLER_CPP


#include "imgui_controller.hpp"


ImGuiController::~ImGuiController(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void ImGuiController::init(GLFWwindow* window){
	const char* glsl_version = "#version 130";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}


void ImGuiController::display(GLFWwindow* window){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {

    	ImGui::Begin("debug");

    	for (auto& item : this->f_variables){

    		float value = *(item.variable);

    		if (value < item.min){
    			std::cerr << "[DEBUG] valor mínimo desrespeitado." << std::endl;
    			std::cerr << "\tvalor mínimo: " << item.min << std::endl;
    			std::cerr << "\t" << item.title.c_str() << ": " << value  << std::endl;
    		}

    		if (value > item.max){
    			std::cerr << "[DEBUG] valor máximo desrespeitado." << std::endl;
    			std::cerr << "\tvalor máximo: " << item.min << std::endl;
    			std::cerr << "\t" << item.title.c_str() << ": " << value  << std::endl;
    		}

    		assert(value >= item.min);
    		assert(value <= item.max);

    		ImGui::SliderFloat(item.title.c_str(), item.variable , item.min, item.max);
    	}

    	for (auto& item : this->i_variables){

    		int value = *(item.variable);

    		if (value < item.min){
    			std::cerr << "[DEBUG] valor mínimo desrespeitado." << std::endl;
    			std::cerr << "\tvalor mínimo: " << item.min << std::endl;
    			std::cerr << "\t" << item.title.c_str() << ": " << value  << std::endl;
    		}

    		if (value > item.max){
    			std::cerr << "[DEBUG] valor máximo desrespeitado." << std::endl;
    			std::cerr << "\tvalor máximo: " << item.min << std::endl;
    			std::cerr << "\t" << item.title.c_str() << ": " << value  << std::endl;
    		}

    		assert(value >= item.min);
    		assert(value <= item.max);

    		ImGui::SliderInt(item.title.c_str(), item.variable, item.min, item.max);
    	}

    	ImGui::End();

    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiController::observef(const std::string& title, float* variable, float min, float max){
	std::string observed_item_title(title);
	struct imgui_item<float> item = {
		observed_item_title,
		variable,
		min,
		max
	};

	this->f_variables.push_back(item);
}


void ImGuiController::observei(const std::string& title, int* variable, int min, int max){
	std::string observed_item_title(title);
	struct imgui_item<int> item = {
		observed_item_title,
		variable,
		min,
		max
	};

	this->i_variables.push_back(item);
}


#endif