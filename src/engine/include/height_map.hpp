#ifndef HEIGHT_MAP_HPP
#define HEIGHT_MAP_HPP

#include "height_map_mesh.hpp"
#include "scenario_item.hpp"

class HeightMap : public ScenarioItem
{
private:
    float texture_size = 1.0;

public:
    HeightMapMesh *height_map_mesh;
    HeightMap();
    HeightMap(const std::string &file_path);
    HeightMap(float *height_map_data, int width, int height);
    ~HeightMap();

    void draw(Shader *shader);
    void set_material(const std::string material_name);
    void set_material(Material *mtl);
    inline void set_texture_size(float texture_size) { this->texture_size = texture_size; };

    /* Temporário */
    static float debug_data[5][5];
    static int debug_width;
    static int debug_height;
};

#endif
