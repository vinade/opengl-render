#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "light.hpp"

void Light::set()
{
    this->set(glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Light::set(const glm::vec4 &color)
{
    this->set_scale(5.0);
    this->debug_mesh = new DebugMesh();
    this->debug_mesh->color = color;
    this->loaded = true;
}

void Light::draw()
{
    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        this->set(); // carrega config padrão
    }

    this->get_model_matrix();
    this->debug_mesh->draw(this->model_matrix);
}

#endif
