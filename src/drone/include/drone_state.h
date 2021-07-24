#ifndef DRONE_STATE_H
#define DRONE_STATE_H

#include <iostream>
#include <glm/glm.hpp>
#include "geometry.h"

union GenericFloatData
{
    float f;
    u_char b[4];
};

class DroneState
{
public:
    static DroneState *instance;

    glm::vec3 angle = {0.0, 0.0, 0.0};
    glm::vec3 accel = {0.0, 0.0, -1.0};
    glm::vec3 accel_norm = {0.0, 0.0, 0.0};
    glm::vec3 coords = {0.0, 0.0, 0.0};
    glm::vec3 magneto = {1.0, 0.0, 0.0};
    glm::vec3 magneto_north = {0.0, 0.0, 0.0};
    glm::vec3 magneto_down = {0.0, 0.0, 0.0};
    glm::vec3 magneto_east = {0.0, 0.0, 0.0};

    glm::vec3 magneto_offset_north = {0.0, 0.0, 0.0};
    glm::vec3 magneto_offset_down = {0.0, 0.0, 0.0};
    glm::vec3 magneto_offset_east = {0.0, 0.0, 0.0};

    /* Mock */
    float mock_mag_alpha = 0;
    float mock_mag_beta = 0;
    float mock_accel_alpha = 0;
    float mock_accel_beta = 0;
    glm::vec3 mock_accel = {0.0, 0.0, -1.0};
    glm::vec3 mock_magneto = {0.0, 0.0, -1.0};

    bool has_north = false;
    bool has_mag_down = false;
    bool has_east = false;
    bool use_mock = false;

    float height = 0;
    float distance_ground = 0;
    void read_buffer(char *buffer);

    static void print();
    static void normalize_accel();
    static void normalize_magneto();
    static void compensate_magneto_from_accel();
    static void update_north();
    static void update_mag_down();
    static void update_east();
    static void accel_mag_adjust(Vec3 *mag, Vec3 *accel);

private:
    static void print_float(const char *message, float *value);
    static void print_vec3f(const char *message, glm::vec3 *value);
    static uint read_float(char *buffer, uint start, float *destiny);
    static uint read_vec3f(char *buffer, uint start, glm::vec3 *destiny);
    static void read_mock();
};

#endif
