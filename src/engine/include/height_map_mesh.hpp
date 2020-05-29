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
    float min_height;
    float max_height;
    float suavity = 2.0;

    VertexBufferLayout *vbo_layout = nullptr;
    VertexBuffer *vbo = nullptr;

    HeightMapMesh();

    float get_height(float x, float y);
    void load(const std::string &file_path);
    void diamond_square();
    void diamond_square(int width, int height);
    void diamond_square_recursion(glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3, float range, float *data, char *flag);
    void from_float_array(float *data, int width, int height);

    void setup();

    float get_height_from_3p(float x, float y, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    glm::vec3 get_point_from_vb(int x, int y);

    inline int address_of_coord(glm::ivec2 p);

    static const std::string height_map_folder;
};

#endif