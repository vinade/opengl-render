#ifndef HEIGHT_MAP_HPP
#define HEIGHT_MAP_HPP

#include "height_map_mesh.hpp"
#include "scenario_item.hpp"

#define HEIGHT_MAP_DEFAULT_WIDTH 15
#define HEIGHT_MAP_DEFAULT_HEIGHT 15
#define HEIGHT_MAP_DEFAULT_MATERIAL "terrain_1"

class HeightMap : public ScenarioItem
{
private:
    int repeat_number = 1;

public:
    HeightMapMesh *height_map_mesh;
    HeightMap();
    HeightMap(const std::string &file_path);
    HeightMap(float *height_map_data, int width, int height);
    HeightMap(int width, int height);
    ~HeightMap();

    void load(const std::string &file_path);
    void draw(Shader *shader);
    void set_material(const std::string material_name);
    void set_material(Material *mtl);
    void diamond_square();
    void diamond_square(int width, int height);
    float get_height(float x, float y);
    inline void set_repeat_number(float repeat_number) { this->repeat_number = repeat_number; };
};

#endif
