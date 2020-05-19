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
#include <cstdlib>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

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

int frame_buffer_select = 0;
Light *light_0;
Light *light_1;
ScenarioItem *cat_1;
ScenarioItem *moon_1;
ScenarioItem *plant_1;
ScenarioItem *nanosuit_1;
Scene *scene = nullptr;
RenderWindow *render;

glm::vec3 color_light_debug(1.0f, 1.0f, 1.0f);
glm::vec3 light_translation_debug(-0.9f, 0.9f, -2.7f);
float light_ambient_debug = 0.2;

void render_handler()
{

    light_0->set_position(light_translation_debug);
    light_0->set_color(glm::vec4(color_light_debug, 1.0));
    light_0->set_ambient(light_ambient_debug);

    light_1->set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));

    moon_1->inc_rotation(glm::vec3(0.0f, 0.01f, 0.0f));

    switch (frame_buffer_select)
    {
    default:
    case 0:
        scene->draw();
        break;
    case 1:
        scene->update_color_buffer(render);
        render->fbo_color->draw();
        // render->fbo_color->save("teste.ppm");
        break;
    case 2:
        scene->update_depth_buffer(render);
        render->fbo_depth->draw();
        // render->fbo_depth->save("teste.ppm");
        break;
    }

    glFlush();
}

void gl_init()
{

    /* initialize flags */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    std::cerr << "[done]" << std::endl;
}

void preload()
{

    scene = new Scene();

    light_0 = new Light();
    light_1 = new Light();
    cat_1 = new ScenarioItem();
    moon_1 = new ScenarioItem();
    plant_1 = new ScenarioItem();
    nanosuit_1 = new ScenarioItem();

    // scene->init();
    scene->add(light_0);
    scene->add(light_1);
    scene->add(cat_1);
    scene->add(moon_1);
    scene->add(nanosuit_1);
    scene->add(plant_1);
    scene->camera.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    scene->camera.update_view_matrix();

    light_1->set_position(glm::vec3(0.5, 0.5, -2.0));
    light_1->set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
    light_1->set_ambient(0.0);

    cat_1->load_data_from_file("cat_1/12221_Cat_v1_l3.obj");
    cat_1->set_position(glm::vec3(-1.0f, -1.0f, -4.0f));
    cat_1->set_rotation(glm::vec3(90.0f, -180.0f, 150.0f));
    cat_1->set_scale(1.0);

    moon_1->load_data_from_file("moon_1/Moon 2K.obj");
    moon_1->set_position(glm::vec3(-2.0f, 2.0f, -8.0f));
    moon_1->set_scale(2.0);

    plant_1->load_data_from_file("plant/01Alocasia_obj.obj");
    plant_1->set_position(glm::vec3(1.0f, 0.0f, -4.0f));
    plant_1->set_scale(1.5);

    nanosuit_1->load_data_from_file("nano_suit/Nanosuit.obj");
    nanosuit_1->set_position(glm::vec3(0.0f, 0.0f, -5.0f));
    nanosuit_1->set_rotation(glm::vec3(90.0f, -180.0f, 0.0f));
    nanosuit_1->set_scale(2.0);
}

void shuffle_materials()
{

    for (auto si : scene->scenario_items)
    {
        for (auto &m : si->meshes)
        {
            m.material = Material::materials[std::rand() % Material::materials.size()];
        }
    }
}

void shuffle_direction()
{
    int i = std::rand() % scene->scenario_items.size();
    ScenarioItem *si = scene->scenario_items[i];
    scene->camera.point_to(si);
    scene->camera.update_view_matrix();
}

void mouse_handler(int button)
{
    if (RenderWindow::context->cursor_enabled)
    {
        return;
    }

    if (RenderWindow::delta_mouse.x || RenderWindow::delta_mouse.y)
    {
        if (RenderWindow::delta_mouse.x)
        {
            scene->camera.rotate(0.01 * -RenderWindow::delta_mouse.x, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if (RenderWindow::delta_mouse.y)
        {
            glm::vec3 normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.rotate(0.01 * -RenderWindow::delta_mouse.y, normal_vector);
        }
    }

    scene->camera.update_view_matrix();
}

void keyboard_handler(int key, int scancode, int action, int mods)
{
    glm::vec3 normal_vector;
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
    {
        float speed = 0.05;

        if (RenderWindow::check_key(GLFW_KEY_W, key, action))
        {
            scene->camera.translate(speed);
        }

        if (RenderWindow::check_key(GLFW_KEY_S, key, action))
        {
            scene->camera.translate(-speed);
        }

        if (RenderWindow::check_key(GLFW_KEY_A, key, action))
        {
            normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.translate(-speed, normal_vector);
        }

        if (RenderWindow::check_key(GLFW_KEY_D, key, action))
        {
            normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.translate(speed, normal_vector);
        }

        if (RenderWindow::check_key(GLFW_KEY_Q, key, action))
        {
            scene->camera.rotate_up(-speed); // teste
        }

        if (RenderWindow::check_key(GLFW_KEY_E, key, action))
        {
            scene->camera.rotate_up(speed); // teste
        }

        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_ESCAPE:
                RenderWindow::context->switch_cursor_mode();
                break;
            default:
                break;
            }
        }

        scene->camera.update_view_matrix();
    }
}

int main()
{
    std::srand(std::time(nullptr));

    render = new RenderWindow();

#ifdef DEBUG_MODE_COMPILE
    render->imgui_controller->observef("ambient", &light_ambient_debug, 0.0f, 1.0f);
    render->imgui_controller->observef("R", &color_light_debug[0], 0.0f, 1.0f);
    render->imgui_controller->observef("G", &color_light_debug[1], 0.0f, 1.0f);
    render->imgui_controller->observef("B", &color_light_debug[2], 0.0f, 1.0f);

    render->imgui_controller->observef("x", &light_translation_debug[0], -5.0f, 5.0f);
    render->imgui_controller->observef("y", &light_translation_debug[1], -5.0f, 5.0f);
    render->imgui_controller->observef("z", &light_translation_debug[2], -10.0f, 3.0f);

    render->imgui_controller->radio("FrameBuffer", &frame_buffer_select, 3);
    render->imgui_controller->button("Shuffle materials", shuffle_materials);
    render->imgui_controller->button("point to objects", shuffle_direction);

#endif

    render->set_keyboard_handler(keyboard_handler);
    render->set_mouse_handler(mouse_handler);
    render->set_preload(preload);
    render->set_gl_init(gl_init);
    render->set_render_handler(render_handler);
    render->start();

    return 0;
}