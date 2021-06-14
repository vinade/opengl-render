#ifndef LINE_MESH_HPP
#define LINE_MESH_HPP

#include "mesh.hpp"

typedef std::vector<glm::vec3> Points;

class LineMesh : public Mesh
{

private:
    static std::vector<glm::vec3> vertex_buffer_src;

    void init();
    void normalize();
    void normalize(glm::vec3 &center, glm::vec3 &size);
    std::vector<float> first_point;

public:
    Points *line_data = nullptr;
    glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    LineMesh();
    LineMesh(Points *data);

    void prepare();
    void prepare(bool preload);
    void setup();
    void load_data_from_vector();
    void draw(const glm::mat4 &model_view, const glm::mat4 &view_matrix, const glm::mat4 &projection_matrix);
};

#endif
