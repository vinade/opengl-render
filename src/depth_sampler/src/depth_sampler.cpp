#ifndef DEPTH_SAMPLER_CPP
#define DEPTH_SAMPLER_CPP

#include "material_loader.hpp"
#include "depth_sampler.hpp"
#include <unistd.h>

void depth::Sampler::render_handler()
{
    depth::Sampler::context->scene->draw();
    sleep(10);
}

void depth::Sampler::preload_handler()
{
    depth::Sampler *context = depth::Sampler::context;

    context->scene = new Scene();

    context->lights[0] = new Light();
    context->lights[1] = new Light();
    context->lights[2] = new Light();
    context->lights[3] = new Light();
    context->scene->add(context->lights[0]);
    context->scene->add(context->lights[1]);
    context->scene->add(context->lights[2]);
    context->scene->add(context->lights[3]);

    context->scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    context->scene->camera.update_view_matrix();

    context->pp_gaussian_noise = new GaussianNoise("gaussian_noise.post");
    context->scene->add(context->pp_gaussian_noise);
    context->pp_gaussian_noise->set_level(0.5);

    /*
        load dos materiais
    */
    MaterialLoader::load_materials(depth::Sampler::material_names);

    /*
        load das models
    */
    context->models = (ScenarioItem **)malloc(sizeof(ScenarioItem *) * depth::Sampler::object_names.size());
    int model_i = 0;
    for (std::string object_name : depth::Sampler::object_names)
    {
        context->models[model_i] = new ScenarioItem();
        context->models[model_i]->load_data_from_file(object_name);
        model_i++;
    }
    context->models_size = model_i;

    /* alocação de memória para os controladores de objetos */
    for (int i = 0; i < context->sample_size; i++)
    {
        context->scene->puppeteers.push_back(new Puppeteer());
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

#endif