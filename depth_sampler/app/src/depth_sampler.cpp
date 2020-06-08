#ifndef DEPTH_SAMPLER_CPP
#define DEPTH_SAMPLER_CPP

#include "material_loader.hpp"
#include "depth_sampler.hpp"
#include "shuffler.hpp"
#include <unistd.h>

void depth::Sampler::create_scene(FrameBuffer *fbo, const glm::vec3 &position, float *image_data)
{
    depth::Sampler *context = depth::Sampler::context;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    context->scene->camera.set_position(position);
    context->scene->camera.update_view_matrix();
    context->scene->draw(fbo);

    fbo->update_data(image_data);
}

ImageData *depth::Sampler::create_content()
{
    depth::Sampler *context = depth::Sampler::context;

    for (int i = 0; i < context->sample_size; i++)
    {
        int j = i * 3;
        depth::shuffle(context);

        depth::Sampler::create_scene(context->fbo_color_left, glm::vec3(-1.0, 0.0, 0.0), context->images_buffer[j].data);
        depth::Sampler::create_scene(context->fbo_depth, glm::vec3(0.0, 0.0, 0.0), context->images_buffer[j + 1].data);
        depth::Sampler::create_scene(context->fbo_color_right, glm::vec3(1.0, 0.0, 0.0), context->images_buffer[j + 2].data);
    }

    return context->images_buffer;
};

void depth::Sampler::render_handler()
{
    int waiting = 1;
    depth::Sampler *context = depth::Sampler::context;

    switch (context->mode)
    {
    case DEPTH_MODE_DEBUG:
        depth::shuffle(context);
        context->scene->draw();
        sleep(5);
        break;
    case DEPTH_MODE_SHARED_LIB:
        std::cout << "[DepthSampler] Esperando request..." << std::endl;
        while (waiting)
        {
            sleep(1);

            depth::Sampler::mtx_sc.lock();
            if (context->sc->request)
            {
                depth::Sampler::mtx_sc.unlock();

                context->sc->content = depth::Sampler::create_content();

                depth::Sampler::mtx_sc.lock();
                context->sc->creating = 0;
                depth::Sampler::mtx_sc.unlock();
                waiting = 0;
            }
            else
            {
                depth::Sampler::mtx_sc.unlock();
            }
        }
        break;
    }
}

void depth::Sampler::preload_handler()
{
    depth::Sampler *context = depth::Sampler::context;

    context->fbo_color_left->set();
    context->fbo_color_right->set();
    context->fbo_depth->set();

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
    this->init(sample_size, DEPTH_MODE_DEBUG);
}

depth::Sampler::Sampler(SampleContent *sc, int mode)
{
    this->sc = sc;
    this->init(sc->n, mode);
}

void depth::Sampler::init(int sample_size, int mode)
{
    this->sample_size = sample_size;
    this->mode = mode;
    this->fbo_color_left = new FrameBuffer();
    this->fbo_color_right = new FrameBuffer();
    this->fbo_depth = new FrameBuffer(FRAME_BUFFER_DEPTH_WITH_CORRECTION);

    depth::Sampler::context = this;

    // Aloca memória para a transferência
    int total_images = sample_size * 3;
    this->images_buffer = (ImageData *)malloc(sizeof(ImageData) * (total_images * 3));
    for (int i = 0; i < total_images * 3; i++)
    {
        images_buffer[i].channels = i % 3 != 1 ? 3 : 1;
        images_buffer[i].width = DEPTH_SAMPLER_WIDTH;
        images_buffer[i].height = DEPTH_SAMPLER_HEIGHT;
        images_buffer[i].data = (float *)malloc(DEPTH_SAMPLER_WIDTH * DEPTH_SAMPLER_HEIGHT * sizeof(float) * images_buffer[i].channels);
    }

#ifdef DEBUG_MODE_COMPILE
    this->engine.render.imgui_controller->observef("fps", &this->engine.render.fps);
#endif

    // inicia render
    this->engine.set_render_handler(depth::Sampler::render_handler);
    this->engine.set_preload(depth::Sampler::preload_handler);
    this->engine.render.set_size(DEPTH_SAMPLER_WIDTH, DEPTH_SAMPLER_HEIGHT);
    this->engine.start();
}

depth::Sampler *depth::Sampler::context = nullptr;
std::mutex depth::Sampler::mtx_sc;

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