#ifndef LINE_HPP
#define LINE_HPP

#include "line_mesh.hpp"
#include "perspective.hpp"
#include "camera.hpp"
#include "scenario_item.hpp"

class Line : public ScenarioItem
{
private:
    bool loaded = false;
    Points *line_data = nullptr;

public:
    glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    LineMesh *line_mesh;

    Line(){};
    Line(Points *data);
    Line(Points &data);

    void set();
    void set(const glm::vec3 &line);

    void draw(){};
    void draw(const Camera &camera, const Perspective &perspective);
    void draw(const glm::mat4 model_matrix, const Camera &camera, const Perspective &perspective);

    void set_color(const glm::vec4 &color);
};

#endif
