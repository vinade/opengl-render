#include <debug_flags.hpp>
#include "render_window.hpp"
#include "camera.hpp"
#include "perspective.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "scenario_item.hpp"
#include "light.hpp"
#include "scene.hpp"

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

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        exit(1);
    }
}

Light light_0;
Light light_1;
ScenarioItem cat_1;
ScenarioItem rocks_1;
ScenarioItem moon_1;
ScenarioItem plant_1;
Scene scene;

glm::vec3 color_light_debug(1.0f, 0.0f, 0.0f);
glm::vec3 light_translation_debug(-80.0f, 40.0f, -300.0f);
float light_ambient_debug = 0.2;

void render_handler()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // scene.camera.set_position(view_translation_debug);
    // scene.camera.update_view_matrix();

    light_0.set_position(light_translation_debug);
    light_0.set_color(glm::vec4(color_light_debug, 1.0));
    light_0.set_ambient(light_ambient_debug);

    light_1.set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));

    moon_1.inc_rotation(glm::vec3(0.0f, 0.01f, 0.0f));

    scene.draw();

    glFlush();
}

void gl_init()
{

    /* initialize flags */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    scene.camera.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    scene.camera.update_view_matrix();
    /* load textures */
    // texture1 = new Texture("./res/textures/rocks_1.png");
    light_0.set_position(glm::vec3(-80.0f, 40.0f, -300.0f));
    light_0.set_ambient(light_ambient_debug);

    light_1.set_position(glm::vec3(100.0, 50.0, -300.0));
    light_1.set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
    light_1.set_ambient(0.0);

    cat_1.load_data_from_file("cat_1/12221_Cat_v1_l3.obj");
    cat_1.set_position(glm::vec3(0.0f, -50.0f, -400.0f));
    cat_1.set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    cat_1.set_scale(100.0);

    // rocks_1.load_data_from_file("simple_1/12221_Simple_v1_l3.obj");
    // rocks_1.set_position(glm::vec3(0.0f, 0.0f, -450.0f));
    // rocks_1.set_scale(200.0);

    moon_1.load_data_from_file("moon_1/Moon 2K.obj");
    moon_1.set_position(glm::vec3(-200.0f, 200.0f, -800.0f));
    moon_1.set_scale(200.0);

    plant_1.load_data_from_file("plant/01Alocasia_obj.obj");
    plant_1.set_position(glm::vec3(50.0f, 0.0f, -400.0f));
    plant_1.set_scale(150.0);

    scene.init();
    scene.add_light(light_0);
    scene.add_light(light_1);
    scene.add_scenario_item(cat_1);
    // scene.add_scenario_item(rocks_1);
    scene.add_scenario_item(moon_1);
    scene.add_scenario_item(plant_1);

    std::cerr << "[done]" << std::endl;
}

int main()
{
    RenderWindow *render = new RenderWindow();

#ifdef DEBUG_MODE_COMPILE
    render->imgui_controller->observef("ambient", &light_ambient_debug, 0.0f, 1.0f);
    render->imgui_controller->observef("R", &color_light_debug[0], 0.0f, 1.0f);
    render->imgui_controller->observef("G", &color_light_debug[1], 0.0f, 1.0f);
    render->imgui_controller->observef("B", &color_light_debug[2], 0.0f, 1.0f);

    render->imgui_controller->observef("x", &light_translation_debug[0], -1000.0f, 1000.0f);
    render->imgui_controller->observef("y", &light_translation_debug[1], -1000.0f, 1000.0f);
    render->imgui_controller->observef("z", &light_translation_debug[2], -1000.0f, 1000.0f);
#endif

    render->set_gl_init(gl_init);
    render->set_render_handler(render_handler);
    render->start();

    return 0;
}