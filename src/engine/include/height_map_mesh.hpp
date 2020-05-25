#ifndef HEIGHT_MAP_MESH_HPP
#define HEIGHT_MAP_MESH_HPP

#include "cmake_params.hpp"
#include "mesh.hpp"

#define HEIGHT_MAP_MESH_DEFAULT_FOLDER "./res/textures/height_maps/"

class HeightMapMesh : public Mesh
{
public:
    float texture_size = 2.0;
    const int vertex_elements_count = 14; // 3 + 2 + 3 + 3 + 3
    int width;
    int height;

    VertexBufferLayout *vbo_layout = nullptr;
    VertexBuffer *vbo = nullptr;
    HeightMapMesh();
    void from_float_array(float *data, int width, int height);
    void load(const std::string &file_path);
    void setup();

    static const std::string height_map_folder;
};

#endif