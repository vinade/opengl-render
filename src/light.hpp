#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "debug_mesh.cpp"
#include "scene_item.cpp"

class Light : public SceneItem
{
private:
    bool loaded = false;

public:
    glm::vec4 color;
    DebugMesh *debug_mesh;

    Light(){};
    void set();
    void set(const glm::vec4 &color);

    void draw();
};

#endif
