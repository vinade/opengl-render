#ifndef DEPTH_SAMPLER_HPP
#define DEPTH_SAMPLER_HPP

#include "light.hpp"
#include "height_map.hpp"
#include "scenario_item.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "post_process.hpp"

#define DEPTH_SAMPLER_WIDTH 320
#define DEPTH_SAMPLER_HEIGHT 200

namespace depth
{
    class Sampler;

    struct sampler_item
    {
        ScenarioItem *obj;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
}; // namespace depth

class depth::Sampler
{
private:
    depth::sampler_item *objs = nullptr; // referência aos meshes criados pelos ScenarioItems para facilitar o shuffle
    ScenarioItem **models = nullptr;     // Objetos da cena

    int models_size = 0;
    int sample_size = 0;

public:
    Engine engine;

    Scene *scene;
    HeightMap *height_map;
    Light *lights[4] = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    GaussianNoise *pp_gaussian_noise = nullptr;
    GaussianBlur *pp_gaussian_blur = nullptr;

    static Sampler *context;
    static std::vector<std::string> object_names;
    static std::vector<std::string> material_names;

    Sampler(int sample_size);

    static void render_handler();
    static void preload_handler();
};

#endif