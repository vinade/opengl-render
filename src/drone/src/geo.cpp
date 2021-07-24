#include <math.h>
#include <glm/glm.hpp>

inline glm::vec3 normalize_unitary(glm::vec3 value)
{
    float x = value.x;
    float y = value.y;
    float z = value.z;
    float mod = x * x + y * y + z * z;
    mod = sqrt(mod);

    return glm::vec3(
        x / mod,
        y / mod,
        z / mod);
}

inline glm::vec3 rotateX(float angle, glm::vec3 source)
{
    return glm::vec3(
        source.x,
        source.y * cos(angle) - source.z * sin(angle),
        source.y * sin(angle) + source.z * cos(angle));
}

inline glm::vec3 rotateY(float angle, glm::vec3 source)
{
    return glm::vec3(
        source.x * cos(angle) + source.z * sin(angle),
        source.y,
        -source.x * sin(angle) + source.z * cos(angle));
}

inline glm::vec3 rotateZ(float angle, glm::vec3 source)
{
    return glm::vec3(
        source.x * cos(angle) - source.y * sin(angle),
        source.x * sin(angle) + source.y * cos(angle),
        source.z);
}

glm::vec3 rotateLat(float lat, glm::vec3 source)
{
    return rotateX(lat, source);
}

glm::vec3 rotateLng(float lng, glm::vec3 source)
{
    return rotateZ(lng, source);
}

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
