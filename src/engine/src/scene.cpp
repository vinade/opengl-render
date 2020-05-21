#ifndef SCENE_CPP
#define SCENE_CPP

#include "scene.hpp"
#include <thread>
#include "render_window.hpp"
#include "app_utils.hpp"

void Scene::init()
{
    this->init(true);
}

void Scene::init(bool init_lights)
{
    if (!RenderWindow::is_render_thread())
    {
        RenderWindow::context->to_setup(this);
        return;
    }

    this->ambient_shader = Shader::get_shader("ambient.std", SHADER_TYPE_SCENE);

    if (this->ambient_shader->use_materials)
    {
        this->ambient_shader->setup("u_Textures.skybox", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.diffuse", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.normal", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.ambient_occlusion", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.metallic", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.roughness", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Textures.reflection", DATA_TYPE_INT);

        this->ambient_shader->setup("u_Material.color", DATA_TYPE_VEC4);
        this->ambient_shader->setup("u_Material.metallic", DATA_TYPE_FLOAT);
        this->ambient_shader->setup("u_Material.roughness", DATA_TYPE_FLOAT);
        this->ambient_shader->setup("u_Material.reflection", DATA_TYPE_FLOAT);

        this->ambient_shader->setup("u_Material.diffuse_texture_flag", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Material.normal_texture_flag", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Material.ambient_occlusion_texture_flag", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Material.metallic_texture_flag", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Material.roughness_texture_flag", DATA_TYPE_INT);
        this->ambient_shader->setup("u_Material.reflection_texture_flag", DATA_TYPE_INT);
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

        if ((this->skybox->light == nullptr) && (this->lights.size()))
        {
            this->skybox->light = this->lights[0];
        }
    }

    if (init_lights)
    {
        this->setup_lights();
    }
}

void Scene::add(Light *light)
{

    if (this->skybox != nullptr)
    {
        if (this->skybox->light == nullptr)
        {
            this->skybox->light = light;
        }
    }

    if (this->lights.size() > SCENE_MAX_LIGHTS)
    {
        std::cerr << "[Scene] Atingido o número máximo de luzes. Ignorando novas entradas." << std::endl;
        return;
    }

    if (AppUtils::add_once(this->lights, light))
    {
        if (!RenderWindow::is_render_thread())
        {
            RenderWindow::context->to_setup(this);
            return;
        }

        if (!this->ambient_shader->use_lights)
        {
            std::cerr << "[Scene] Ambient shader deveria ter use_lights == true." << std::endl;
            return;
        }

        int i = this->lights.size() - 1;
        this->setup_light(i);
    }
}

void Scene::add(Light &light)
{
    Scene::add((Light *)&light);
}

void Scene::setup_lights()
{

    if (!this->ambient_shader->use_lights)
    {
        std::cerr << "[Scene] Ambient shader deveria ter use_lights == true." << std::endl;
        return;
    }

    if (this->lights.size())
    {
        this->ambient_shader->setup("u_Camera", DATA_TYPE_VEC3);
    }

    int lights_count = this->lights.size();
    for (int i = 0; i < lights_count; i++)
    {
        this->setup_light(i);
    }
}

void Scene::setup_light(int i)
{
    std::string prefix = "u_Lights[" + std::to_string(i) + "].";

    std::string light_type = prefix + "type";
    std::string light_ambient = prefix + "ambient";
    std::string light_color = prefix + "color";
    std::string light_position = prefix + "position";

    this->ambient_shader->setup(light_ambient, DATA_TYPE_FLOAT);
    this->ambient_shader->setup(light_type, DATA_TYPE_INT);
    this->ambient_shader->setup(light_color, DATA_TYPE_VEC4);
    this->ambient_shader->setup(light_position, DATA_TYPE_VEC3);
}

void Scene::add(ScenarioItem *scenario_item)
{
    AppUtils::add_once(this->scenario_items, scenario_item);
}

void Scene::add(ScenarioItem &scenario_item)
{
    AppUtils::add_once(this->scenario_items, (ScenarioItem *)&scenario_item);
}

void Scene::add(Tile &tile)
{
    Scene::add((Tile *)&tile);
}

void Scene::add(Tile *tile)
{
    AppUtils::add_once(this->tiles, tile);
}

void Scene::update_color_buffer(RenderWindow *render)
{
    this->draw_on_buffer(render->fbo_color);
}

void Scene::update_depth_buffer(RenderWindow *render)
{
    this->draw_on_buffer(render->fbo_depth);
}

void Scene::draw_on_buffer(FrameBuffer *fbo)
{

    if (fbo == nullptr)
    {
        std::cerr << "[Scene] draw_on_buffer() em um ponteiro nulo." << std::endl;
        exit(1);
    }

    if (fbo->data == nullptr)
    {
        fbo->set();
    }

    this->draw(fbo);
}

void Scene::draw()
{
    this->draw(nullptr);
}

void Scene::draw(FrameBuffer *fbo)
{

    /*
        Bind FrameBuffer
    */
    if (fbo == nullptr)
    {
        FrameBuffer::unbind();
    }
    else
    {
        fbo->bind();
    }

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
        this->ambient_shader->fill("u_Textures.skybox", 0);
        this->ambient_shader->fill("u_Textures.diffuse", Texture::get_type_slot(TEXTURE_DIFFUSE));
        this->ambient_shader->fill("u_Textures.normal", Texture::get_type_slot(TEXTURE_NORMAL));
        this->ambient_shader->fill("u_Textures.metallic", Texture::get_type_slot(TEXTURE_METALLIC));
        this->ambient_shader->fill("u_Textures.roughness", Texture::get_type_slot(TEXTURE_ROUGHNESS));
        this->ambient_shader->fill("u_Textures.ambient_occlusion", Texture::get_type_slot(TEXTURE_AMBIENT_OCLUSION));
        this->ambient_shader->fill("u_Textures.reflection", Texture::get_type_slot(TEXTURE_REFLECTION));
    }

    if (this->ambient_shader->use_mvp)
    {
        this->ambient_shader->fill("u_View", this->camera.view_matrix);
        this->ambient_shader->fill("u_Projection", this->perspective.projection_matrix);
    }

    if (this->ambient_shader->use_lights)
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
    if (DEBUG_MODE && (fbo == nullptr))
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

void Scene::draw_tiles()
{
    for (auto tile : this->tiles)
    {
        tile->draw();
    }
}

void Scene::setup()
{
    this->init(true);
}

#endif
