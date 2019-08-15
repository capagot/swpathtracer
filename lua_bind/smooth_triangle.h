#ifndef LUA_BIND_SMOOTH_TRIANGLE_H
#define LUA_BIND_SMOOTH_TRIANGLE_H

#include <glm/vec3.hpp>

#include "triangle.h"

namespace lb {

struct SmoothTriangle : public Triangle {
    SmoothTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& n0,
                   const glm::vec3& n1, const glm::vec3& n2, long unsigned int material_id)
        : Triangle(v0, v1, v2, material_id, Type::SMOOTH_TRIANGLE), n0_(n0), n1_(n1), n2_(n2) {}

    glm::vec3 n0_;
    glm::vec3 n1_;
    glm::vec3 n2_;
};

}  // namespace lb

#endif  // LUA_BIND_SMOOTH_TRIANGLE_H
