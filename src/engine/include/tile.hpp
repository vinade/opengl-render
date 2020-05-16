#ifndef TILE_HPP
#define TILE_HPP

#include "tile_mesh.hpp"
#include "scenario_item.hpp"

class Tile : public ScenarioItem
{
private:
    bool loaded = false;

public:
    glm::vec3 tile_position = {0.0, 0.0, 0.0};
    glm::vec3 tile_size = {1.0, 1.0, 1.0};

    TileMesh *tile_mesh;

    Tile(){};
    void set();
    void set(Texture *texture_obj);
    void set(const std::string &texture_path);

    void draw();

    void set_size_in_percent(float width, float height);
    void set_size_in_pixels(float width, float height);

    void set_position_in_percent(float x, float y);
    void set_position_in_pixels(float x, float y);

    void center_y();
    void center_x();

    void update_model_matrix();
};

#endif
