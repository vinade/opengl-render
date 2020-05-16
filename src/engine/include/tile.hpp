#ifndef TILE_HPP
#define TILE_HPP

#include "tile_mesh.hpp"
#include "scenario_item.hpp"

class Tile : public ScenarioItem
{
private:
    bool loaded = false;

public:
    TileMesh *tile_mesh;

    Tile(){};
    void set();
    void set(Texture *texture_obj);
    void set(const std::string &texture_path);

    void draw();
};

#endif
