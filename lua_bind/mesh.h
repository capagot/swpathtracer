#ifndef LUA_BIND_MESH_H
#define LUA_BIND_MESH_H

#include "object.h"

namespace lb {
struct Mesh : public Object {
    Mesh(const std::string& filename, long unsigned int material_id, Type type = Type::MESH)
        : Object(material_id, type), filename_(filename) {}

    std::string filename_;
};
}  // namespace lb

#endif  // LUA_BIND_MESH_H
