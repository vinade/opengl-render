#ifndef TILE_MESH_CPP
#define TILE_MESH_CPP

#include "tile_mesh.hpp"

TileMesh::TileMesh()
{
    this->texture = new Texture(TILE_MESH_DEFAULT_TEXTURE);
    this->init(TILE_MESH_SIMPLE_SHADER);
};

TileMesh::TileMesh(const std::string &texture_path)
{
    this->texture = new Texture(texture_path);
    this->init(TILE_MESH_SIMPLE_SHADER);
};

TileMesh::TileMesh(Texture *texture_obj)
{
    this->texture = texture_obj;
    this->init(TILE_MESH_SIMPLE_SHADER);
};

TileMesh::TileMesh(const std::string &texture_path, const std::string &shader_name)
{
    this->texture = new Texture(texture_path);
    this->init(shader_name);
};

TileMesh::TileMesh(Texture *texture_obj, const std::string &shader_name)
{
    this->texture = texture_obj;
    this->init(shader_name);
};

void TileMesh::init(const std::string &shader_name)
{
    this->shader = Shader::get_shader(shader_name, SHADER_TYPE_OTHER);
    this->shader->setup("u_Texture", DATA_TYPE_INT);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->vao = new VertexArray();

    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 2);
    vbo_layout->push(DATA_TYPE_FLOAT, 2);

    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer_src2, this->vertex_count * vbo_layout->get_stride());

    this->vao->add_buffer(vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_buffer_src2[0], this->index_count);
};

void TileMesh::draw(const glm::mat4 &model_matrix)
{
    this->texture->bind();
    this->shader->fill("u_Texture", 1);
    this->shader->fill("u_Model", model_matrix);
    this->shader->exec();
    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

const float TileMesh::vertex_buffer_src2[16] = {
    -1.0, -1.0, 0.0, 0.0,
    -1.0, 1.0, 0.0, 1.0,
    1.0, -1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0};

const unsigned int TileMesh::index_buffer_src2[6] = {
    0, 1, 2,
    1, 3, 2};

#endif
