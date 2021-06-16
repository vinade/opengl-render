#ifndef BASIC_ITEM_CPP
#define BASIC_ITEM_CPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "basic_item.hpp"

void BasicItem::update_model_matrix()
{

    // pela normalizacao, o objeto já se encontra no p(0,0,0)

    this->model_matrix = glm::translate(glm::mat4(1.0f), this->m_position);
    this->model_matrix = glm::scale(this->model_matrix, this->m_scale);
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}

void BasicItem::update_model_matrix(const glm::vec3 &pos, const glm::vec3 &rotation, const glm::vec3 &scale)
{
    this->model_matrix = glm::translate(glm::mat4(1.0f), pos);
    this->model_matrix = glm::scale(this->model_matrix, scale);
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}

void BasicItem::set_position(const glm::vec3 position)
{
    this->m_position[0] = position[0];
    this->m_position[1] = position[1];
    this->m_position[2] = position[2];
}

void BasicItem::set_rotation(const glm::vec3 rotation)
{
    this->m_rotation[0] = rotation[0];
    this->m_rotation[1] = rotation[1];
    this->m_rotation[2] = rotation[2];
}

void BasicItem::set_scale(const glm::vec3 scale)
{
    this->m_scale[0] = scale[0];
    this->m_scale[1] = scale[1];
    this->m_scale[2] = scale[2];
}

void BasicItem::set_scale(float scale)
{
    this->m_scale[0] = scale;
    this->m_scale[1] = scale;
    this->m_scale[2] = scale;
}

void BasicItem::inc_position(const glm::vec3 position)
{
    this->m_position[0] = this->m_position[0] + position[0];
    this->m_position[1] = this->m_position[1] + position[1];
    this->m_position[2] = this->m_position[2] + position[2];
}

void BasicItem::inc_rotation(const glm::vec3 rotation)
{
    this->m_rotation[0] = this->m_rotation[0] + rotation[0];
    this->m_rotation[1] = this->m_rotation[1] + rotation[1];
    this->m_rotation[2] = this->m_rotation[2] + rotation[2];
}

void BasicItem::inc_scale(const glm::vec3 scale)
{
    this->m_scale[0] = this->m_scale[0] + scale[0];
    this->m_scale[1] = this->m_scale[1] + scale[1];
    this->m_scale[2] = this->m_scale[2] + scale[2];
}

void BasicItem::inc_scale(float scale)
{
    this->m_scale[0] = this->m_scale[0] + scale;
    this->m_scale[1] = this->m_scale[1] + scale;
    this->m_scale[2] = this->m_scale[2] + scale;
}

#endif
