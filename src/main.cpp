#define DEBUG_MODE_COMPILE

#define DEBUG_MODE true

#include "shaders.cpp"
#include "render_window.cpp"
#include "vertex_buffer.cpp"
#include "index_buffer.cpp"
#include "texture.cpp"
#include "vertex_array.cpp"

#include <stdio.h>


void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    if ((type == GL_DEBUG_TYPE_ERROR) || DEBUG_MODE){
        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
               ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                type, severity, message );
    }
}

glm::vec3 translation_debug(0.0f, 0.0f, 0.0f);

glm::mat4 projection_matrix;
glm::mat4 view_matrix;
glm::mat4 mvp_matrix;

Shader *std_shader;

Texture* texture1;

VertexArray* rectangle_vao;
VertexBufferLayout* rectangle_vbo_layout;

VertexBuffer* rectangle_vbo;
IndexBuffer* rectangle_ibo;

unsigned int rectangle_index_buffer;
float rectangle_positions[] = {
	-0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f
};

unsigned int rectangle_indexes[] = {
    0, 1, 2,
    2, 3, 0
};

void init_rectangle(){

    rectangle_vao = new VertexArray();
    rectangle_vbo = new VertexBuffer(rectangle_positions, 4 * 4);
    rectangle_vbo_layout = new VertexBufferLayout();

    rectangle_vbo_layout->push(DATA_TYPE_FLOAT, 2);
    rectangle_vbo_layout->push(DATA_TYPE_FLOAT, 2);
    rectangle_vao->add_buffer(rectangle_vbo, rectangle_vbo_layout);

    rectangle_ibo = new IndexBuffer(rectangle_indexes, 6);
}

void draw_rectangle(){
    texture1->bind();
    rectangle_vao->bind();

    rectangle_ibo->bind();
    rectangle_ibo->draw();
}


void render_handler(){

    view_matrix = glm::translate(glm::mat4(1.0f), translation_debug);
    mvp_matrix = projection_matrix * view_matrix;

    glm::vec4 u_color(0.2, 0.4, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// ...............
    // std_shader->fill("u_Color", &u_color);
    std_shader->fill("u_Color", u_color);
    std_shader->fill("u_Texture", 0);
    std_shader->fill("u_MVP", mvp_matrix);
    std_shader->exec();
    draw_rectangle();

// ...............
    glFlush();

}


void gl_init(){

    /* initialize flags */

    glEnable ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    /* compile shaders */

    std_shader = new Shader();
    std_shader->load("std");
    std_shader->setup("u_Color", DATA_TYPE_VEC4);
    std_shader->setup("u_Texture", DATA_TYPE_INT);
    std_shader->setup("u_MVP", DATA_TYPE_MAT4);


    /* load textures */
    texture1 = new Texture("./res/textures/rocks_1.png");

    /* */
    // projection_matrix = glm::perspective(glm::radians(65.0), 1.0, 10.0, 20000.0);
    projection_matrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    /* create objects */
	init_rectangle();
}


int main(int iArgc, char** cppArgv){

	RenderWindow* render = new RenderWindow();

    #ifdef DEBUG_MODE_COMPILE
        render->imgui_controller->observef("x", &translation_debug[0], -1.0f, 1.0f);
        render->imgui_controller->observef("y", &translation_debug[1], -1.0f, 1.0f);
        render->imgui_controller->observef("z", &translation_debug[2], -1.0f, 1.0f);
    #endif

	render->set_gl_init(gl_init);
	render->set_render_handler(render_handler);
	render->start();

	return 0;
}