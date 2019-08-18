#ifndef LUA_BIND_LB_TRIANGLE_H
#define LUA_BIND_LB_TRIANGLE_H

#include <glm/vec3.hpp>

#include "lb_object.h"

namespace lb {

struct Triangle : public Object {
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, long unsigned int material_id,
             Type type = Type::TRIANGLE)
        : Object(material_id, type), v0_(v0), v1_(v1), v2_(v2) {}

    glm::vec3 v0_;
    glm::vec3 v1_;
    glm::vec3 v2_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_TRIANGLE_H
