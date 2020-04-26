#include "render_window.hpp"


RenderWindow::RenderWindow(){
    this->init_RenderWindow();
}


RenderWindow::RenderWindow(int iArgc, char** cppArgv){
    this->init_RenderWindow(iArgc, cppArgv);
}


RenderWindow::RenderWindow(int iArgc, char** cppArgv, const std::string& title){
    this->init_RenderWindow(iArgc, cppArgv, title);
}


void RenderWindow::init_RenderWindow(){
	std::string arg_a("argument");
	int iArgc = 1;
	char** cppArgv;

	cppArgv = (char**)malloc(sizeof(char*));
	cppArgv[0] = const_cast<char*>(arg_a.c_str());

    this->init_RenderWindow(iArgc, cppArgv);
}


void RenderWindow::init_RenderWindow(int iArgc, char** cppArgv){
	std::string title("Render");
    this->init_RenderWindow(iArgc, cppArgv, title);
}


void RenderWindow::init_RenderWindow(int iArgc, char** cppArgv, const std::string& title){
	this->title = title.c_str();
	this->iArgc = iArgc;
	this->cppArgv = cppArgv;
	this->width = RENDER_WINDOW_WIDTH;
	this->height = RENDER_WINDOW_HEIGHT;
	this->pos_x = RENDER_WINDOW_POS_X;
	this->pos_y = RENDER_WINDOW_POS_Y;
}


void RenderWindow::set_pos(int x, int y){
	this->pos_x = x;
	this->pos_y = y;
}


void RenderWindow::set_size(int width, int height){
	this->width = width;
	this->height = height;
}


void RenderWindow::set_keyboard_handler(void (*handler)(unsigned char key, int x, int y)){
	this->keyboard_handler = handler;
}

void RenderWindow::set_mouse_handler(void (*handler)(int button, int state, int x, int y)){
	this->mouse_handler = handler;
}

void RenderWindow::set_reshape_handler(void (*handler)(int width, int height)){
	this->reshape_handler = handler;
}


void RenderWindow::set_render_handler(void (*handler)()){
	this->render_handler = handler;
}


void RenderWindow::set_gl_init(void (*handler)()){
	this->gl_init = handler;
}


void RenderWindow::refresh( int data ){
  glutPostRedisplay();
  glutTimerFunc( RENDER_WINDOW_RENDER_WAIT, RenderWindow::refresh, -1);
}


void RenderWindow::start(){

	if (this->running){
		std::cerr << "[RENDER_WINDOW] start() deve ser chamado apenas umas vez." << std::endl;
		return;
	}

	if (this->render_handler == nullptr){
		std::cerr << "[RENDER_WINDOW] start() chamado sem um handler para o Render ser definido." << std::endl;
		std::cerr << "\tExemplo: render_window->set_render_handler(alguma_funcao)." << std::endl;
		return;
	}

	this->running = true;

	glutInit(&this->iArgc, this->cppArgv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(this->width, this->height);
	glutInitWindowPosition(this->pos_x, this->pos_y);
	glutCreateWindow(this->title); // BUG: Não está exibindo o título

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(1);
	}

	if (this->gl_init != nullptr){
		(*this->gl_init)();
	}

	if (this->keyboard_handler != nullptr){
		glutKeyboardFunc(this->keyboard_handler);
	}

	if (this->mouse_handler != nullptr){
		glutMouseFunc(this->mouse_handler);
	}

	if (this->reshape_handler != nullptr){
		glutReshapeFunc(this->reshape_handler);
	}

	glutDisplayFunc(this->render_handler);
	glutTimerFunc(RENDER_WINDOW_RENDER_WAIT, RenderWindow::refresh, -1);
	glutMainLoop();

}


void RenderWindow::stop(){
	glutLeaveMainLoop();
	this->running = false;
}
