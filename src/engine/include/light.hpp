#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "debug_mesh.hpp"
#include "perspective.hpp"
#include "camera.hpp"
#include "scenario_item.hpp"

enum LightType
{
    LIGHT_DIFFUSE
};

class Light : public ScenarioItem
{
private:
    bool loaded = false;

public:
    LightType type;
    float ambient = 0.1;
    glm::vec4 color = {1.0, 0.8, 0.3, 1.0};
    DebugMesh *debug_mesh;

    Light(){};
    void set();
    void set(const glm::vec4 &color);
    void set(const glm::vec4 &color, LightType type);

    void draw(){};
    void draw(const Camera &camera, const Perspective &perspective);

    void set_color(const glm::vec4 &color);
    inline void set_ambient(float ambient) { this->ambient = ambient; };
};

#endif
