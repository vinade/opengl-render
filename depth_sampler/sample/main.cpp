#define DEBUG_MODE_COMPILE
#define DEBUG_MODE true

#include <debug_flags.hpp>
#include "camera.hpp"
#include "perspective.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "scenario_item.hpp"
#include "height_map.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "post_process.hpp"
#include "tile.hpp"
#include "material_loader.hpp"

#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Engine engine;
int frame_buffer_select = 0;

ScenarioItem *cat_1;
ScenarioItem *moon_1;
ScenarioItem *plant_1;
ScenarioItem *nanosuit_1;
HeightMap *height_map;
Scene *scene = nullptr;

Light *light_0;
Light *light_1;
glm::vec3 color_light_debug(1.0, 1.0, 1.0);
glm::vec3 light_translation_debug(-0.9, 0.9, -2.7);
float light_ambient_debug = 0.1;
float light_strength_debug = 1.0;

GaussianNoise *pp_gaussian_noise = nullptr;
GaussianBlur *pp_gaussian_blur = nullptr;
float noise_level_debug = 0.0;
float blur_level_debug = 0.0;

void render_handler()
{
    pp_gaussian_noise->set_level(noise_level_debug);
    pp_gaussian_blur->set_range(int(blur_level_debug));

    glm::vec3 cam = scene->camera.get_position();
    float height = height_map->get_height(cam.x, cam.z);
    scene->camera.m_position.y = height + 1.7;

    light_0->set_position(light_translation_debug);
    light_0->set_color(glm::vec4(color_light_debug, 1.0));
    light_0->set_ambient(light_ambient_debug);
    light_0->set_strength(light_strength_debug);

    light_1->set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));

    moon_1->inc_rotation(glm::vec3(0.0, 0.01f, 0.0));

    scene->skybox->inc_rotation(glm::vec3(0.0, 0.01f, 0.0));

    switch (frame_buffer_select)
    {
    default:
    case 0:
        scene->draw();
        break;
    case 1:
        scene->update_color_buffer(&engine.render);
        engine.render.fbo_color->draw();
        // render->fbo_color->save("teste.ppm");
        break;
    case 2:
        scene->update_depth_buffer(&engine.render);
        engine.render.fbo_depth->draw();
        // render->fbo_depth->save("teste.ppm");
        break;
    }

    glFlush();
}

