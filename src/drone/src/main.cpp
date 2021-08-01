#include "engine.hpp"
#include "drone_socket.cpp"

// #include <glm/gtx/rotate_vector.hpp>

Engine engine;
ScenarioItem *spaceship;
Scene *scene = nullptr;
Light *light;
glm::vec3 spaceship_offset(0.0, 0.0, 0.0);
glm::vec3 center_position(0.0, 1.5, -5.0);

Line *line_magneto;
Line *line_mag_front;
Line *line_accel;
Line *line_north;

Line *line_up;
Line *line_right;
Line *line_front;

void render_handler()
{
    glm::vec3 cam = scene->camera.get_position();
    scene->camera.m_position.y = 1.7;

    if (DroneState::instance->use_mock)
    {
        DroneState::instance->read_buffer((char *)NULL);
    }

    line_accel->set(glm::vec3(
        DroneState::instance->accel.x,
        DroneState::instance->accel.z,
        DroneState::instance->accel.y));

    line_magneto->set(glm::vec3(
        DroneState::instance->magneto.x,
        DroneState::instance->magneto.z,
        DroneState::instance->magneto.y));

    line_north->set(glm::vec3(
        DroneState::instance->magneto_north.x,
        DroneState::instance->magneto_north.z,
        DroneState::instance->magneto_north.y));

    line_up->set(glm::vec3(
        DroneState::instance->up.x,
        DroneState::instance->up.z,
        DroneState::instance->up.y));

    line_front->set(glm::vec3(
        DroneState::instance->front.x,
        DroneState::instance->front.z,
        DroneState::instance->front.y));

    line_mag_front->set(glm::vec3(
        DroneState::instance->magneto_front.x,
        DroneState::instance->magneto_front.z,
        DroneState::instance->magneto_front.y));

    line_right->set(glm::vec3(
        DroneState::instance->right.x,
        DroneState::instance->right.z,
        DroneState::instance->right.y));

    // spaceship->set_rotation(glm::vec3(
    //     (-DroneState::instance->angle.x + spaceship_offset.x),
    //     180.0 + (-DroneState::instance->angle.z + spaceship_offset.z + 90),
    //     (-DroneState::instance->angle.y + spaceship_offset.y) // cima, na cena
    //     ));
    spaceship->set_rotation(glm::vec3(
        DroneState::instance->angle.x,
        DroneState::instance->angle.z,
        DroneState::instance->angle.y // cima, na cena
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

    line_mag_front = new Line();
    line_magneto = new Line();
    line_accel = new Line();
    line_north = new Line();

    line_up = new Line();
    line_right = new Line();
    line_front = new Line();

    scene->add(light);
    scene->add(spaceship);
    scene->add(line_magneto);
    scene->add(line_accel);
    scene->add(line_north);
    scene->add(line_up);
    scene->add(line_front);
    scene->add(line_right);
    scene->add(line_mag_front);

    scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    scene->camera.m_position.y = 1.7;
    scene->camera.update_view_matrix();

    light->set_position(glm::vec3(-0.9, 0.9, -2.7));
    light->set_color(glm::vec4(1.0, 1.0, 1.0, 1.0));
    light->set_ambient(2.7);

    spaceship->load_data_from_file("spaceship_1/spaceship_1.obj");
    spaceship->set_position(center_position);
    spaceship->set_scale(1.0);
    spaceship->set_rotation(DroneState::instance->angle);

    line_magneto->set_position(center_position);
    line_magneto->set_color(glm::vec4(0.0, 0.0, 0.0, 1.0));
    line_mag_front->set_position(center_position);
    line_mag_front->set_color(glm::vec4(0.5, 0.0, 0.0, 1.0));
    line_accel->set_position(center_position);
    line_accel->set_color(glm::vec4(1.0, 1.0, 0.0, 1.0));
    line_north->set_position(center_position);
    line_north->set_color(glm::vec4(1.0, 0.0, 1.0, 1.0));

    glm::vec4 directions_color(0.8, 0.8, 0.8, 1.0);
    line_up->set_position(center_position);
    line_front->set_position(center_position);
    line_right->set_position(center_position);

    line_up->set_color(directions_color);
    line_front->set_color(directions_color);
    line_right->set_color(directions_color);
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

void init_server(int argc, char **argv)
{
    if (argc == 2)
    {
        std::string arg2 = std::string(argv[1]);

        if (arg2.compare("server") == 0)
        {
            create_server();
        }
    }
}

int main(int argc, char **argv)
{

    DroneState::instance = new DroneState();
    // DroneState::instance->use_mock = true;

#ifdef DEBUG_MODE_COMPILE
    engine.render.imgui_controller->observef("fps", &engine.render.fps);

    engine.render.imgui_controller->observef("offset_x", &spaceship_offset[0], -180.0, 180.0);
    engine.render.imgui_controller->observef("offset_y", &spaceship_offset[1], -180.0, 180.0);
    engine.render.imgui_controller->observef("offset_z", &spaceship_offset[2], -180.0, 180.0);

    engine.render.imgui_controller->observef("mock: Pitch", &DroneState::instance->mock_pitch, -180.0, 180.0);
    engine.render.imgui_controller->observef("mock: Roll", &DroneState::instance->mock_roll, -180.0, 180.0);

    engine.render.imgui_controller->observef("mock: MagAlpha", &DroneState::instance->mock_mag_alpha, -180.0, 180.0);
    engine.render.imgui_controller->observef("mock: MagBeta", &DroneState::instance->mock_mag_beta, 0, 180.0);

    engine.render.imgui_controller->observef("mock: AccelAlpha", &DroneState::instance->mock_accel_alpha, -180.0, 180.0);
    engine.render.imgui_controller->observef("mock: AccelBeta", &DroneState::instance->mock_accel_beta, 0, 180.0);

    engine.render.imgui_controller->observev3("P.R.Y.", &DroneState::instance->angle);
    engine.render.imgui_controller->observev3("accel", &DroneState::instance->accel);
    engine.render.imgui_controller->observev3("magneto", &DroneState::instance->magneto);
    engine.render.imgui_controller->observev3("coords", &DroneState::instance->coords);
    engine.render.imgui_controller->observev3("mag_north", &DroneState::instance->magneto_north);
    engine.render.imgui_controller->observev3("mag_down", &DroneState::instance->magneto_down);
    engine.render.imgui_controller->observev3("mag_front", &DroneState::instance->magneto_front);
    engine.render.imgui_controller->observev2("mag_front_offset", &DroneState::instance->magneto_offset_front);

    engine.render.imgui_controller->button("Set initialized", DroneState::instance->set_initialized);

#endif

    engine.set_keyboard_handler(keyboard_handler);
    engine.set_mouse_handler(mouse_handler);
    engine.set_preload(preload);
    engine.set_render_handler(render_handler);
    engine.render.set_size(1024, 768);

    init_server(argc, argv);

    engine.start();

    return 0;
}