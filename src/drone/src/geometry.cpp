#include "geometry.h"
#include <iostream>

/**
 * 
 *  VEC2
 * 
 **/

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2::Vec2(glm::vec2 &vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

Vec2::Vec2(float x, float y, bool unitary)
{
    this->x = x;
    this->y = y;

    if (unitary)
    {
        this->set_unitary();
    }
}

Vec2::Vec2(glm::vec2 &vec, bool unitary)
{
    this->x = vec.x;
    this->y = vec.y;

    if (unitary)
    {
        this->set_unitary();
    }
}

Vec2 *Vec2::copy()
{
    Vec2 *v = new Vec2(this->x, this->y);
    v->unitary = this->unitary;
    return v;
}

void Vec2::set_unitary()
{
    float mod = this->x * this->x + this->y * this->y;
    mod = sqrt(mod);

    this->x = x / mod;
    this->y = y / mod;
    this->unitary = true;
}

void Vec2::log()
{
    std::cout << "Vec(" << this->x << ", " << this->y << ")\n";
    std::cout << "\tangle:" << this->get_angle() << "\n";
    std::cout << "\tunitary:" << (this->unitary ? "true" : "false") << "\n";
}

void Vec2::rotate(float angle)
{
    float nx, ny;

    angle = angle * M_PI / 180.0;

    nx = this->x * cos(angle) - this->y * sin(angle);
    ny = this->x * sin(angle) + this->y * cos(angle);

    this->x = nx;
    this->y = ny;
}

float Vec2::get_angle()
{
    float angle = atan2(this->y, this->x);

    angle = isnan(angle) ? 0 : angle;
    return 180 * (angle / M_PI);
}

/**
 * 
 *  VEC3
 * 
 **/

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::Vec3(float x, float y, float z, bool unitary)
{
    this->x = x;
    this->y = y;
    this->z = z;

    if (unitary)
    {
        this->set_unitary();
    }
}

Vec3::Vec3(glm::vec3 &vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

Vec3::Vec3(glm::vec3 &vec, bool unitary)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;

    if (unitary)
    {
        this->set_unitary();
    }
}

Vec3 *Vec3::copy()
{
    Vec3 *v = new Vec3(this->x, this->y, this->z);
    v->unitary = this->unitary;
    return v;
}

void Vec3::set_unitary()
{
    float mod = this->x * this->x + this->y * this->y + this->z * this->z;
    mod = sqrt(mod);

    this->x = x / mod;
    this->y = y / mod;
    this->z = z / mod;
    this->unitary = true;
}

void Vec3::log()
{
    std::cout << "Vec(" << this->x << ", " << this->y << ", " << this->z << ")\n";
    std::cout << "\talpha:" << this->get_alpha_angle() << "\n";
    std::cout << "\tbeta:" << this->get_beta_angle() << "\n";
    std::cout << "\tunitary:" << (this->unitary ? "true" : "false") << "\n";
}

Vec2 *Vec3::get_XY_projection()
{
    return new Vec2(this->x, this->y);
}

Vec2 *Vec3::get_XZ_projection()
{
    return new Vec2(this->x, this->z);
}

Vec2 *Vec3::get_YZ_projection()
{
    return new Vec2(this->y, this->z);
}

void Vec3::rotate_X(float angle)
{
    float nx, ny, nz;
    angle = angle * M_PI / 180;

    ny = this->y * cos(angle) - this->z * sin(angle);
    nz = this->y * sin(angle) + this->z * cos(angle);

    this->y = ny;
    this->z = nz;
}

void Vec3::rotate_Y(float angle)
{
    float nx, ny, nz;
    angle = angle * M_PI / 180;

    nx = this->x * cos(angle) + this->z * sin(angle);
    nz = this->x * sin(angle) + this->z * cos(angle);

    this->x = nx;
    this->z = nz;
}

void Vec3::rotate_Z(float angle)
{
    float nx, ny, nz;
    angle = angle * M_PI / 180;

    nx = this->x * cos(angle) - this->y * sin(angle);
    ny = this->x * sin(angle) + this->y * cos(angle);

    this->x = nx;
    this->y = ny;
}

float Vec3::get_alpha_angle()
{
    Vec2 *v1 = this->get_XY_projection();
    v1->set_unitary();
    return v1->get_angle();
}

float Vec3::get_beta_angle()
{
    float angle = 180 * asin(this->z) / M_PI;
    return angle + 90;
}

float Vec3::calc_alpha_offset(Vec3 *v)
{
    float alpha2 = v->get_alpha_angle();
    float alpha1 = this->get_alpha_angle();

    return alpha2 - alpha1;
}

float Vec3::calc_beta_offset(Vec3 *v)
{
    float beta2 = v->get_beta_angle();
    float beta1 = this->get_beta_angle();

    return beta2 - beta1;
}

void Vec3::add_angles(float alpha, float beta)
{
    this->add_beta_angle(beta);
    this->add_alpha_angle(alpha);
}

void Vec3::add_alpha_angle(float angle)
{
    this->rotate_Z(angle);
}

void Vec3::add_beta_angle(float angle)
{
    Vec3 *v = this->copy();
    float alpha_angle = this->get_alpha_angle();

    if (!isnan(alpha_angle))
    {
        v->add_alpha_angle(-alpha_angle);
    }

    Vec2 *projection_XZ = v->get_XZ_projection();
    projection_XZ->set_unitary();
    projection_XZ->rotate(angle);
    v->x = projection_XZ->x;
    v->y = 0.0;
    v->z = projection_XZ->y;

    if (!isnan(alpha_angle))
    {
        v->add_alpha_angle(alpha_angle);
    }

    this->x = v->x;
    this->y = v->y;
    this->z = v->z;
}

/****************************************************************************************/

// Rx(a) * <x, y, z> = <
//                       x
//                       y * cos(a) - z * sin(a)
//                       y * sin(a) + z * cos(a)
//                      >

// Rz(a) * <x, y, z> = <
//                       x * cos(a) - y * sin(a)
//                       x * sin(a) + y * cos(a)
//                       z
//                      >

// Ry(a) * <x, y, z> = <
//                       x * cos(a) + z * sin(a)
//                       y
//                       - x * sin(a) + z * cos(a)
//                      >

/*


function getInclination(v_mag, v_accel) {
    var alpha = v_mag.getAlphaAngle();
    console.log(alpha);
    var accel_copy = v_accel.copy().addAlphaAngle((360 - alpha) % 360);
    // var accel_copy = v_accel.copy().addAlphaAngle( -alpha );
    console.log(accel_copy.getAlphaAngle());
    var accel_projected = accel_copy.getXZProjection().setUnitary();
    console.log(accel_projected.getAngle());
    var angle = accel_projected.getAngle() + 90;
    console.log(angle);
    angle = (360 + angle) % 360;
    // angle = angle % 360;
    console.log(angle);
    if (angle < 180) {
        angle = -angle;
    } else {
        angle = 360 - angle;
    }
    console.log(angle);

    return angle;
}

function setInclination(v_mag, v_accel) {
    var angle = getInclination(v_mag, v_accel);
    v_mag.addBetaAngle(angle);
}

*/