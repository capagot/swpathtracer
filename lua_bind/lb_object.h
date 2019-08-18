#ifndef LUA_BIND_LB_OBJECT_H
#define LUA_BIND_LB_OBJECT_H

namespace lb {

struct Object {
    enum class Type { NONE, TRIANGLE, SMOOTH_TRIANGLE, SPHERE, MESH, SMOOTH_MESH };

    Object(long unsigned int material_id, Type type) : material_id_(material_id), type_(type) {}
    virtual ~Object() = 0;

    long unsigned int material_id_;
    Type type_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_OBJECT_H
