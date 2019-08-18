#ifndef LUA_BIND_LB_SMOOTH_CONDUCTOR_H
#define LUA_BIND_LB_SMOOTH_CONDUCTOR_H

#include <glm/vec3.hpp>

#include "lb_bsdf.h"

namespace lb {

struct SmoothConductor : public BSDF {
    SmoothConductor(const glm::vec3& reflectance_at_normal_incidence, SamplerType sampler_type)
        : BSDF(Type::SMOOTH_CONDUCTOR, sampler_type),
          reflectance_at_normal_incidence_(reflectance_at_normal_incidence) {}

    glm::vec3 reflectance_at_normal_incidence_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_SMOOTH_CONDUCTOR_H
