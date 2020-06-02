#ifndef PUPPETEER_HPP
#define PUPPETEER_HPP

#include <glm/glm.hpp>
#include "shader.hpp"
#include "scenario_item.hpp"

class Puppeteer
{
public:
    ScenarioItem *obj = nullptr;
    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

    void draw(Shader *shader);
};

#endif
