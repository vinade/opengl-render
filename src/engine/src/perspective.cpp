#ifndef PERSPECTIVE_CPP
#define PERSPECTIVE_CPP

#include <glm/gtc/matrix_transform.hpp>
#include "perspective.hpp"

void Perspective::set_default()
{
    Perspective::projection_matrix = glm::perspective(glm::radians(45.0), 1.0, 1.0, 2000.0);
}

glm::mat4 Perspective::projection_matrix;

#endif