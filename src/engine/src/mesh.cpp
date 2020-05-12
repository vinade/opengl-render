#ifndef MESH_CPP
#define MESH_CPP

#include "camera.hpp"
#include "perspective.hpp"
#include "mesh.hpp"

void Mesh::flat_vertex_data(VertexBufferLayout *vbo_layout)
{
    int stride = vbo_layout->get_stride();
    int counter = vbo_layout->get_counter();
    int vertices_length = this->vertex_data.size();

    this->vertex_buffer = (float *)malloc(sizeof(float) * stride * vertices_length);

    for (int i = 0; i < vertices_length; i++)
    {
        std::copy(this->vertex_data[i].begin(), this->vertex_data[i].end(), &this->vertex_buffer[i * counter]);
    }
}

void Mesh::normalize(glm::vec3 &center, glm::vec3 &size)
{

    float max = size.x;

    if (max < size.y)
    {
        max = size.y;
    }

    if (max < size.z)
    {
        max = size.z;
    }

    for (unsigned int i = 0; i < this->vertex_count; i++)
    {
        this->vertex_data[i][0] = this->vertex_data[i][0] - center.x;
        this->vertex_data[i][1] = this->vertex_data[i][1] - center.y;
        this->vertex_data[i][2] = this->vertex_data[i][2] - center.z;

        if (max > 0)
        {
            this->vertex_data[i][0] = this->vertex_data[i][0] / max;
            this->vertex_data[i][1] = this->vertex_data[i][1] / max;
            this->vertex_data[i][2] = this->vertex_data[i][2] / max;
        }
    }
}

void Mesh::prepare(glm::vec3 &center, glm::vec3 &size)
{

    this->normalize(center, size);

    this->vao = new VertexArray();
    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 3);
    vbo_layout->push(DATA_TYPE_FLOAT, 2);
    vbo_layout->push(DATA_TYPE_FLOAT, 3);

    this->flat_vertex_data(vbo_layout);

    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer, this->vertex_count * this->vertex_size);

    this->vao->add_buffer(vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_data[0], this->index_count);
}

void Mesh::draw(Shader *shader)
{
    if (shader->use_materials)
    {
        shader->fill("u_Material.diffuse_color", this->material.diffuse_color);
        shader->fill("u_Material.specular_color", this->material.specular_color);
        shader->fill("u_Material.ambient_color", this->material.ambient_color);
        shader->fill("u_Material.emission_color", this->material.emission_color);
        shader->fill("u_Material.shininess", this->material.shininess);
        shader->fill("u_Material.shininess_strength", this->material.shininess_strength);
    }

    shader->exec();
    if (this->material.diffuse_texture_flag)
    {
        this->material.diffuse_texture->bind();
    }
    else
    {
        Texture::fallback->bind();
    }

    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

#endif
