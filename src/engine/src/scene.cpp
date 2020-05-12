#ifndef SCENE_CPP
#define SCENE_CPP

#include "scene.hpp"

void Scene::init()
{
    this->ambient_shader = Shader::get_shader("ambient.std", SHADER_TYPE_SCENE);

    if (this->ambient_shader->use_materials)
    {
        this->ambient_shader->setup("u_Texture", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Skybox", DATA_TYPE_INT);

        this->ambient_shader->setup("u_Material.diffuse_color", DATA_TYPE_VEC4);
        this->ambient_shader->setup("u_Material.ambient_color", DATA_TYPE_VEC4);
        this->ambient_shader->setup("u_Material.specular_color", DATA_TYPE_VEC4);
        this->ambient_shader->setup("u_Material.emission_color", DATA_TYPE_VEC4);
        this->ambient_shader->setup("u_Material.shininess", DATA_TYPE_FLOAT);
        this->ambient_shader->setup("u_Material.shininess_strength", DATA_TYPE_FLOAT);
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

    if (this->skybox == nullptr)
    {
        this->skybox = new SkyboxMesh();
        this->skybox->set_scale(5000.0);
        // this->skybox->set_position(glm::vec3(0.0f, -50.0f, -200.0f));
    }
}

void Scene::add_light(Light *light)
{

    if (this->lights.size() > SCENE_MAX_LIGHTS)
    {
        std::cerr << "[Scene] Atingido o número máximo de luzes. Ignorando novas entradas." << std::endl;
        return;
    }

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
        std::string light_ambient = prefix + "ambient";
        std::string light_color = prefix + "color";
        std::string light_position = prefix + "position";

        // std::string light_direction = prefix + "direction";  // TODO
        // std::string light_direction_angle = prefix + "angle";  // angulo de abertura da luz direcional

        this->ambient_shader->setup(light_ambient, DATA_TYPE_FLOAT);
        this->ambient_shader->setup(light_type, DATA_TYPE_INT);
        this->ambient_shader->setup(light_color, DATA_TYPE_VEC4);
        this->ambient_shader->setup(light_position, DATA_TYPE_VEC3);
        this->ambient_shader->setup("u_Camera", DATA_TYPE_VEC3);
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
    /*
        Skybox
    */
    this->skybox->set_position(this->camera.get_position());
    this->skybox->draw(this->camera.view_matrix, this->perspective.projection_matrix);

    /*
        Objetos
    */
    if (this->ambient_shader->use_materials)
    {
        this->ambient_shader->fill("u_Skybox", 0);  // temporario
        this->ambient_shader->fill("u_Texture", 1); // temporario
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

            std::string light_ambient = prefix + "ambient";
            std::string light_type = prefix + "type";
            std::string light_color = prefix + "color";
            std::string light_position = prefix + "position";

            this->ambient_shader->fill(light_ambient, this->lights[i]->ambient);
            this->ambient_shader->fill(light_type, this->lights[i]->type);
            this->ambient_shader->fill(light_color, this->lights[i]->color);
            this->ambient_shader->fill(light_position, this->lights[i]->get_position());
        }

        this->ambient_shader->fill("u_Camera", this->camera.get_position());
    }

    for (auto item : this->scenario_items)
    {

        if (this->ambient_shader->use_mvp)
        {
            this->ambient_shader->fill("u_Model", item->model_matrix);
        }

        item->draw(this->ambient_shader);
    }

    /*
        Objetos de debug
    */
    if (DEBUG_MODE)
    {
        /*
            Luzes
        */
        for (auto light : this->lights)
        {
            light->draw(this->camera, this->perspective);
        }
    }
}
#endif
