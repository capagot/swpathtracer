#include "smooth_triangle.h"

SmoothTriangle::SmoothTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &n0,
                               const glm::vec3 &n1, const glm::vec3 &n2, long unsigned int material_id)
    : Triangle(v0, v1, v2, material_id), n0_(n0), n1_(n1), n2_(n2) {}
