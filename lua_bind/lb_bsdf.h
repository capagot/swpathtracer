#ifndef LUA_BIND_LB_BSDF_H
#define LUA_BIND_LB_BSDF_H

namespace lb {

struct BSDF {
    enum class Type { NONE, LAMBERTIAN, SMOOTH_CONDUCTOR, SMOOTH_DIELECTRIC, COOK_TORRANCE };
    enum class SamplerType { NONE, UNIFORM, IMPORTANCE };

    BSDF(Type type, SamplerType sampler_type) : type_(type), sampler_type_(sampler_type) {}
    virtual ~BSDF() = 0;

    Type type_;
    SamplerType sampler_type_;
    float roughness_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_BSDF_H
