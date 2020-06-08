#ifndef TILE_MESH_HPP
#define TILE_MESH_HPP

#include "mesh.hpp"

#define TILE_MESH_SIMPLE_SHADER "simple-tile"
#define TILE_MESH_DEFAULT_TEXTURE "teste.png"

class TileMesh : public Mesh
{
private:
    static const float vertex_buffer_src2[16];
    static const unsigned int index_buffer_src2[6];

public:
    unsigned int vertex_count = 4;
    unsigned int index_count = 6;
    std::string shader_name;
    Texture *texture = nullptr;

    TileMesh();
    TileMesh(const std::string &texture_path);
    TileMesh(const std::string &texture_path, const std::string &shader_name);
    TileMesh(Texture *texture_obj);
    TileMesh(Texture *texture_obj, const std::string &shader_name);

    void init(const std::string &shader_name);
    void draw();
    void draw(const glm::mat4 &model_matrix);
    void setup();
};

#endif
