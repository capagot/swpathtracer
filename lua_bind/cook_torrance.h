#ifndef LUA_BIND_COOK_TORRANCE_H
#define LUA_BIND_COOK_TORRANCE_H

#include <glm/vec3.hpp>

#include "bsdf.h"

namespace lb {
struct CookTorrance : public BSDF {
    CookTorrance(float roughness, const glm::vec3& reflectance_at_normal_incidence, BSDFSamplerType bsdf_sampler_type)
        : BSDF(Type::COOK_TORRANCE, bsdf_sampler_type),
          roughness_(roughness),
          reflectance_at_normal_incidence_(reflectance_at_normal_incidence) {}

    float roughness_;
    glm::vec3 reflectance_at_normal_incidence_;
};
}  // namespace lb

#endif  // LUA_BIND_COOK_TORRANCE_H
