#ifndef DEBUG_MESH_HPP
#define DEBUG_MESH_HPP

#include "mesh.hpp"

class DebugMesh : public Mesh
{
private:
    static const float vertex_buffer_src[24];
    static const unsigned int index_buffer_src[36];

public:
    unsigned int vertex_count = 8;
    unsigned int index_count = 36;

    glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    DebugMesh();
    void draw(const glm::mat4 &model_view, const glm::mat4 &view_matrix, const glm::mat4 &projection_matrix);
};

#endif
