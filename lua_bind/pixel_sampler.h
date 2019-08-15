#ifndef LUA_BIND_PIXEL_SAMPLER_H
#define LUA_BIND_PIXEL_SAMPLER_H

#include <glm/vec3.hpp>

namespace lb {

struct PixelSampler {
    enum class Type { NONE, UNIFORM, REGULAR, JITTERED };

    PixelSampler(Type type, std::size_t spp) : type_(type), spp_(spp) {}

    Type type_;
    std::size_t spp_;
};

}  // namespace lb

#endif  // LUA_BIND_PIXEL_SAMPLER_H
