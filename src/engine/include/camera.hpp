#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "scenario_item.hpp"

class Camera
{
private:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};

public:
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_direction = {1.0f, 0.0f, 0.0f};
    glm::mat4 view_matrix;

    Camera(){};
    inline glm::vec3 get_position() { return glm::vec3(this->m_position[0], this->m_position[1], this->m_position[2]); };
    inline glm::vec3 get_direction() { return glm::vec3(this->m_direction[0], this->m_direction[1], this->m_direction[2]); };
    inline glm::vec3 get_up() { return glm::vec3(this->m_up[0], this->m_up[1], this->m_up[2]); };

    void set_position(const glm::vec3 &position);
    void translate(float amount);
    void translate(const glm::vec3 &direction);
    void translate(float amount, const glm::vec3 &direction);
    void rotate(float amount, const glm::vec3 &axis);
    void rotate_up(float amount);
    void point_to(ScenarioItem *item);
    void update_view_matrix();
};

#endif