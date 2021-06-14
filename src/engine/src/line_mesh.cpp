#ifndef LINE_MESH_CPP
#define LINE_MESH_CPP

#include "line_mesh.hpp"
#include "render_window.hpp"

LineMesh::LineMesh(Points *data)
{
    this->line_data = data;
    this->init();
}

LineMesh::LineMesh()
{
    this->init();
}

void LineMesh::init()
{
    std::cerr << "\t[LineMesh] init\n";

    this->shader = Shader::get_shader("debug", SHADER_TYPE_OTHER);
    this->shader->use_mvp = true;

    // esse bloco será desnecessário, ficará como responsabilidade da cena.
    this->shader->setup("u_Color", DATA_TYPE_VEC4);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->shader->setup("u_View", DATA_TYPE_MAT4);
    this->shader->setup("u_Projection", DATA_TYPE_MAT4);

    std::cerr << "\t[LineMesh] init finished\n";
}

void LineMesh::prepare()
{
    this->prepare(false);
}

void LineMesh::prepare(bool preload)
{
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

void LineMesh::normalize(glm::vec3 &center, glm::vec3 &size)
{
    this->normalize();
}

void LineMesh::normalize()
{
    this->vertex_count = this->vertex_data.size();
    std::cerr << "\t[LineMesh] normalize\n";
    std::cerr << "\t\t vertex counter: " << this->vertex_count << "\n";

    for (unsigned int i = 0; i < this->vertex_count; i++)
    {
        this->vertex_data[i][0] = this->vertex_data[i][0] - this->vertex_data[0][0];
        this->vertex_data[i][1] = this->vertex_data[i][1] - this->vertex_data[0][1];
        this->vertex_data[i][2] = this->vertex_data[i][2] - this->vertex_data[0][2];
    }

    std::cerr << "\t[LineMesh] normalize finished\n";
}

void LineMesh::load_data_from_vector()
{
    std::cerr << "\t[LineMesh] load_data_from_vector\n";

    if (this->line_data == nullptr)
    {
        return;
    }

    unsigned int data_size = this->line_data->size();
    Points data = *(this->line_data);

    for (unsigned int i = 0; i < data_size; i++)
    {
        std::vector<float> vertex;
        vertex.push_back(data[i].x);
        vertex.push_back(data[i].y);
        vertex.push_back(data[i].z);

        this->vertex_data.push_back(vertex);
        this->index_data.push_back(i);
        std::cerr << "\t\t" << i << "\n";
    }
    // this->index_data.push_back(0);
    // std::cerr << "\t\t" << 0 << "\n";

    this->vertex_count = this->vertex_data.size();
    this->index_count = this->index_data.size();
    std::cerr << "\t[LineMesh] load_data_from_vector finished\n";
}

void LineMesh::setup()
{

    std::cerr << "\t[LineMesh] Setup called\n";

    if (this->ready)
    {
        return;
    }

    if (this->line_data == nullptr)
    {
        this->line_data = &LineMesh::vertex_buffer_src;
    }

    this->ready = true;

    this->load_data_from_vector();
    this->normalize();

    this->vao = new VertexArray();
    VertexBufferLayout *vbo_layout = new VertexBufferLayout();
    vbo_layout->push(DATA_TYPE_FLOAT, 3);

    this->flat_vertex_data(vbo_layout);

    std::cerr << "\t[LineMesh] index_count:" << this->index_count << "\n";
    std::cerr << "\t[LineMesh] vertex_count:" << this->vertex_count << "\n";
    std::cerr << "\t[LineMesh] vbo_layout counter:" << vbo_layout->get_counter() << "\n";
    std::cerr << "\t[LineMesh] vertexbuffer Size:" << this->vertex_count * vbo_layout->get_counter() << "\n";
    std::cerr << "\t[LineMesh] vertexbuffer Size:" << this->vertex_count * vbo_layout->get_counter() << "\n";
    VertexBuffer *vbo = new VertexBuffer(this->vertex_buffer, this->vertex_count * vbo_layout->get_counter());

    this->vao->add_buffer(vbo, vbo_layout);

    this->ibo = new IndexBuffer(&this->index_data[0], this->index_count);

    std::cerr << "\t[LineMesh] Setup finished\n";
}

void LineMesh::draw(const glm::mat4 &model_view, const glm::mat4 &view_matrix, const glm::mat4 &projection_matrix)
{
    // std::cerr << "\t[LineMesh] Draw called\n";
    Texture::unbind();
    this->shader->fill("u_Color", this->color);
    this->shader->fill("u_Model", model_view);
    this->shader->fill("u_View", view_matrix);
    this->shader->fill("u_Projection", projection_matrix);
    this->shader->exec();

    this->vao->bind();
    this->ibo->bind();
    this->ibo->draw(GL_LINE_STRIP);
}

Points LineMesh::vertex_buffer_src = {
    {0.0, 0.0, 0.0}, // p0
    {1.0, 0.0, 0.0}  // p1
};

#endif
