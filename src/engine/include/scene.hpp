#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "perspective.hpp"
#include "scenario_item.hpp"
#include "skybox_mesh.hpp"
#include "light.hpp"
#include "tile.hpp"
#include "shader.hpp"
#include <vector>
#include <algorithm>

#define SCENE_MAX_LIGHTS 4

class Scene
{

private:
    std::vector<Tile *> tiles;
    std::vector<Light *> lights;
    std::vector<ScenarioItem *> scenario_items;
    SkyboxMesh *skybox = nullptr;

    // std::vector<PostProcess*> post_processes;

    // FrameBufferObject* destination; // ou o unit id do fbo

public:
    Camera camera;
    Perspective perspective;
    Shader *ambient_shader;

    void init();

    void add(Tile *tile);
    void add(Tile &tile);

    void add(Light *light);
    void add(Light &light);
    // void remove_light(Light *light);

    void add(ScenarioItem *scenario_item);
    void add(ScenarioItem &scenario_item);
    // void remove_scenario_item(ScenarioItem *scenario_item);

    // void add_post_process(PostProcess *post_process);
    // void add_post_process(PostProcess &post_process);
    // void remove_post_process(PostProcess *post_process);

    // void add_framebuffer(FrameBufferObject *fbo);
    // void add_framebuffer(FrameBufferObject &fbo);
    // void remove_framebuffer(FrameBufferObject *fbo);

    inline unsigned int count_lights() { return this->lights.size(); };
    inline unsigned int count_scenario_items() { return this->scenario_items.size(); };

    void draw();
    void draw_tiles();

    template <typename T>
    static bool add_once(std::vector<T> &item_vector, const T &item);

    template <typename T>
    static bool has_item(const std::vector<T> &item_vector, const T &item);
};

template <typename T>
bool Scene::has_item(const std::vector<T> &item_vector, const T &item)
{
    auto it = std::find(item_vector.begin(), item_vector.end(), item);
    return (it != item_vector.end());
}

template <typename T>
bool Scene::add_once(std::vector<T> &item_vector, const T &item)
{
    if (Scene::has_item(item_vector, item))
    {
        return false;
    }

    item_vector.push_back(item);
    return true;
}

#endif
