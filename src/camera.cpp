#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"

void Camera::set_position(const glm::vec3 &position)
{
    this->m_position[0] = position[0];
    this->m_position[1] = position[1];
    this->m_position[2] = position[2];
}

void Camera::set_rotation(const glm::vec3 &rotation)
{
    this->m_rotation[0] = rotation[0];
    this->m_rotation[1] = rotation[1];
    this->m_rotation[2] = rotation[2];
}

void Camera::update_view_matrix()
{
    Camera::view_matrix = glm::translate(glm::mat4(1.0f), this->m_position);
    Camera::view_matrix = glm::rotate(Camera::view_matrix, glm::radians(this->m_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    Camera::view_matrix = glm::rotate(Camera::view_matrix, glm::radians(this->m_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    Camera::view_matrix = glm::rotate(Camera::view_matrix, glm::radians(this->m_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::view_matrix = glm::mat4(1.0f);

#endif