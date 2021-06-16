#ifndef REFERENCE_ITEM_CPP
#define REFERENCE_ITEM_CPP

#include "reference_item.hpp"

ReferenceItem::ReferenceItem()
{
    this->x = new Line(ReferenceItem::axis_x);
    this->y = new Line(ReferenceItem::axis_y);
    this->z = new Line(ReferenceItem::axis_z);

    this->x->set_color(glm::vec4(1.0, 0.0, 0.0, 1.0));
    this->y->set_color(glm::vec4(0.0, 1.0, 0.0, 1.0));
    this->z->set_color(glm::vec4(0.0, 0.0, 1.0, 1.0));
}

void ReferenceItem::set_position(const glm::vec3 position)
{
    this->x->set_position(position);
    this->y->set_position(position);
    this->z->set_position(position);
}

void ReferenceItem::set_rotation(const glm::vec3 rotation)
{
    this->x->set_rotation(rotation);
    this->y->set_rotation(rotation);
    this->z->set_rotation(rotation);
}

void ReferenceItem::set_scale(float scale)
{
    this->x->set_scale(scale);
    this->y->set_scale(scale);
    this->z->set_scale(scale);
}

void ReferenceItem::draw(const glm::mat4 model_matrix, const Camera &camera, const Perspective &perspective)
{
    this->x->draw(model_matrix, camera, perspective);
    this->y->draw(model_matrix, camera, perspective);
    this->z->draw(model_matrix, camera, perspective);
}

Points ReferenceItem::axis_x = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};
Points ReferenceItem::axis_y = {{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
Points ReferenceItem::axis_z = {{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};

#endif
