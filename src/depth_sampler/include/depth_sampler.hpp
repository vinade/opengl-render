#ifndef DEPTH_SAMPLER_HPP
#define DEPTH_SAMPLER_HPP

#include "light.hpp"
#include "height_map.hpp"
#include "scenario_item.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "post_process.hpp"

// #define DEPTH_SAMPLER_WIDTH 320
// #define DEPTH_SAMPLER_HEIGHT 200
#define DEPTH_SAMPLER_WIDTH 1024
#define DEPTH_SAMPLER_HEIGHT 768

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
    depth::sampler_item *objs = nullptr;

    int models_size = 0;

public:
    int sample_size = 0;
    std::vector<ScenarioItem *> models; // referência aos meshes criados pelos ScenarioItems para facilitar o shuffle
    std::vector<SkyboxMesh *> skyboxes; // referência aos skyboxes possíveis para o shuffle
    Engine engine;

    Scene *scene;

    GaussianNoise *pp_gaussian_noise = nullptr;
    GaussianBlur *pp_gaussian_blur = nullptr;

    static Sampler *context;
    static std::vector<std::string> object_names;
    static std::vector<std::string> material_names;
    static std::vector<std::string> skyboxes_names;

    Sampler(int sample_size);

    static void render_handler();
    static void preload_handler();
};

#endif