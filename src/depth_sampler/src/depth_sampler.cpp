#ifndef DEPTH_SAMPLER_CPP
#define DEPTH_SAMPLER_CPP

#include "material_loader.hpp"
#include "depth_sampler.hpp"

void depth::Sampler::render_handler()
{
    depth::Sampler::context->scene->draw();
}

void depth::Sampler::preload_handler()
{
    depth::Sampler *context = depth::Sampler::context;

    Light *light_0 = new Light();
    context->scene = new Scene();

    context->scene->camera.set_position(glm::vec3(0.0, 0.0, 0.0));
    context->scene->camera.update_view_matrix();

    context->pp_gaussian_noise = new GaussianNoise("gaussian_noise.post");
    context->scene->add(context->pp_gaussian_noise);
    context->pp_gaussian_noise->set_level(0.5);

    context->scene->add(light_0);
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
#endif