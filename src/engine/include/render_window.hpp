#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include "debug_flags.hpp"
#include "app_utils.hpp"
#include "frame_buffer.hpp"
#include "light.hpp"
#include "scene.hpp"
#include <string>
#include <thread>

#define RENDER_WINDOW_OPENGL_VERSION_MAJOR 4
#define RENDER_WINDOW_OPENGL_VERSION_MINOR 6
#define RENDER_WINDOW_WIDTH 800
#define RENDER_WINDOW_HEIGHT 600
#define RENDER_WINDOW_POS_X 100
#define RENDER_WINDOW_POS_Y 100
#define RENDER_WINDOW_RENDER_WAIT 32

#ifdef DEBUG_MODE_COMPILE
#include "imgui_controller.hpp"
#endif

class RenderWindow
{

private:
	bool running = false;
	int pos_x;
	int pos_y;
	int iArgc;
	char **cppArgv;
	std::string title;

	void (*keyboard_handler)(unsigned char key, int x, int y);
	void (*mouse_handler)(int button, int state, int x, int y);
	void (*reshape_handler)(int width, int height);
	void (*render_handler)();

	void init_RenderWindow();
	void init_RenderWindow(int iArgc, char **cppArgv);
	void init_RenderWindow(int iArgc, char **cppArgv, const std::string &title);
	void preload_wrapper();
	void setup_preloaded();
	bool preload_done = false;
	bool setup_done = false;
	void (*gl_init)();
	void (*preload)();

	/* setup list */
	std::vector<Shader *> shader_setup_list;
	std::vector<TileMesh *> tile_mesh_setup_list;
	std::vector<Mesh *> mesh_setup_list;
	std::vector<Texture *> texture_setup_list;
	std::vector<Scene *> scene_setup_list;
	std::vector<FrameBuffer *> frame_buffer_setup_list;

#ifdef DEBUG_MODE_COMPILE
	void render_handler_wrapper(GLFWwindow *window);
#endif

public:
#ifdef DEBUG_MODE_COMPILE
	ImGuiController *imgui_controller;
#endif

	FrameBuffer *fbo_color;
	FrameBuffer *fbo_depth;

	int width;
	int height;

	const static std::thread::id RENDER_THREAD_ID;
	static RenderWindow *context;

	RenderWindow();
	~RenderWindow();
	RenderWindow(int iArgc, char **cppArgv);
	RenderWindow(int iArgc, char **cppArgv, const std::string &title);

	void set_pos(int x, int y);
	void set_size(int width, int height);
	void set_keyboard_handler(void (*handler)(unsigned char key, int x, int y));
	void set_mouse_handler(void (*handler)(int button, int state, int x, int y));
	void set_reshape_handler(void (*handler)(int width, int height));
	void set_render_handler(void (*handler)());
	void set_gl_init(void (*handler)());
	void set_preload(void (*handler)());
	void start();
	void stop();

	void to_setup(Shader *shader) { AppUtils::add_once(this->shader_setup_list, shader); };
	void to_setup(TileMesh *tile_mesh) { AppUtils::add_once(this->tile_mesh_setup_list, tile_mesh); };
	void to_setup(Texture *texture) { AppUtils::add_once(this->texture_setup_list, texture); };
	void to_setup(Mesh *mesh) { AppUtils::add_once(this->mesh_setup_list, mesh); };
	void to_setup(Scene *scene) { AppUtils::add_once(this->scene_setup_list, scene); };
	void to_setup(FrameBuffer *frame_buffer) { AppUtils::add_once(this->frame_buffer_setup_list, frame_buffer); };

	static void update_window_size_info(GLFWwindow *window, int width, int height);
	inline static bool is_render_thread();

	template <typename T>
	static void call_setup(T &obj_list);
};

template <typename T>
void RenderWindow::call_setup(T &obj_list)
{
	for (auto item : obj_list)
	{
		item->setup();
	}
	obj_list.erase(obj_list.begin(), obj_list.end());
}

inline bool RenderWindow::is_render_thread()
{
	return (std::this_thread::get_id() == RenderWindow::RENDER_THREAD_ID);
}

#endif