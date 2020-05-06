#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "debug_mesh.hpp"
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
    glm::vec4 color;
    DebugMesh *debug_mesh;

    Light(){};
    void set();
    void set(const glm::vec4 &color);
    void set(const glm::vec4 &color, LightType type);

    void draw(){};
    void draw(const Camera &camera, const Perspective &perspective);
};

#endif
