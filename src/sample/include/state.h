#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <glm/glm.hpp>

union GenericFloatData
{
    float f;
    u_char b[4];
};

class State
{
public:
    static State *instance;

    glm::vec3 angle = {0.0, 0.0, 0.0};
    glm::vec3 accel = {0.0, 0.0, 0.0};
    glm::vec3 coords = {0.0, 0.0, 0.0};
    glm::vec3 magneto = {0.0, 0.0, 0.0};
    float height = 0;
    float distance_ground = 0;
    void print();
    void read_buffer(char *buffer);

private:
    static void print_float(const char *message, float *value);
    static void print_vec3f(const char *message, glm::vec3 *value);
    static uint read_float(char *buffer, uint start, float *destiny);
    static uint read_vec3f(char *buffer, uint start, glm::vec3 *destiny);
};

#endif
