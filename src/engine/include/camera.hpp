#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
private:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_rotation = {0.0f, 0.0f, 0.0f};

public:
    glm::mat4 view_matrix;

    Camera(){};
    void set_position(const glm::vec3 &position);
    void set_rotation(const glm::vec3 &rotation);
    void update_view_matrix();
};

#endif