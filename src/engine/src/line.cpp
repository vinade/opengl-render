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

void Line::draw(const Camera &camera, const Perspective &perspective)
{
    // std::cerr << "\t[Line] Draw called\n";

    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        // std::cerr << "\t[Line] Not loaded\n";
        this->set(); // carrega config padrão
    }

    // std::cerr << "\t[Line] update matrix\n";
    this->update_model_matrix();
    // std::cerr << "\t[Line] draw mesh\n";
    this->line_mesh->draw(this->model_matrix, camera.view_matrix, perspective.projection_matrix);
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
