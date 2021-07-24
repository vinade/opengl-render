#include <math.h>
#include <glm/glm.hpp>

class Vec2
{
public:
    float x;
    float y;
    bool unitary = false;

    Vec2(float x, float y);
    Vec2(float x, float y, bool unitary);
    Vec2(glm::vec2 &vec);
    Vec2(glm::vec2 &vec, bool unitary);
    Vec2 *copy();
    void set_unitary();
    void log();

    void rotate(float angle);
    float get_angle();
};

class Vec3
{
public:
    float x;
    float y;
    float z;
    bool unitary = false;

    Vec3(float x, float y, float z);
    Vec3(float x, float y, float z, bool unitary);
    Vec3(glm::vec3 &vec);
    Vec3(glm::vec3 &vec, bool unitary);
    Vec3 *copy();
    void set_unitary();
    void log();

    Vec2 *get_XY_projection();
    Vec2 *get_XZ_projection();
    Vec2 *get_YZ_projection();

    void rotate_X(float angle);
    void rotate_Y(float angle);
    void rotate_Z(float angle);

    float get_alpha_angle();
    float get_beta_angle();

    float calc_alpha_offset(Vec3 *v);
    float calc_beta_offset(Vec3 *v);

    void add_alpha_angle(float angle);
    void add_beta_angle(float angle);
    void add_angles(float alpha, float beta);
};

class Geo
{
    glm::vec2 double_arc(float angle_sin, float angle_cos);
    glm::vec2 rotate_arc(glm::vec2 point, float angle);
    float angle(float angle_sin, float angle_cos);
};

// glm::vec3 rotate_x(glm::vec3 v, float angle)
// {
//     return
//     {
//         v.x,
//             v.y *cos(angle) - v.z *sin(angle),
//             v.y *cos(angle) + v.z *sin(angle)
//     }
// }

//
// v1 e v2
// a1 = angle(v1)
// v3 = rotate_arc(v2, -a1)
// v3 = double_arc(v3);
// v3 = rotate_arc(v4, a1);

// v5 = v4 * Rx(a1)
// v5 = double_arc(v3) * Rx(a1)
// v5 = double_arc(v2 * Rx(-a1)) * Rx(a1)

// v5 = Rx(a1) * double_arc(Rx(-a1) * v2)

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
