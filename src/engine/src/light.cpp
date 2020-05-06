#ifndef LIGHT_CPP
#define LIGHT_CPP

#include "perspective.hpp"
#include "camera.hpp"
#include "light.hpp"

void Light::set()
{
    this->set(glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void Light::set(const glm::vec4 &color)
{
    this->set(glm::vec4(1.0, 1.0, 1.0, 1.0), LIGHT_DIFFUSE);
}

void Light::set(const glm::vec4 &color, LightType type)
{
    this->color = color;
    this->type = type;

    if (this->loaded)
    {
        this->debug_mesh->color = color;
        return;
    }

    this->set_scale(5.0);
    this->debug_mesh = new DebugMesh();
    this->debug_mesh->color = color;
    this->loaded = true;
}

void Light::draw(const Camera &camera, const Perspective &perspective)
{
    if (!this->loaded) // Se não foi carregado na hora do draw()
    {
        this->set(); // carrega config padrão
    }

    this->update_model_matrix();
    this->debug_mesh->draw(this->model_matrix, camera.view_matrix, perspective.projection_matrix);
}

void Light::set_color(const glm::vec4 &color)
{
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
    this->color[3] = color[3];

    if (this->loaded)
    {
        this->debug_mesh->color = color;
    }
}

#endif
