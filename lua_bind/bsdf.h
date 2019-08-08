#ifndef LUA_BIND_BSDF_H
#define LUA_BIND_BSDF_H

namespace lb {
struct BSDF {
    enum class Type { NONE, LAMBERTIAN, SMOOTH_CONDUCTOR, SMOOTH_DIELECTRIC, COOK_TORRANCE, LAYERED };
    enum class BSDFSamplerType { NONE, UNIFORM, IMPORTANCE };

    BSDF(Type type, BSDFSamplerType bsdf_sampler_type) : type_(type), bsdf_sampler_type_(bsdf_sampler_type) {}
    virtual ~BSDF() = 0;

    Type type_;
    BSDFSamplerType bsdf_sampler_type_;
    float roughness_;
};
}  // namespace lb

#endif  // LUA_BIND_BSDF_H
