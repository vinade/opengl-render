#ifndef RENDER_WINDOW_CPP
#define RENDER_WINDOW_CPP

#include <iostream>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_window.hpp"
#include "tile.hpp"
#include "basic_scene.hpp"
#include "scene.hpp"

RenderWindow::RenderWindow()
{
	this->init_RenderWindow();
}

RenderWindow::~RenderWindow()
{
#ifdef DEBUG_MODE_COMPILE
	delete this->imgui_controller;
#endif
}

RenderWindow::RenderWindow(int iArgc, char **cppArgv)
{
	this->init_RenderWindow(iArgc, cppArgv);
}

RenderWindow::RenderWindow(int iArgc, char **cppArgv, const std::string &title)
{
	this->init_RenderWindow(iArgc, cppArgv, title);
}

void RenderWindow::init_RenderWindow()
{
	std::string arg_a("argument");
	int iArgc = 1;
	char **cppArgv;

	cppArgv = (char **)malloc(sizeof(char *));
	cppArgv[0] = const_cast<char *>(arg_a.c_str());

	this->init_RenderWindow(iArgc, cppArgv);
}

void RenderWindow::init_RenderWindow(int iArgc, char **cppArgv)
{
	std::string title("Render");
	this->init_RenderWindow(iArgc, cppArgv, title);
}

void RenderWindow::init_RenderWindow(int iArgc, char **cppArgv, const std::string &title)
{
	this->title = title;
	this->iArgc = iArgc;
	this->cppArgv = cppArgv;
	this->width = RENDER_WINDOW_WIDTH;
	this->height = RENDER_WINDOW_HEIGHT;
	this->pos_x = RENDER_WINDOW_POS_X;
	this->pos_y = RENDER_WINDOW_POS_Y;
	this->fbo_color = new FrameBuffer();
	this->fbo_depth = new FrameBuffer(true);

#ifdef DEBUG_MODE_COMPILE
	this->imgui_controller = new ImGuiController();
#endif
}

void RenderWindow::set_pos(int x, int y)
{
	this->pos_x = x;
	this->pos_y = y;
}

void RenderWindow::set_size(int width, int height)
{
	this->width = width;
	this->height = height;
}

void RenderWindow::set_keyboard_handler(void (*handler)(unsigned char key, int x, int y))
{
	this->keyboard_handler = handler;
}

void RenderWindow::set_mouse_handler(void (*handler)(int button, int state, int x, int y))
{
	this->mouse_handler = handler;
}

void RenderWindow::set_reshape_handler(void (*handler)(int width, int height))
{
	this->reshape_handler = handler;
}

void RenderWindow::set_render_handler(void (*handler)())
{
	this->render_handler = handler;
}

void RenderWindow::set_gl_init(void (*handler)())
{
	this->gl_init = handler;
}

void RenderWindow::set_preload(void (*handler)())
{
	this->preload = handler;
}

void RenderWindow::preload_wrapper()
{

	this->fbo_color->set();
	this->fbo_depth->set();

	if (this->preload != nullptr)
	{
		(*this->preload)();
	}

	this->preload_done = true;
}

void RenderWindow::setup_preloaded()
{
	std::cerr << "Setup..." << std::endl;

	RenderWindow::call_setup(this->shader_setup_list);
	std::cerr << "\tShaders ready." << std::endl;

	RenderWindow::call_setup(this->texture_setup_list);
	std::cerr << "\tTextures ready." << std::endl;

	RenderWindow::call_setup(this->tile_mesh_setup_list);
	std::cerr << "\tTileMeshes ready." << std::endl;

	RenderWindow::call_setup(this->mesh_setup_list);
	std::cerr << "\tMeshes ready." << std::endl;

	RenderWindow::call_setup(this->scene_setup_list);
	std::cerr << "\tScene ready." << std::endl;

	RenderWindow::call_setup(this->frame_buffer_setup_list);
	std::cerr << "\tFrameBuffers ready." << std::endl;

	this->setup_done = true;
}

#ifdef DEBUG_MODE_COMPILE
void RenderWindow::render_handler_wrapper(GLFWwindow *window)
{
	this->render_handler();
	this->imgui_controller->display(window);
}
#endif

void RenderWindow::start()
{
	BasicScene splash_screen_scene;
	Tile splash_screen_logo;

	RenderWindow::context = this;
	if (this->running)
	{
		std::cerr << "[RENDER_WINDOW] start() deve ser chamado apenas umas vez." << std::endl;
		return;
	}

	if (this->render_handler == nullptr)
	{
		std::cerr << "[RENDER_WINDOW] start() chamado sem um handler para o Render ser definido." << std::endl;
		std::cerr << "\tExemplo: render_window->set_render_handler(alguma_funcao)." << std::endl;
		return;
	}

	this->running = true;

	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cerr << "[RenderWindow] glfwInit retornou um erro." << std::endl;
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, RENDER_WINDOW_OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, RENDER_WINDOW_OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cerr << "[RenderWindow] glfwCreateWindow retornou um erro." << std::endl;
		exit(1);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, update_window_size_info);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "[RenderWindow] glewInit retornou um erro." << std::endl;
		exit(1);
	}

	// Inicializa a splash screen
	splash_screen_logo.set("splash.png"); // TODO: está carregando o skybox em algum momento.
	splash_screen_scene.add(splash_screen_logo);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	if (this->gl_init != nullptr)
	{
		(*this->gl_init)();
	}

	// Chama o preloader
	std::thread([this] { preload_wrapper(); }).detach();

#ifdef DEBUG_MODE_COMPILE
	this->imgui_controller->init(window);
#endif

	while (!glfwWindowShouldClose(window) && this->running)
	{

		if (this->preload_done)
		{

			if (!this->setup_done)
			{
				// Termina a inicialização dos objetos que foram carregados fora da thread principal
				// (não é possível chamar métodos do opengl fora da thread principal)
				this->setup_preloaded();
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DEBUG_MODE_COMPILE
			this->render_handler_wrapper(window);
#else
			this->render_handler();
#endif
		}
		else
		{
			// Enquanto a thread de preload não termine, exibe uma splash screen
			// para evitar deixar a thread principal ocupada.
			// (evitando a mensagem de "não respondendo" do SO para o processo)

			splash_screen_logo.set_size_in_pixels(256, 256);
			splash_screen_logo.center_x();
			splash_screen_logo.center_y();
			splash_screen_scene.draw_tiles();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void RenderWindow::stop()
{
	this->running = false;
}

void RenderWindow::update_window_size_info(GLFWwindow *window, int width, int height)
{
	RenderWindow::context->width = width;
	RenderWindow::context->height = height;
}

RenderWindow *RenderWindow::context = nullptr;
const std::thread::id RenderWindow::RENDER_THREAD_ID = std::this_thread::get_id();

#endif