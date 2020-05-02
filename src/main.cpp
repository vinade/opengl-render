#define DEBUG_MODE_COMPILE

#define DEBUG_MODE true

#include "shaders.cpp"
#include "render_window.cpp"
#include "vertex_buffer.cpp"
#include "index_buffer.cpp"
#include "texture.cpp"
#include "vertex_array.cpp"
#include "scene_item.cpp"

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

SceneItem cat_1;
SceneItem rocks_1;
SceneItem moon_1;


glm::vec3 model_rotation_debug(0.0f, 0.0f, 0.0f);
glm::vec3 view_translation_debug(0.0f, 0.0f, 0.0f);

glm::mat4 projection_matrix;
glm::mat4 view_matrix;
glm::mat4 model_matrix;
glm::mat4 mvp_matrix;

Shader *std_shader;

Texture* texture1;

VertexArray* rectangle_vao;
VertexBufferLayout* rectangle_vbo_layout;

VertexBuffer* rectangle_vbo;
IndexBuffer* rectangle_ibo;

unsigned int rectangle_index_buffer;
float rectangle_positions[] = {
	-5.0f, -5.0f, -30.0f, 0.0f, 0.0f,
	 5.0f, -5.0f, -30.0f, 1.0f, 0.0f,
	 5.0f,  5.0f, -30.0f, 1.0f, 1.0f,
    -5.0f,  5.0f, -30.0f, 0.0f, 1.0f
};

unsigned int rectangle_indexes[] = {
    0, 1, 2,
    0, 2, 3
};

void init_rectangle(){

    rectangle_vao = new VertexArray();
    rectangle_vbo = new VertexBuffer(rectangle_positions, 5 * 4);
    rectangle_vbo_layout = new VertexBufferLayout();

    rectangle_vbo_layout->push(DATA_TYPE_FLOAT, 3);
    rectangle_vbo_layout->push(DATA_TYPE_FLOAT, 2);
    rectangle_vao->add_buffer(rectangle_vbo, rectangle_vbo_layout);

    rectangle_ibo = new IndexBuffer(rectangle_indexes, 6);
}

void draw_scene_item(SceneItem& item){  // TODO: passar tudo para o draw do item

    model_matrix = item.get_model_matrix();
    view_matrix = glm::translate(glm::mat4(1.0f), view_translation_debug);  // TODO: Colocar na Câmera
    mvp_matrix = projection_matrix * view_matrix * model_matrix;

    glm::vec4 u_color(0.2, 0.4, 0.8, 1.0);

    // std_shader->fill("u_Color", &u_color);
    std_shader->fill("u_Color", u_color);
    std_shader->fill("u_Texture", 0);
    std_shader->fill("u_MVP", mvp_matrix);
    std_shader->exec();
    item.draw();

}


void render_handler(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_scene_item(rocks_1);
    draw_scene_item(cat_1);

    moon_1.inc_rotation(glm::vec3(0.0f, 0.001f, 0.0f));
    draw_scene_item(moon_1);

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
    // texture1 = new Texture("./res/textures/rocks_1.png");

    cat_1.load_data_from_file("./res/models/cat_1/12221_Cat_v1_l3.obj");
    cat_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    cat_1.set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    cat_1.set_scale(200.0);

    rocks_1.load_data_from_file("./res/models/simple_1/12221_Simple_v1_l3.obj");
    rocks_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    // rocks_1.set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    rocks_1.set_scale(200.0);

    moon_1.load_data_from_file("./res/models/moon_1/Moon 2K.obj");
    moon_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    moon_1.set_scale(200.0);

    std::cerr << "[done]" << std::endl;

    /* */
    projection_matrix = glm::perspective(glm::radians(45.0), 1.0, 1.0, 2000.0);  // TODO: Colocar no RenderWindow

    /* create objects */
	init_rectangle();
}


int main(int iArgc, char** cppArgv){

	RenderWindow* render = new RenderWindow();

    #ifdef DEBUG_MODE_COMPILE
        render->imgui_controller->observef("alpha", &model_rotation_debug[0], -180.0f, 180.0f);
        render->imgui_controller->observef("beta", &model_rotation_debug[1], -180.0f, 180.0f);
        render->imgui_controller->observef("gamma", &model_rotation_debug[2], -180.0f, 180.0f);

        render->imgui_controller->observef("x", &view_translation_debug[0], -100.0f, 100.0f);
        render->imgui_controller->observef("y", &view_translation_debug[1], -100.0f, 100.0f);
        render->imgui_controller->observef("z", &view_translation_debug[2], -1000.0f, 1000.0f);
    #endif

	render->set_gl_init(gl_init);
	render->set_render_handler(render_handler);
	render->start();

	return 0;
}