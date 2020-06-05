#ifndef SHUFFLER_HPP
#define SHUFFLER_HPP

#include "light.hpp"
#include "height_map.hpp"
#include "scenario_item.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "post_process.hpp"

namespace depth
{
    class Sampler;

    inline float random(float min_value, float max_value);

    void shuffle_color(glm::vec4 &color);
    void shuffle_color(glm::vec4 &color, float alpha);
    void shuffle_position(glm::vec3 &position);
    void shuffle_scale(glm::vec3 &scale);
    void shuffle_rotation(glm::vec3 &rotation);
    glm::vec3 shuffle_position();

    void shuffle_skybox(Scene *scene, std::vector<SkyboxMesh *> &skyboxes);
    void shuffle_height_map(HeightMap *height_map);
    void shuffle_puppeteer(Puppeteer *puppeteer, std::vector<ScenarioItem *> &models);
    void shuffle_materials(std::vector<ScenarioItem *> &models);
    void shuffle_light(Light *light);
    void shuffle_camera(Camera &camera);
    void shuffle_post(GaussianNoise *post);
    void shuffle_post(GaussianBlur *post);
    void shuffle_all_puppeters(depth::Sampler *sampler_context);

    void shuffle(depth::Sampler *sampler_context);
}; // namespace depth

#endif