#ifndef DEPTH_SAMPLER_HPP
#define DEPTH_SAMPLER_HPP

#include "light.hpp"
#include "height_map.hpp"
#include "scenario_item.hpp"
#include "scene.hpp"
#include "engine.hpp"
#include "post_process.hpp"

#define DEPTH_SAMPLER_WIDTH 320
#define DEPTH_SAMPLER_HEIGHT 240
#define DEPTH_MODE_SHARED_LIB 0
#define DEPTH_MODE_DEBUG 1

#include <mutex>

typedef struct image_data
{
    int width;
    int height;
    char channels;
    float *data;
} ImageData;

typedef struct struct_sample
{
    int n;
    char creating;
    char request;
    ImageData *content;
} SampleContent;

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
    int mode = DEPTH_MODE_DEBUG;
    FrameBuffer *fbo_color_left = nullptr;
    FrameBuffer *fbo_color_right = nullptr;
    FrameBuffer *fbo_depth = nullptr;
    SampleContent *sc = nullptr;
    ImageData *images_buffer = nullptr;
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
    Sampler(SampleContent *sc, int mode);
    void init(int sample_size, int mode);

    static void render_handler();
    static void preload_handler();
    static ImageData *create_content();
    static void create_scene(FrameBuffer *fbo, const glm::vec3 &position, float *image_data);
    static std::mutex mtx_sc;
};

#endif