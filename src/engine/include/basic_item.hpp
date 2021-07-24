#ifndef BASIC_ITEM_HPP
#define BASIC_ITEM_HPP

#include <glm/glm.hpp>

class BasicItem
{

public:
    glm::vec3 m_position = {0.0, 0.0, 0.0};
    glm::vec3 m_rotation = {0.0, 0.0, 0.0};
    glm::vec3 m_scale = {1.0, 1.0, 1.0};

    glm::mat4 model_matrix;

    BasicItem(){};

    void update_model_matrix();
    void update_model_matrix(const glm::vec3 &pos, const glm::vec3 &rotation, const glm::vec3 &scale);
    inline glm::vec3 get_position() { return glm::vec3(this->m_position[0], this->m_position[1], this->m_position[2]); };
    inline glm::vec3 get_scale() { return glm::vec3(this->m_scale[0], this->m_scale[1], this->m_scale[2]); };

    void set_position(const glm::vec3 pos);
    void set_rotation(const glm::vec3 rotation);
    void set_scale(const glm::vec3 scale);
    void set_scale(float scale);

    void inc_position(const glm::vec3 pos);
    void inc_rotation(const glm::vec3 rotation);
    void inc_scale(const glm::vec3 scale);
    void inc_scale(float scale);
};

#endif
