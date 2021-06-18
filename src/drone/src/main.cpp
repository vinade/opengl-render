#include "engine.hpp"
#include "socket.cpp"

// #include <glm/gtx/rotate_vector.hpp>

Engine engine;
ScenarioItem *spaceship;
Scene *scene = nullptr;
Line *line_magneto_down_reference;
Light *light;
glm::vec3 spaceship_offset(0.0, 0.0, 0.0);

void render_handler()
{
    glm::vec3 cam = scene->camera.get_position();
    scene->camera.m_position.y = 1.7;

    spaceship->set_rotation(glm::vec3(
        (-DroneState::instance->angle.x + spaceship_offset.x),
        180.0 + (-DroneState::instance->angle.z + spaceship_offset.z + 90),
        (-DroneState::instance->angle.y + spaceship_offset.y) // cima, na cena
        ));

    scene->draw();

    glFlush();
}

void preload()
{

    scene = new Scene();
    scene->use_skybox = true;

    light = new Light();
    spaceship = new ScenarioItem(true);

    line_magneto_down_reference = new Line();

    scene->add(light);
    scene->add(spaceship);
    scene->add(line_magneto_down_reference);

    scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    scene->camera.m_position.y = 1.7;
    scene->camera.update_view_matrix();

    light->set_position(glm::vec3(-0.9, 0.9, -2.7));
    light->set_color(glm::vec4(1.0, 1.0, 1.0, 1.0));
    light->set_ambient(2.7);

    spaceship->load_data_from_file("spaceship_1/spaceship_1.obj");
    spaceship->set_position(glm::vec3(0.0, 1.5, -5.0));
    spaceship->set_scale(1.0);
    spaceship->set_rotation(DroneState::instance->angle);

    line_magneto_down_reference->set_position(glm::vec3(0.0, 1.5, -5.0));
    line_magneto_down_reference->set_color(glm::vec4(0.0, 0.0, 0.0, 1.0));
    line_magneto_down_reference->set(glm::vec3(-1.0, 1.0, 1.0));
}

void mouse_handler(int button)
{
    if (Engine::context->cursor_enabled)
    {
        return;
    }

    if (Engine::delta_mouse.x || Engine::delta_mouse.y)
    {
        if (Engine::delta_mouse.x)
        {
            scene->camera.rotate(0.01 * -Engine::delta_mouse.x, glm::vec3(0.0, 1.0, 0.0));
        }

        if (Engine::delta_mouse.y)
        {
            glm::vec3 normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.rotate(0.01 * -Engine::delta_mouse.y, normal_vector);
        }
    }

    scene->camera.update_view_matrix();
}

void keyboard_handler(int key, int scancode, int action, int mods)
{
    glm::vec3 normal_vector;
    if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
    {
        float speed = 0.95;

        if (Engine::check_key(GLFW_KEY_W, key, action))
        {
            scene->camera.translate(speed);
        }

        if (Engine::check_key(GLFW_KEY_S, key, action))
        {
            scene->camera.translate(-speed);
        }

        if (Engine::check_key(GLFW_KEY_A, key, action))
        {
            normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.translate(-speed, normal_vector);
        }

        if (Engine::check_key(GLFW_KEY_D, key, action))
        {
            normal_vector = glm::cross(scene->camera.get_direction(), scene->camera.get_up());
            scene->camera.translate(speed, normal_vector);
        }

        if (Engine::check_key(GLFW_KEY_Q, key, action))
        {
            scene->camera.rotate_up(-speed); // teste
        }

        if (Engine::check_key(GLFW_KEY_E, key, action))
        {
            scene->camera.rotate_up(speed); // teste
        }

        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_ESCAPE:
                Engine::context->switch_cursor_mode();
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

    DroneState::instance = new DroneState();

#ifdef DEBUG_MODE_COMPILE
    engine.render.imgui_controller->observef("fps", &engine.render.fps);

    engine.render.imgui_controller->observef("offset_x", &spaceship_offset[0], -180.0, 180.0);
    engine.render.imgui_controller->observef("offset_y", &spaceship_offset[1], -180.0, 180.0);
    engine.render.imgui_controller->observef("offset_z", &spaceship_offset[2], -180.0, 180.0);
#endif

    engine.set_keyboard_handler(keyboard_handler);
    engine.set_mouse_handler(mouse_handler);
    engine.set_preload(preload);
    engine.set_render_handler(render_handler);
    engine.render.set_size(1024, 768);

    // create_server();

    engine.start();

    return 0;
}