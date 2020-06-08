#ifndef SHUFFLER_CPP
#define SHUFFLER_CPP

#include "shuffler.hpp"
#include "depth_sampler.hpp"
#include <cstdlib>

inline float depth::random(float min_value, float max_value)
{
    float rand_value = float(std::rand() % 100000) / 100000.0;
    float range = max_value - min_value;
    return min_value + range * rand_value;
}

void depth::shuffle_color(glm::vec4 &color)
{
    float alpha = depth::random(0.0, 1.0);
    depth::shuffle_color(color, alpha);
}

void depth::shuffle_color(glm::vec4 &color, float alpha)
{
    float lum = depth::random(0.0, 1.0);
    color.r = lum * depth::random(0.0, 1.0);
    color.g = lum * depth::random(0.0, 1.0);
    color.b = lum * depth::random(0.0, 1.0);
    color.a = alpha;
}

void depth::shuffle_position(glm::vec3 &position)
{
    position.x = depth::random(-20.0, 20.0);
    position.y = depth::random(-20.0, 20.0);
    position.z = depth::random(-80.0, -4.0);
}

glm::vec3 depth::shuffle_position()
{
    glm::vec3 pos;
    depth::shuffle_position(pos);
    return pos;
}

void depth::shuffle_scale(glm::vec3 &scale)
{
    scale.x = depth::random(0.2, 10.0);
    scale.y = depth::random(0.2, 10.0);
    scale.z = depth::random(0.2, 10.0);
}

void depth::shuffle_rotation(glm::vec3 &rotation)
{
    rotation.x = depth::random(0.0, 360.0);
    rotation.y = depth::random(0.0, 360.0);
    rotation.z = depth::random(0.0, 360.0);
}

void depth::shuffle_skybox(Scene *scene, std::vector<SkyboxMesh *> &skyboxes)
{
    int i = std::rand() % skyboxes.size();
    scene->skybox = skyboxes[i];
    depth::shuffle_color(scene->skybox->color);
    depth::shuffle_rotation(scene->skybox->m_rotation);
}

void depth::shuffle_post(GaussianNoise *post)
{
    post->set_level(depth::random(0.0, 1.0));
}

void depth::shuffle_post(GaussianBlur *post)
{
    post->set_range(int(depth::random(0.0, 5.0)));
}

void depth::shuffle_light(Light *light)
{
    light->set_ambient(depth::random(0.0, 4.0));
    depth::shuffle_color(light->color, 1.0);
    light->set_position(depth::shuffle_position());
    light->set_strength(depth::random(0.0, 5.0));
}

void depth::shuffle_materials(std::vector<ScenarioItem *> &models)
{
    for (auto si : models)
    {
        for (auto &m : si->meshes)
        {
            m.material = Material::materials[std::rand() % Material::materials.size()];
        }
    }
}

void depth::shuffle_height_map(HeightMap *height_map)
{
    height_map->diamond_square();
    glm::vec3 scale;

    depth::shuffle_scale(scale);
    scale.x = scale.x * 5.0;
    scale.z = scale.z * 5.0;
    height_map->set_scale(scale);
    height_map->update_model_matrix();
}

void depth::shuffle_puppeteer(Puppeteer *puppeteer, std::vector<ScenarioItem *> &models)
{
    int i = std::rand() % models.size();

    depth::shuffle_position(puppeteer->position);
    depth::shuffle_scale(puppeteer->scale);
    depth::shuffle_rotation(puppeteer->rotation);

    puppeteer->obj = models[i];
}

void depth::shuffle_all_puppeters(depth::Sampler *sampler_context)
{

    int visible_objs = 10 + (std::rand() % (sampler_context->sample_size - 10));
    int i = 0;
    for (auto puppeteer : sampler_context->scene->puppeteers)
    {
        if (i < visible_objs)
        {
            depth::shuffle_puppeteer(puppeteer, sampler_context->models);
        }
        else
        {
            puppeteer->obj = nullptr;
        }
        i++;
    }
}

void depth::shuffle(depth::Sampler *sampler_context)
{
    //depth::shuffle_camera(sampler_context->scene->camera); // TODO (mudança do up, apenas)
    depth::shuffle_skybox(sampler_context->scene, sampler_context->skyboxes);
    depth::shuffle_post(sampler_context->pp_gaussian_noise);
    depth::shuffle_post(sampler_context->pp_gaussian_blur);
    depth::shuffle_all_puppeters(sampler_context);

    for (auto height_map : sampler_context->scene->height_map_items)
    {
        std::cerr << "HMap Shuffle" << std::endl;
        exit(1);
        depth::shuffle_height_map(height_map);
    }

    for (auto light : sampler_context->scene->lights)
    {
        depth::shuffle_light(light);
    }

    depth::shuffle_materials(sampler_context->models);
}
#endif