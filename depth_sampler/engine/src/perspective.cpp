#ifndef PERSPECTIVE_CPP
#define PERSPECTIVE_CPP

#include <glm/gtc/matrix_transform.hpp>
#include "perspective.hpp"

Perspective::Perspective()
{
    Perspective::set_default(this);
}

void Perspective::set_default(Perspective *perspective)
{
    perspective->fovy = glm::radians(45.0);
    perspective->aspect = 1.0;
    perspective->near = 1.0;
    perspective->far = 8000.0;

    perspective->update_matrix();
}

void Perspective::update_matrix()
{
    this->projection_matrix = glm::perspective(
        this->fovy,
        this->aspect,
        this->near,
        this->far);
}

#endif