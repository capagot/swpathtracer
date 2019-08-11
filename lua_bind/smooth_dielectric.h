#ifndef LUA_BIND_SMOOTH_DIELECTRIC_H
#define LUA_BIND_SMOOTH_DIELECTRIC_H

#include "bsdf.h"

namespace lb {

struct SmoothDielectric : public BSDF {
    SmoothDielectric(float ior, SamplerType sampler_type) : BSDF(Type::SMOOTH_DIELECTRIC, sampler_type), ior_(ior) {}

    float ior_;
};

}  // namespace lb

#endif  // LUA_BIND_SMOOTH_DIELECTRIC_H
