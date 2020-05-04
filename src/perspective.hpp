#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

#include <glm/glm.hpp>

class Perspective
{

public:
    static glm::mat4 projection_matrix;

    static void set_default();
};

#endif