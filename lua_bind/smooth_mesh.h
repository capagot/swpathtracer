#ifndef LUA_BIND_SMOOTH_MESH_H
#define LUA_BIND_SMOOTH_MESH_H

#include "mesh.h"

namespace lb {
struct SmoothMesh : public Mesh {
    SmoothMesh(const std::string& filename, long unsigned int material_id)
        : Mesh(filename, material_id, Type::SMOOTH_MESH) {}
};
}  // namespace lb

#endif  // LUA_BIND_SMOOTH_MESH_H
