#ifndef SMOOTH_TRIANGLE_H
#define SMOOTH_TRIANGLE_H

#include "triangle.h"

class SmoothTriangle : public Triangle {
   public:
    SmoothTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &n0,
                   const glm::vec3 &n1, const glm::vec3 &n2, long unsigned int material_id);

    glm::vec3 getNormal(float u, float v) const override {
        return glm::normalize(n0_ * (1.0f - u - v) + n1_ * u + n2_ * v);
    }

   private:
    glm::vec3 n0_;
    glm::vec3 n1_;
    glm::vec3 n2_;
};

#endif  // SMOOTH_TRIANGLE_H
