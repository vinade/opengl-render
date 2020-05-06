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
    this->type = type;
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

#endif