void preload()
{

    scene = new Scene();
    scene->use_skybox = true;

    // height_map = new HeightMap("h2.jpeg");
    height_map = new HeightMap(15, 15);
    light_0 = new Light();
    light_1 = new Light();
    cat_1 = new ScenarioItem();
    moon_1 = new ScenarioItem();
    plant_1 = new ScenarioItem();
    nanosuit_1 = new ScenarioItem();
    pp_gaussian_noise = new GaussianNoise("gaussian_noise.post");
    pp_gaussian_blur = new GaussianBlur("gaussian_blur.post");

    scene->add(pp_gaussian_blur);
    scene->add(pp_gaussian_noise);

    scene->add(light_0);
    // scene->add(light_1);

    scene->add(cat_1);
    scene->add(moon_1);
    scene->add(nanosuit_1);
    scene->add(plant_1);

    scene->add(height_map);
    height_map->set_position(glm::vec3(0.0, 0.0, 0.0));
    height_map->set_scale(glm::vec3(10.0, 5.0, 10.0));

    // float height = height_map->get_height(-3.4, 2.7);
    scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    float height = height_map->get_height(scene->camera.m_position.x, scene->camera.m_position.z);
    scene->camera.m_position.y = height + 1.7;
    scene->camera.update_view_matrix();

    pp_gaussian_noise->every_frame = true;

    light_1->set_position(glm::vec3(0.5, height + 0.5, -2.0));
    light_1->set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
    light_1->set_ambient(0.0);

    // cat_1->load_data_from_file("bricks/brick01a.obj");
    // cat_1->load_data_from_file("bricks/brick01b.obj");
    // cat_1->load_data_from_file("bricks/brick01c.obj");
    // cat_1->load_data_from_file("bricks/brick01d.obj");
    // cat_1->load_data_from_file("bricks/brick01e.obj");
    // cat_1->load_data_from_file("bricks/brick01f.obj");
    // cat_1->load_data_from_file("bricks/brick01g.obj");
    // cat_1->load_data_from_file("plant_2/potted_plant_obj.obj");
    // cat_1->load_data_from_file("airgun_1/Air_Gun-Wavefront OBJ.obj");
    // cat_1->load_data_from_file("cobblestones_1/cobblestones.obj");
    cat_1->load_data_from_file("cat_1/12221_Cat_v1_l3.obj");
    cat_1->set_position(glm::vec3(-1.0, 0.0, -4.0));
    cat_1->set_scale(1.0);
    cat_1->set_on_height_map(height_map);
    cat_1->set_rotation(glm::vec3(90.0, -180.0, 150.0));

    moon_1->load_data_from_file("moon_1/Moon 2K.obj");
    moon_1->set_position(glm::vec3(-2.0, 0.0, -8.0));
    moon_1->set_scale(2.0);
    moon_1->set_on_height_map(height_map);
    moon_1->inc_position(glm::vec3(0.0, 2.0, 0.0));

    plant_1->load_data_from_file("plant_1/01Alocasia_obj.obj");
    plant_1->set_position(glm::vec3(1.0, 0.0, -4.0));
    plant_1->set_scale(1.5);
    plant_1->set_on_height_map(height_map);

    nanosuit_1->load_data_from_file("nano_suit/Nanosuit.obj");
    nanosuit_1->set_position(glm::vec3(0.0, 0.0, -5.0));
    nanosuit_1->set_scale(2.0);
    nanosuit_1->set_on_height_map(height_map);
    nanosuit_1->set_rotation(glm::vec3(90.0, -180.0, 0.0));

    MaterialLoader::load_materials({
        "leather_1",
        "metal_1",
        "alien_1",
        "metal_6",
        "simple_gold_1",
    });

    // moon_1->set_material("metal_1");
    // nanosuit_1->meshes[0].material = MaterialLoader::get_material("leather_1");
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

    for (auto hp : scene->height_map_items)
    {
        hp->height_map_mesh->material = Material::materials[std::rand() % Material::materials.size()];
    }
}

void shuffle_direction()
{
    int i = std::rand() % scene->scenario_items.size();
    ScenarioItem *si = scene->scenario_items[i];
    scene->camera.point_to(si);
    scene->camera.update_view_matrix();
}

void change_height_map()
{
    height_map->diamond_square();
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

#ifdef DEBUG_MODE_COMPILE
    engine.render.imgui_controller->observef("fps", &engine.render.fps);

    engine.render.imgui_controller->observef("L0.ambient", &light_ambient_debug, 0.0, 10.0);
    engine.render.imgui_controller->observef("L0.strength", &light_strength_debug, 0.0, 20.0);
    engine.render.imgui_controller->observef("L0.R", &color_light_debug[0], 0.0, 1.0);
    engine.render.imgui_controller->observef("L0.G", &color_light_debug[1], 0.0, 1.0);
    engine.render.imgui_controller->observef("L0.B", &color_light_debug[2], 0.0, 1.0);

    engine.render.imgui_controller->observef("x", &light_translation_debug[0], -5.0, 5.0);
    engine.render.imgui_controller->observef("y", &light_translation_debug[1], -5.0, 5.0);
    engine.render.imgui_controller->observef("z", &light_translation_debug[2], -10.0, 3.0);

    engine.render.imgui_controller->observef("noise", &noise_level_debug, 0.0, 1.0);
    engine.render.imgui_controller->observef("blur", &blur_level_debug, 0.0, 10.0);

    engine.render.imgui_controller->radio("FrameBuffer", &frame_buffer_select, 3);
    engine.render.imgui_controller->button("Shuffle materials", shuffle_materials);
    engine.render.imgui_controller->button("point to objects", shuffle_direction);
    engine.render.imgui_controller->button("change heightmap", change_height_map);

#endif

    engine.set_keyboard_handler(keyboard_handler);
    engine.set_mouse_handler(mouse_handler);
    engine.set_preload(preload);
    engine.set_render_handler(render_handler);
    engine.render.set_size(1024, 768);
    engine.start();

    return 0;
}