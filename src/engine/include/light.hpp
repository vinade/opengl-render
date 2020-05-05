#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "debug_mesh.hpp"
#include "scene_item.hpp"

enum LightType
{
    LIGHT_DIFFUSE
};

class Light : public SceneItem
{
private:
    bool loaded = false;

public:
    LightType m_type;
    glm::vec4 color;
    DebugMesh *debug_mesh;

    Light(){};
    void set();
    void set(const glm::vec4 &color);
    void set(const glm::vec4 &color, LightType type);

    void draw();
};

#endif
