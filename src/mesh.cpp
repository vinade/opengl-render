#ifndef MESH_CPP
#define MESH_CPP

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

    for (int i = 0; i < this->vertex_count; i++)
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
    this->shader = Shader::getShader("std");
    this->shader->setup("u_Texture", DATA_TYPE_INT);
    this->shader->setup("u_MVP", DATA_TYPE_MAT4);

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

void Mesh::draw(glm::mat4 *mvp)
{
    this->shader->fill("u_Texture", 0);
    this->shader->fill("u_MVP", *mvp);
    this->shader->exec();

    this->textures[0]->bind();
    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

#endif
