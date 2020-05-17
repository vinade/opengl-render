#ifndef SCENE_HPP
#define SCENE_HPP

#include "camera.hpp"
#include "perspective.hpp"
#include "scenario_item.hpp"
#include "skybox_mesh.hpp"
#include "light.hpp"
#include "tile.hpp"
#include "shader.hpp"
#include "frame_buffer.hpp"
#include <vector>
#include <algorithm>

#define SCENE_MAX_LIGHTS 4

class RenderWindow; // resolvendo a dependência circular

class Scene
{

private:
    std::vector<Tile *> tiles;
    std::vector<Light *> lights;
    std::vector<ScenarioItem *> scenario_items;
    SkyboxMesh *skybox = nullptr;

    void draw_on_buffer(FrameBuffer *fbo);
    void setup_light(int i);
    // std::vector<PostProcess*> post_processes;

    // FrameBufferObject* destination; // ou o unit id do fbo

public:
    Camera camera;
    Perspective perspective;
    Shader *ambient_shader;

    void init();
    void init(bool init_lights);

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

    inline unsigned int count_lights() { return this->lights.size(); };
    inline unsigned int count_scenario_items() { return this->scenario_items.size(); };

    void draw();
    void draw(FrameBuffer *fbo);
    void draw_tiles();
    void update_color_buffer(RenderWindow *render);
    void update_depth_buffer(RenderWindow *render);
    void setup();
    void setup_lights();
};

#endif
