#ifndef LUA_BIND_EMISSION_H
#define LUA_BIND_EMISSION_H

#include <glm/vec3.hpp>

namespace lb {
struct Emission {
    Emission(const glm::vec3& emission) : emission_(emission) {}

    glm::vec3 emission_;
};
}  // namespace lb

#endif  // LUA_BIND_EMISSION_H
