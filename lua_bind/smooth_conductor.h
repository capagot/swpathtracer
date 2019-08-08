#ifndef LUA_BIND_SMOOTH_CONDUCTOR_H
#define LUA_BIND_SMOOTH_CONDUCTOR_H

#include <glm/vec3.hpp>

#include "bsdf.h"

namespace lb {
struct SmoothConductor : public BSDF {
    SmoothConductor(const glm::vec3& reflectance_at_normal_incidence, BSDFSamplerType bsdf_sampler_type)
        : BSDF(Type::SMOOTH_CONDUCTOR, bsdf_sampler_type),
          reflectance_at_normal_incidence_(reflectance_at_normal_incidence) {}

    glm::vec3 reflectance_at_normal_incidence_;
};
}  // namespace lb

#endif  // LUA_BIND_SMOOTH_CONDUCTOR_H
