#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>

#define RENDER_WINDOW_WIDTH 800
#define RENDER_WINDOW_HEIGHT 600
#define RENDER_WINDOW_POS_X 100
#define RENDER_WINDOW_POS_Y 100
#define RENDER_WINDOW_RENDER_WAIT 32

class RenderWindow {

private:
	bool running = false;
	int width;
	int height;
	int pos_x;
	int pos_y;
	int iArgc;
	char** cppArgv;
	const char* title;

	void (*gl_init)();
	void (*keyboard_handler)(unsigned char key, int x, int y);
	void (*mouse_handler)(int button, int state, int x, int y);
	void (*reshape_handler)(int width, int height);
	void (*render_handler)();

	static void refresh(int data);

	void init_RenderWindow();
	void init_RenderWindow(int iArgc, char** cppArgv);
	void init_RenderWindow(int iArgc, char** cppArgv, const std::string& title);

public:
	RenderWindow();
	RenderWindow(int iArgc, char** cppArgv);
	RenderWindow(int iArgc, char** cppArgv, const std::string& title);


	void set_pos(int x, int y);
	void set_size(int width, int height);
	void set_keyboard_handler(void (*handler)(unsigned char key, int x, int y));
	void set_mouse_handler(void (*handler)(int button, int state, int x, int y));
	void set_reshape_handler(void (*handler)(int width, int height));
	void set_render_handler(void (*handler)());
	void set_gl_init(void (*handler)());
	void start();
	void stop();
};

#endif