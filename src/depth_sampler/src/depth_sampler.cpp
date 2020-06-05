#ifndef DEPTH_SAMPLER_CPP
#define DEPTH_SAMPLER_CPP

#include "material_loader.hpp"
#include "depth_sampler.hpp"
#include "shuffler.hpp"
#include <unistd.h>

void depth::Sampler::render_handler()
{
    depth::Sampler *context = depth::Sampler::context;
    depth::shuffle(context);
    context->scene->draw();

    sleep(1);
}

void depth::Sampler::preload_handler()
{
    depth::Sampler *context = depth::Sampler::context;

    context->scene = new Scene();
    context->scene->use_skybox = true;

    context->pp_gaussian_noise = new GaussianNoise("gaussian_noise.post");
    context->pp_gaussian_noise->set_level(0.5);
    context->pp_gaussian_blur = new GaussianBlur("gaussian_blur.post");
    context->pp_gaussian_blur->set_range(3);

    context->scene->add(new Light());
    context->scene->add(new Light());
    context->scene->add(new Light());
    context->scene->add(new Light());

    context->scene->add(context->pp_gaussian_noise);
    context->scene->add(context->pp_gaussian_blur);

    context->scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    context->scene->camera.update_view_matrix();

    /*
        load dos materiais
    */
    MaterialLoader::load_materials(depth::Sampler::material_names);

    /*
        load das models
    */
    for (std::string object_name : depth::Sampler::object_names)
    {
        ScenarioItem *si = new ScenarioItem();
        context->models.push_back(si);
        si->load_data_from_file(object_name);
    }

    /*
        load dos skyboxes
    */
    for (std::string skybox_name : depth::Sampler::skyboxes_names)
    {
        SkyboxMesh *skybox = new SkyboxMesh(skybox_name);
        skybox->light = context->scene->lights[0];
        skybox->set_scale(4000.0);
        context->skyboxes.push_back(skybox);
    }

    /* alocação de memória para os controladores de objetos */
    for (int i = 0; i < context->sample_size; i++)
    {
        context->scene->add(new Puppeteer());
    }

    /* cena completa... pronta para o setup */
    context->scene->setup();
}

depth::Sampler::Sampler(int sample_size)
{
    this->sample_size = sample_size;
    depth::Sampler::context = this;

#ifdef DEBUG_MODE_COMPILE
    this->engine.render.imgui_controller->observef("fps", &this->engine.render.fps);
#endif

    this->engine.set_render_handler(depth::Sampler::render_handler);
    this->engine.set_preload(depth::Sampler::preload_handler);
    this->engine.render.set_size(DEPTH_SAMPLER_WIDTH, DEPTH_SAMPLER_HEIGHT);
    this->engine.start();
}

depth::Sampler *depth::Sampler::context = nullptr;

std::vector<std::string> depth::Sampler::object_names = {
    "cat_1/12221_Cat_v1_l3.obj",
    "moon_1/Moon 2K.obj",
    "plant_1/01Alocasia_obj.obj",
    "nano_suit/Nanosuit.obj",
};

std::vector<std::string> depth::Sampler::material_names = {
    "leather_1",
    "metal_1",
    "alien_1",
    "metal_6",
    "simple_gold_1",
};

std::vector<std::string> depth::Sampler::skyboxes_names = {
    "sunset_1",
    "sky_1",
    "yokohama",
};

#endif