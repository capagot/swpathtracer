#ifndef LUA_BIND_LB_SPHERE_H
#define LUA_BIND_LB_SPHERE_H

#include <glm/vec3.hpp>

#include "lb_object.h"

namespace lb {

struct Sphere : public Object {
    Sphere(const glm::vec3& center, float radius, long unsigned int material_id)
        : Object(material_id, Type::SPHERE), center_(center), radius_(radius) {}

    glm::vec3 center_;
    float radius_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_SPHERE_H
