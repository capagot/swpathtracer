#ifndef LUA_BIND_LAMBERTIAN_H
#define LUA_BIND_LAMBERTIAN_H

#include <glm/vec3.hpp>

#include "bsdf.h"

namespace lb {

struct Lambertian : public BSDF {
    Lambertian(const glm::vec3& kd, SamplerType sampler_type) : BSDF(Type::LAMBERTIAN, sampler_type), kd_(kd) {}

    glm::vec3 kd_;
};

}  // namespace lb

#endif  // LUA_BIND_LAMBERTIAN_H
