#ifndef MESH_CPP
#define MESH_CPP

#include "camera.hpp"
#include "perspective.hpp"
#include "mesh.hpp"
#include "render_window.hpp"

Mesh::Mesh()
{

    if (!Material::materials.size())
    {
        std::cerr << "[Mesh] Criando material de fallback." << std::endl;
        Material::materials.push_back(new Material());
    }

    this->material = Material::materials[0];
}

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
    this->prepare(center, size, false);
}

void Mesh::prepare(glm::vec3 &center, glm::vec3 &size, bool preload)
{
    this->normalize(center, size);

    if (!preload)
    {
        this->setup();
    }
    else
    {
        this->ready = false;
        RenderWindow::context->to_setup(this);
    }
}

void Mesh::setup()
{

    if (this->ready)
    {
        return;
    }
    this->ready = true;

    this->vao = new VertexArray();
    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 3);
    vbo_layout->push(DATA_TYPE_FLOAT, 2);
    vbo_layout->push(DATA_TYPE_FLOAT, 3);
    vbo_layout->push(DATA_TYPE_FLOAT, 3);
    vbo_layout->push(DATA_TYPE_FLOAT, 3);

    this->flat_vertex_data(vbo_layout);

    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer, this->vertex_count * vbo_layout->get_counter());

    this->vao->add_buffer(vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_data[0], this->index_count);
    std::cerr << "UEPA\n";
}

void Mesh::draw(Shader *shader)
{
    if (shader->use_materials)
    {
        shader->fill("u_Material.color", this->material->color);
        shader->fill("u_Material.metallic", this->material->metallic);
        shader->fill("u_Material.roughness", this->material->roughness);
        shader->fill("u_Material.reflection", this->material->reflection);

        shader->fill("u_Material.diffuse_texture_flag", this->material->diffuse_textures.size() ? 1 : 0);
        shader->fill("u_Material.normal_texture_flag", this->material->normal_textures.size() ? 1 : 0);
        shader->fill("u_Material.ambient_occlusion_texture_flag", this->material->ambient_occlusion_textures.size() ? 1 : 0);
        shader->fill("u_Material.metallic_texture_flag", this->material->metallic_textures.size() ? 1 : 0);
        shader->fill("u_Material.roughness_texture_flag", this->material->roughness_textures.size() ? 1 : 0);
        shader->fill("u_Material.reflection_texture_flag", this->material->reflection_textures.size() ? 1 : 0);
    }

    shader->exec();
    if (this->material->diffuse_textures.size())
    {
        this->material->diffuse_textures[0]->bind();
    }
    else
    {
        Texture::fallback->bind();
    }

    if (this->material->normal_textures.size())
    {
        this->material->normal_textures[0]->bind();
    }

    if (this->material->ambient_occlusion_textures.size())
    {
        this->material->ambient_occlusion_textures[0]->bind();
    }

    if (this->material->metallic_textures.size())
    {
        this->material->metallic_textures[0]->bind();
    }

    if (this->material->roughness_textures.size())
    {
        this->material->roughness_textures[0]->bind();
    }

    if (this->material->reflection_textures.size())
    {
        this->material->reflection_textures[0]->bind();
    }

    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

#endif
