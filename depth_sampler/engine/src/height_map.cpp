#ifndef HEIGHT_MAP_CPP
#define HEIGHT_MAP_CPP

#include "height_map.hpp"
#include "material_loader.hpp"

HeightMap::HeightMap()
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->diamond_square(HEIGHT_MAP_DEFAULT_WIDTH, HEIGHT_MAP_DEFAULT_WIDTH);
    this->set_material(HEIGHT_MAP_DEFAULT_MATERIAL);
}

HeightMap::HeightMap(const std::string &file_path)
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->load(file_path);
    this->set_material(HEIGHT_MAP_DEFAULT_MATERIAL);
}

HeightMap::HeightMap(float *height_map_data, int width, int height)
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->from_float_array(height_map_data, width, height);
    this->set_material(HEIGHT_MAP_DEFAULT_MATERIAL);
}

HeightMap::HeightMap(int width, int height)
{
    this->height_map_mesh = new HeightMapMesh();
    this->height_map_mesh->diamond_square(width, height);
    this->set_material(HEIGHT_MAP_DEFAULT_MATERIAL);
}

HeightMap::~HeightMap()
{
    delete this->height_map_mesh;
}

void HeightMap::diamond_square()
{
    this->height_map_mesh->diamond_square();
}

void HeightMap::diamond_square(int width, int height)
{
    this->height_map_mesh->diamond_square(width, height);
}

void HeightMap::load(const std::string &file_path)
{
    if (this->height_map_mesh == nullptr)
    {
        this->height_map_mesh = new HeightMapMesh();
    }

    this->height_map_mesh->load(file_path);
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
    shader->fill("u_Textures.repeat", this->repeat_number);
    shader->fill("u_Model", this->model_matrix);
    this->height_map_mesh->draw(shader);
}

float HeightMap::get_height(float x, float y)
{
    glm::vec3 s = this->get_scale();
    glm::vec3 p = this->get_position();
    float height = this->height_map_mesh->get_height((x - p.x) / s.x, (y - p.z) / s.z); // simples, sem considerar rotacão
    return height * s.y + p.y;
}

#endif
