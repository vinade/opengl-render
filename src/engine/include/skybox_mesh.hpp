#ifndef SKYBOX_MESH_HPP
#define SKYBOX_MESH_HPP

#define SKYBOX_TEXTURES_FOLDER "./res/textures/skyboxes/"
#define SKYBOX_DEFAULT_TEXTURE "yokohama"

#include "mesh.hpp"
#include "light.hpp"
#include <glm/glm.hpp>

class SkyboxMesh : public Mesh
{
private:
    static const float vertex_buffer_src[24];
    static const unsigned int index_buffer_src[36];

    glm::vec3 m_position = {0.0, 0.0, 0.0};
    glm::vec3 m_rotation = {0.0, 0.0, 0.0};
    glm::vec3 m_scale = {1.0, 1.0, 1.0};
    glm::mat4 model_matrix;
    void update_model_matrix();

public:
    static const std::string skybox_textures_folder;
    static const std::vector<std::string> faces;
    Light *light = nullptr;

    unsigned int vertex_size = 3 * sizeof(float);
    unsigned int vertex_count = 24;
    unsigned int index_count = 36;
    unsigned int texture_id;

    void set_position(const glm::vec3 pos);
    void set_rotation(const glm::vec3 rotation);
    void set_scale(const glm::vec3 scale);
    void set_scale(float scale);
    void inc_rotation(const glm::vec3 rotation);

    SkyboxMesh();
    SkyboxMesh(const std::string &texture_path);

    void init(const std::string &texture_path);
    void draw(const glm::mat4 &view_matrix, const glm::mat4 &projection_matrix);
    void load_cubemap(const std::string &texture_name);
};

#endif
