#ifndef HEIGHT_MAP_CPP
#define HEIGHT_MAP_CPP

#include "height_map.hpp"
#include "material_loader.hpp"

HeightMap::HeightMap()
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->from_float_array(&HeightMap::debug_data[0][0], HeightMap::debug_width, HeightMap::debug_height);
}

HeightMap::HeightMap(float *height_map_data, int width, int height)
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->from_float_array(height_map_data, width, height);
}

HeightMap::~HeightMap()
{
    delete this->height_map_mesh;
}

void HeightMap::set_material(const std::string material_name)
{
    Material *mtl = MaterialLoader::get_material(material_name);
    this->set_material(mtl);
}

void HeightMap::set_material(Material *mtl)
{
    this->height_map_mesh->material = mtl;
}

void HeightMap::draw(Shader *shader)
{
    this->update_model_matrix();
    shader->fill("u_Textures.size", this->texture_size);
    shader->fill("u_Model", this->model_matrix);
    this->height_map_mesh->draw(shader);
}

int HeightMap::debug_width = 5;
int HeightMap::debug_height = 5;
float HeightMap::debug_data[5][5] = {
    {1.0, 0.3, 0.4, 0.2, 0.0},
    {1.0, 0.3, 0.4, 0.2, 0.0},
    {1.0, 0.3, 0.4, 0.2, 0.0},
    {1.0, 0.3, 0.4, 0.2, 0.0},
    {1.0, 0.3, 0.4, 0.2, 0.0},
};

#endif
