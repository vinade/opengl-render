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
};

class depth::Sampler
{
private:
    Mesh *meshes = nullptr;         // referência aos meshes criados pelos ScenarioItems para facilitar o shuffle
    ScenarioItem **items = nullptr; // Objetos da cena
    // Material *materials = nullptr;  // Objetos da cena
    int items_size = 0;
    // int materials_size = 0;

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