#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

#include <glm/glm.hpp>

class Perspective
{
public:
    double fovy;
    double aspect;
    double near;
    double far;

    glm::mat4 projection_matrix;

    Perspective();
    Perspective(double fovy, double aspect, double near, double far) : fovy(fovy), aspect(aspect), near(near), far(far)
    {
        this->update_matrix();
    };

    void update_matrix();

    void set_fovy(double fovy);
    void set_aspect(double aspect);
    void set_near(double near);
    void set_far(double far);

    static void set_default(Perspective *perspective);
};

#endif