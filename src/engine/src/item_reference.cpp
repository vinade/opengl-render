#ifndef ITEM_REFERENCE_CPP
#define ITEM_REFERENCE_CPP

#include "item_reference.hpp"
// #include "render_window.hpp"

ItemReference::ItemReference()
{
    this->x = new Line(ItemReference::axis_x);
    this->y = new Line(ItemReference::axis_y);
    this->z = new Line(ItemReference::axis_z);
}

void ItemReference::set_position(const glm::vec3 position)
{
    this->x->set_position(position);
    this->y->set_position(position);
    this->z->set_position(position);
}

void ItemReference::set_rotation(const glm::vec3 rotation)
{
    this->x->set_rotation(rotation);
    this->y->set_rotation(rotation);
    this->z->set_rotation(rotation);
}

void ItemReference::set_scale(float scale)
{
    this->x->set_scale(scale);
    this->y->set_scale(scale);
    this->z->set_scale(scale);
}

void ItemReference::draw(const glm::mat4 model_matrix, const Camera &camera, const Perspective &perspective)
{
    this->x->draw(model_matrix, camera, perspective);
    this->y->draw(model_matrix, camera, perspective);
    this->z->draw(model_matrix, camera, perspective);
}

Points ItemReference::axis_x = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
Points ItemReference::axis_y = {{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
Points ItemReference::axis_z = {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};

#endif
