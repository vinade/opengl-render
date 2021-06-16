#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include <cmath>
#include <glm/gtx/rotate_vector.hpp>

void Camera::set_position(const glm::vec3 &position)
{
    this->m_position[0] = position[0];
    this->m_position[1] = position[1];
    this->m_position[2] = position[2];
}

void Camera::rotate(float amount, const glm::vec3 &axis)
{
    this->m_direction = glm::rotate(this->m_direction, amount, axis);
}

// Teste
void Camera::rotate_up(float amount)
{
    glm::vec3 normal_vector = glm::cross(this->m_direction, this->m_up);
    glm::vec3 normal_up = glm::cross(this->m_direction, -normal_vector);
    this->m_up = glm::rotate(normal_up, amount, this->m_direction);
}

void Camera::translate(float amount)
{
    this->m_position = this->m_position + amount * this->m_direction;
}

void Camera::translate(float amount, const glm::vec3 &direction)
{
    this->m_position = this->m_position + amount * direction;
}

void Camera::translate(const glm::vec3 &direction)
{
    this->m_position = this->m_position + direction;
}

void Camera::update_view_matrix()
{
    this->view_matrix = glm::lookAt(this->m_position, this->m_position + this->m_direction, this->m_up);
}

void Camera::point_to(BasicItem *item)
{
    glm::vec3 item_position = item->get_position();
    this->m_direction = item_position - this->m_position;
    this->m_direction = glm::normalize(this->m_direction);
}

#endif