#include <debug_flags.hpp>
#include "render_window.hpp"
#include "camera.hpp"
#include "perspective.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "scene_item.hpp"
#include "light.hpp"

#include <stdio.h>

#define UNUSED(x) (void)(x)
void GLAPIENTRY
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam)
{
    if ((type == GL_DEBUG_TYPE_ERROR) || DEBUG_MODE)
    {
        UNUSED(source);
        UNUSED(id);
        UNUSED(length);
        UNUSED(userParam);

        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
    }
}

Camera camera;
Light light_0;
SceneItem cat_1;
SceneItem rocks_1;
SceneItem moon_1;

glm::vec3 model_rotation_debug(0.0f, 0.0f, 0.0f);
glm::vec3 view_translation_debug(0.0f, 0.0f, 0.0f);

Shader *std_shader;

void render_handler()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.set_position(view_translation_debug);
    camera.update_view_matrix();

    rocks_1.draw();
    cat_1.draw();

    light_0.inc_position(glm::vec3(0.001f, -0.001f, 0.0f));
    light_0.draw();

    moon_1.inc_rotation(glm::vec3(0.0f, 0.001f, 0.0f));
    moon_1.draw();

    glFlush();
}

void gl_init()
{

    /* initialize flags */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    /* compile shaders */

    std_shader = Shader::getShader("std");
    std_shader->setup("u_Color", DATA_TYPE_VEC4);
    std_shader->setup("u_Texture", DATA_TYPE_INT);
    std_shader->setup("u_MVP", DATA_TYPE_MAT4);

    /* load textures */
    // texture1 = new Texture("./res/textures/rocks_1.png");
    light_0.set_position(glm::vec3(-80.0f, 40.0f, -300.0f));

    cat_1.load_data_from_file("cat_1/12221_Cat_v1_l3.obj");
    cat_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    cat_1.set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    cat_1.set_scale(200.0);

    rocks_1.load_data_from_file("simple_1/12221_Simple_v1_l3.obj");
    rocks_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    // rocks_1.set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    rocks_1.set_scale(200.0);

    moon_1.load_data_from_file("moon_1/Moon 2K.obj");
    moon_1.set_position(glm::vec3(0.0f, 0.0f, -400.0f));
    moon_1.set_scale(200.0);

    Perspective::set_default();

    std::cerr << "[done]" << std::endl;
}

int main()
{
    RenderWindow *render = new RenderWindow();

#ifdef DEBUG_MODE_COMPILE
    render->imgui_controller->observef("alpha", &model_rotation_debug[0], -180.0f, 180.0f);
    render->imgui_controller->observef("beta", &model_rotation_debug[1], -180.0f, 180.0f);
    render->imgui_controller->observef("gamma", &model_rotation_debug[2], -180.0f, 180.0f);

    render->imgui_controller->observef("x", &view_translation_debug[0], -1000.0f, 1000.0f);
    render->imgui_controller->observef("y", &view_translation_debug[1], -1000.0f, 1000.0f);
    render->imgui_controller->observef("z", &view_translation_debug[2], -1000.0f, 1000.0f);
#endif

    render->set_gl_init(gl_init);
    render->set_render_handler(render_handler);
    render->start();

    return 0;
}