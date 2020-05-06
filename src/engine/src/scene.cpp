#ifndef SCENE_CPP
#define SCENE_CPP

#include "scene.hpp"

void Scene::init()
{
    this->ambient_shader = Shader::get_shader("std", SHADER_TYPE_SCENE);

    if (this->ambient_shader->use_materials)
    {
        this->ambient_shader->setup("u_Texture", DATA_TYPE_INT);
    }
    else
    {
        std::cerr << "[Scene] Ambient shader deveria ter use_materials == true." << std::endl;
        return;
    }

    if (this->ambient_shader->use_mvp)
    {
        this->ambient_shader->setup("u_Model", DATA_TYPE_MAT4);
        this->ambient_shader->setup("u_View", DATA_TYPE_MAT4);
        this->ambient_shader->setup("u_Projection", DATA_TYPE_MAT4);
    }
    else
    {
        std::cerr << "[Scene] Ambient shader deveria ter use_mvp == true." << std::endl;
        return;
    }
}

void Scene::add_light(Light *light)
{
    if (Scene::add_once(this->lights, light))
    {
        if (!this->ambient_shader->use_ligths)
        {
            std::cerr << "[Scene] Ambient shader deveria ter use_lights == true." << std::endl;
            return;
        }

        unsigned int i = this->lights.size() - 1;
        std::string prefix = "u_Lights[" + std::to_string(i) + "].";

        std::string light_type = prefix + "type";
        std::string light_color = prefix + "color";
        std::string light_position = prefix + "position";
        // std::string light_direction = prefix + "direction";  // TODO
        // std::string light_direction_angle = prefix + "angle";  // angulo de abertura da luz direcional

        this->ambient_shader->setup(light_type, DATA_TYPE_INT);
        this->ambient_shader->setup(light_color, DATA_TYPE_VEC4);
        this->ambient_shader->setup(light_position, DATA_TYPE_VEC3);
    }
}

void Scene::add_light(Light &light)
{
    Scene::add_light((Light *)&light);
}

void Scene::add_scenario_item(ScenarioItem *scenario_item)
{
    Scene::add_once(this->scenario_items, scenario_item);
}

void Scene::add_scenario_item(ScenarioItem &scenario_item)
{
    Scene::add_once(this->scenario_items, (ScenarioItem *)&scenario_item);
}

void Scene::draw()
{

    if (this->ambient_shader->use_materials)
    {
        this->ambient_shader->fill("u_Texture", 0); // temporario
    }

    if (this->ambient_shader->use_mvp)
    {
        this->ambient_shader->fill("u_View", this->camera.view_matrix);
        this->ambient_shader->fill("u_Projection", this->perspective.projection_matrix);
    }

    if (this->ambient_shader->use_ligths)
    {
        unsigned int light_size = this->lights.size();
        for (unsigned int i = 0; i < light_size; i++)
        {
            std::string prefix = "u_Lights[" + std::to_string(i) + "].";

            std::string light_type = prefix + "type";
            std::string light_color = prefix + "color";
            std::string light_position = prefix + "position";

            this->ambient_shader->fill(light_type, this->lights[0]->type);
            this->ambient_shader->fill(light_color, this->lights[0]->color);
            this->ambient_shader->fill(light_position, this->lights[0]->get_position());
        }
    }

    for (auto item : this->scenario_items)
    {

        if (this->ambient_shader->use_mvp)
        {
            this->ambient_shader->fill("u_Model", item->model_matrix);
        }

        this->ambient_shader->exec();
        item->draw();
        // exit(1);
    }

    if (DEBUG_MODE)
    {
        for (auto light : this->lights)
        {
            light->draw(this->camera, this->perspective);
        }
    }
}
#endif
