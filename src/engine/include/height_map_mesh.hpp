#ifndef HEIGHT_MAP_MESH_HPP
#define HEIGHT_MAP_MESH_HPP

#include "mesh.hpp"

class HeightMapMesh : public Mesh
{
public:
    float texture_size = 2.0;
    const int vertex_elements_count = 14; // 3 + 2 + 3 + 3 + 3
    unsigned int width;
    unsigned int height;

    VertexBufferLayout *vbo_layout = nullptr;
    VertexBuffer *vbo = nullptr;
    HeightMapMesh();
    void from_float_array(float *data, unsigned int width, unsigned int height);
    void setup();
};

#endif