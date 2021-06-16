#ifndef REFERENCE_ITEM_HPP
#define REFERENCE_ITEM_HPP

#include "line.hpp"

class ReferenceItem
{

private:
    Line *x;
    Line *y;
    Line *z;

public:
    static Points axis_x;
    static Points axis_y;
    static Points axis_z;

    ReferenceItem();

    void setup();
    void set_position(const glm::vec3 position);
    void set_rotation(const glm::vec3 rotation);
    void set_scale(float scale);

    void draw(const glm::mat4 model_matrix, const Camera &camera, const Perspective &perspective);
};

#endif
