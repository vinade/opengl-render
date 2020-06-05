#ifndef SKYBOX_MESH_CPP
#define SKYBOX_MESH_CPP

#include "camera.hpp"
#include "perspective.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "stb/stb_image.h"
#include "skybox_mesh.hpp"
#include "cmake_params.hpp"
#include "render_window.hpp"

SkyboxMesh::SkyboxMesh()
{
    std::string texture_path = SkyboxMesh::skybox_textures_folder + SKYBOX_DEFAULT_TEXTURE;
    this->init(texture_path);
}

SkyboxMesh::SkyboxMesh(const std::string &texture_path)
{
    this->init(texture_path);
}

void SkyboxMesh::init(const std::string &texture_path)
{
    this->texture_path = texture_path;
    if (!RenderWindow::is_render_thread())
    {
        RenderWindow::context->to_setup(this);
        return;
    }

    this->shader = Shader::get_shader("skybox", SHADER_TYPE_OTHER);
    this->shader->use_mvp = true;

    this->shader->setup("u_Texture", DATA_TYPE_INT);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->shader->setup("u_View", DATA_TYPE_MAT4);
    this->shader->setup("u_Color", DATA_TYPE_VEC4);
    this->shader->setup("u_Projection", DATA_TYPE_MAT4);
    this->shader->setup("u_Projection", DATA_TYPE_MAT4);
    this->shader->setup("u_AmbientLight.ambient", DATA_TYPE_FLOAT);
    this->shader->setup("u_AmbientLight.color", DATA_TYPE_VEC4);

    this->vao = new VertexArray();

    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 3);

    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer_src, this->vertex_count * this->vertex_size);

    this->vao->add_buffer(vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_buffer_src[0], this->index_count);

    this->load_cubemap(SKYBOX_DEFAULT_TEXTURE);
};

void SkyboxMesh::load_cubemap(const std::string &texture_name)
{
    int width, height, nrChannels;

    glGenTextures(1, &this->texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);

    stbi_set_flip_vertically_on_load(0);

    for (unsigned int i = 0; i < SkyboxMesh::faces.size(); i++)
    {
        std::string face_name = SkyboxMesh::skybox_textures_folder + texture_name + "/" + faces[i];
        unsigned char *data = stbi_load(face_name.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "[SKYBOX] Cubemap tex failed to load at path: " << face_name << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyboxMesh::draw(const glm::mat4 &view_matrix, const glm::mat4 &projection_matrix)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture_id);
    this->shader->fill("u_Texture", 0);
    this->shader->fill("u_Model", this->model_matrix);
    this->shader->fill("u_View", view_matrix);
    this->shader->fill("u_Projection", projection_matrix);
    this->shader->fill("u_Color", this->color);

    if (this->light != nullptr)
    {
        this->shader->fill("u_AmbientLight.ambient", this->light->ambient);
        this->shader->fill("u_AmbientLight.color", this->light->color);
    }

    this->shader->exec();
    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

void SkyboxMesh::update_model_matrix()
{
    this->model_matrix = glm::translate(glm::mat4(1.0f), this->m_position);
    this->model_matrix = glm::scale(this->model_matrix, this->m_scale);
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}

void SkyboxMesh::set_position(const glm::vec3 position)
{
    this->m_position[0] = position[0];
    this->m_position[1] = position[1];
    this->m_position[2] = position[2];
    this->update_model_matrix();
}

void SkyboxMesh::set_rotation(const glm::vec3 rotation)
{
    this->m_rotation[0] = rotation[0];
    this->m_rotation[1] = rotation[1];
    this->m_rotation[2] = rotation[2];
    this->update_model_matrix();
}

void SkyboxMesh::set_scale(const glm::vec3 scale)
{
    this->m_scale[0] = scale[0];
    this->m_scale[1] = scale[1];
    this->m_scale[2] = scale[2];
    this->update_model_matrix();
}

void SkyboxMesh::set_scale(float scale)
{
    this->m_scale[0] = scale;
    this->m_scale[1] = scale;
    this->m_scale[2] = scale;
    this->update_model_matrix();
}

void SkyboxMesh::inc_rotation(const glm::vec3 rotation)
{
    this->m_rotation[0] = this->m_rotation[0] + rotation[0];
    this->m_rotation[1] = this->m_rotation[1] + rotation[1];
    this->m_rotation[2] = this->m_rotation[2] + rotation[2];
    this->update_model_matrix();
}

void SkyboxMesh::setup()
{
    this->init(this->texture_path);
}

const float SkyboxMesh::vertex_buffer_src[24] = {
    -1.0, -1.0, -1.0, // 0: l, b, f
    -1.0, -1.0, 1.0,  // 1: l, b, n
    -1.0, 1.0, -1.0,  // 2: l, t, f
    -1.0, 1.0, 1.0,   // 3: l, t, n
    1.0, -1.0, -1.0,  // 4: r, b, f
    1.0, -1.0, 1.0,   // 5: r, b, n
    1.0, 1.0, -1.0,   // 6: r, t, f
    1.0, 1.0, 1.0     // 7: r, t, n
};

// const float SkyboxMesh::vertex_buffer_src[40] = {
//     -1.0, -1.0, -1.0, 0.0, 0.25, // 0: l, b, f
//     -1.0, -1.0, 1.0, 0.25, 0.25, // 1: l, b, n
//     -1.0, 1.0, -1.0, 0.0, 0.75,  // 2: l, t, f
//     -1.0, 1.0, 1.0, 0.25, 0.75,  // 3: l, t, n
//     1.0, -1.0, -1.0, 0.75, 0.25, // 4: r, b, f
//     1.0, -1.0, 1.0, 0.5, 0.25,   // 5: r, b, n
//     1.0, 1.0, -1.0, 0.75, 0.75,  // 6: r, t, f
//     1.0, 1.0, 1.0, 0.5, 0.75     // 7: r, t, n
// };

const unsigned int SkyboxMesh::index_buffer_src[36] = {
    0, 1, 5, // bottom
    0, 4, 5,
    2, 3, 7, // top
    2, 6, 7,
    1, 3, 7, // near
    1, 5, 7,
    0, 2, 6, // far
    0, 4, 6,
    0, 1, 3, // left
    0, 2, 3,
    4, 5, 7, // right
    4, 6, 7  //
};

const std::string SkyboxMesh::skybox_textures_folder = std::string(CMAKE_ROOT_DIR SKYBOX_TEXTURES_FOLDER);
const std::vector<std::string> SkyboxMesh::faces = {
    "posx.jpg",
    "negx.jpg",
    "posy.jpg",
    "negy.jpg",
    "posz.jpg",
    "negz.jpg"};

#endif
