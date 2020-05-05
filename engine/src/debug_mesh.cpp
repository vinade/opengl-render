#ifndef DEBUG_MESH_CPP
#define DEBUG_MESH_CPP

#include "camera.hpp"
#include "perspective.hpp"
#include "debug_mesh.hpp"

DebugMesh::DebugMesh()
{
    this->shader = Shader::getShader("debug");
    this->shader->setup("u_Color", DATA_TYPE_VEC4);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->shader->setup("u_View", DATA_TYPE_MAT4);
    this->shader->setup("u_Projection", DATA_TYPE_MAT4);

    this->vao = new VertexArray();

    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 3);

    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer_src, this->vertex_count * this->vertex_size);

    this->vao->add_buffer(vbo, vbo_layout);
    this->ibo = new IndexBuffer(&this->index_buffer_src[0], this->index_count);
};

void DebugMesh::draw(const glm::mat4 &model_view)
{
    Texture::unbind();
    this->shader->fill("u_Color", this->color);
    this->shader->fill("u_Model", model_view);
    this->shader->fill("u_View", Camera::view_matrix);
    this->shader->fill("u_Projection", Perspective::projection_matrix);
    this->shader->exec();
    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw();
}

const float DebugMesh::vertex_buffer_src[24] = {
    -1.0, -1.0, -1.0, // 0: l, b, f
    -1.0, -1.0, 1.0,  // 1: l, b, n
    -1.0, 1.0, -1.0,  // 2: l, t, f
    -1.0, 1.0, 1.0,   // 3: l, t, n
    1.0, -1.0, -1.0,  // 4: r, b, f
    1.0, -1.0, 1.0,   // 5: r, b, n
    1.0, 1.0, -1.0,   // 6: r, t, f
    1.0, 1.0, 1.0     // 7: r, t, n
};

const unsigned int DebugMesh::index_buffer_src[36] = {
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

#endif
