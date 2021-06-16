#ifndef LINE_CPP
#define LINE_CPP

#include "line.hpp"
#include <glm/gtx/component_wise.hpp>

Line::Line(Points *data)
{
    this->line_data = data;
}

Line::Line(Points &data)
{
    this->line_data = (Points *)&data;
}

void Line::set()
{
    if (this->loaded)
    {
        this->line_mesh->color = this->color;
        return;
    }

    std::cerr << "\t[Line] Set LineMesh\n";
    // this->set_scale(1.0);
    this->line_mesh = new LineMesh(this->line_data);
    this->line_mesh->color = this->color;
    this->line_mesh->prepare();
    this->loaded = true;
}

void Line::set(const glm::vec3 &line)
{
    this->set_scale(line);
}

void Line::draw(const Camera &camera, const Perspective &perspective)
{
    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        this->set(); // carrega config padrão
    }

    this->update_model_matrix();
    this->draw(this->model_matrix, camera, perspective);
}

void Line::draw(const glm::mat4 model_matrix, const Camera &camera, const Perspective &perspective)
{

    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        this->set(); // carrega config padrão
    }

    this->line_mesh->draw(model_matrix, camera.view_matrix, perspective.projection_matrix);
}

void Line::set_color(const glm::vec4 &color)
{
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
    this->color[3] = color[3];

    if (this->loaded)
    {
        this->line_mesh->color = color;
    }
}

#endif
