#ifndef DRONE_STATE_CPP
#define DRONE_STATE_CPP

#include <math.h>
#include "drone_state.h"

void DroneState::accel_mag_adjust(Vec3 *mag, Vec3 *accel)
{
    float alpha, angle;
    Vec3 *accel_copy;
    Vec2 *accel_projected;

    alpha = mag->get_alpha_angle();
    accel_copy = accel->copy();
    accel_copy->add_alpha_angle(-alpha);
    accel_projected = accel_copy->get_XZ_projection();
    accel_projected->set_unitary();

    accel_projected->log();

    angle = accel_projected->get_angle() + 90;
    // if (angle < 180)
    // {
    //     angle = -angle;
    // }
    // else
    // {
    //     angle = 360 - angle;
    // }

    mag->add_beta_angle(angle);
}

void DroneState::normalize_accel()
{
    Vec3 *accel = new Vec3(DroneState::instance->accel, true);
    accel->set_unitary();
    DroneState::instance->accel_norm.x = accel->x;
    DroneState::instance->accel_norm.y = accel->y;
    DroneState::instance->accel_norm.z = accel->z;
}

void DroneState::normalize_magneto()
{
    DroneState::compensate_magneto_from_accel();
    DroneState::update_north();
    DroneState::update_mag_down();
}

void DroneState::compensate_magneto_from_accel()
{
    Vec3 *mag = new Vec3(DroneState::instance->magneto, true);
    Vec3 *accel = new Vec3(DroneState::instance->accel_norm, true);

    DroneState::accel_mag_adjust(mag, accel);
    mag->set_unitary();
    DroneState::instance->magneto.x = mag->x;
    DroneState::instance->magneto.y = mag->y;
    DroneState::instance->magneto.z = mag->z;
}

void DroneState::update_north()
{
    Vec3 *mag = new Vec3(DroneState::instance->magneto, true);
    if (!DroneState::instance->has_north)
    {
        DroneState::instance->has_north = true;

        Vec3 *n_mag = mag->copy();
        n_mag->z = 0;
        n_mag->set_unitary();

        glm::vec2 mag_offset_north;
        mag_offset_north.x = mag->calc_alpha_offset(n_mag);
        mag_offset_north.y = mag->calc_beta_offset(n_mag);

        DroneState::instance->magneto_offset_north.x = mag_offset_north.x;
        DroneState::instance->magneto_offset_north.y = mag_offset_north.y;
    }

    mag->add_angles(DroneState::instance->magneto_offset_north.x, DroneState::instance->magneto_offset_north.y);

    DroneState::instance->magneto_north.x = mag->x;
    DroneState::instance->magneto_north.y = mag->y;
    DroneState::instance->magneto_north.z = mag->z;
}

void DroneState::update_mag_down()
{
    Vec3 *mag = new Vec3(DroneState::instance->magneto, true);
    if (!DroneState::instance->has_mag_down)
    {
        DroneState::instance->has_mag_down = true;
        Vec3 *n_mag_down = new Vec3(DroneState::instance->accel_norm);
        n_mag_down->set_unitary();

        glm::vec2 mag_offset_down;
        mag_offset_down.x = mag->calc_alpha_offset(n_mag_down);
        mag_offset_down.y = mag->calc_beta_offset(n_mag_down);

        DroneState::instance->magneto_offset_down.x = mag_offset_down.x;
        DroneState::instance->magneto_offset_down.y = mag_offset_down.y;
    }

    mag->add_angles(DroneState::instance->magneto_offset_down.x, DroneState::instance->magneto_offset_down.y);

    DroneState::instance->magneto_down.x = mag->x;
    DroneState::instance->magneto_down.y = mag->y;
    DroneState::instance->magneto_down.z = mag->z;
}

void DroneState::read_mock()
{
    Vec3 *v_mag = new Vec3(DroneState::instance->mock_magneto, true);
    Vec3 *v_accel = new Vec3(DroneState::instance->mock_accel, true);

    v_mag->add_angles(DroneState::instance->mock_mag_alpha, DroneState::instance->mock_mag_beta);
    v_accel->add_angles(DroneState::instance->mock_accel_alpha, DroneState::instance->mock_accel_beta);

    DroneState::instance->magneto.x = v_mag->x;
    DroneState::instance->magneto.y = v_mag->y;
    DroneState::instance->magneto.z = v_mag->z;

    DroneState::instance->accel.x = v_accel->x;
    DroneState::instance->accel.y = v_accel->y;
    DroneState::instance->accel.z = v_accel->z;
}

void DroneState::print()
{
    DroneState::print_vec3f("angle", &DroneState::instance->angle);
    DroneState::print_vec3f("accel", &DroneState::instance->accel);
    DroneState::print_vec3f("coords", &DroneState::instance->coords);
    DroneState::print_vec3f("magneto", &DroneState::instance->magneto);
    DroneState::print_float("height", &DroneState::instance->height);
    DroneState::print_float("distance_ground", &DroneState::instance->distance_ground);
}

void DroneState::read_buffer(char *buffer)
{
    if (!DroneState::instance->use_mock)
    {
        uint start = 0;
        start = read_vec3f(buffer, start, &this->angle);
        start = read_vec3f(buffer, start, &this->accel);
        start = read_vec3f(buffer, start, &this->coords);
        start = read_vec3f(buffer, start, &this->magneto);
        start = read_float(buffer, start, &this->height);
        start = read_float(buffer, start, &this->distance_ground);
    }
    else
    {
        DroneState::read_mock();
    }

    normalize_accel();
    normalize_magneto();
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