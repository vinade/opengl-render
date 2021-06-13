#ifndef DRONE_STATE_CPP
#define DRONE_STATE_CPP

#include "drone_state.h"

void DroneState::print()
{
    DroneState::print_vec3f("angle", &this->angle);
    DroneState::print_vec3f("accel", &this->accel);
    DroneState::print_vec3f("coords", &this->coords);
    DroneState::print_vec3f("magneto", &this->magneto);
    DroneState::print_float("height", &this->height);
    DroneState::print_float("distance_ground", &this->distance_ground);
}

void DroneState::read_buffer(char *buffer)
{
    uint start = 0;
    start = read_vec3f(buffer, start, &this->angle);
    start = read_vec3f(buffer, start, &this->accel);
    start = read_vec3f(buffer, start, &this->coords);
    start = read_vec3f(buffer, start, &this->magneto);
    start = read_float(buffer, start, &this->height);
    start = read_float(buffer, start, &this->distance_ground);
}

void DroneState::print_float(const char *message, float *value)
{
    std::cout << message << ": " << *value << "\n";
}

void DroneState::print_vec3f(const char *message, glm::vec3 *value)
{
    std::cout << message << ": (";
    std::cout << value->x << ",";
    std::cout << value->y << ",";
    std::cout << value->z << ")\n";
}

uint DroneState::read_float(char *buffer, uint start, float *destiny)
{
    GenericFloatData value;

    value.b[0] = buffer[start++];
    value.b[1] = buffer[start++];
    value.b[2] = buffer[start++];
    value.b[3] = buffer[start++];

    *destiny = value.f;

    return start;
}

uint DroneState::read_vec3f(char *buffer, uint start, glm::vec3 *destiny)
{
    GenericFloatData ax, ay, az;

    start = read_float(buffer, start, &ax.f);
    start = read_float(buffer, start, &ay.f);
    start = read_float(buffer, start, &az.f);

    (*destiny).x = ax.f;
    (*destiny).y = ay.f;
    (*destiny).z = az.f;

    return start;
}

DroneState *DroneState::instance;

#endif