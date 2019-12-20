#ifndef LUA_BIND_LB_MESH_H
#define LUA_BIND_LB_MESH_H

#include "lb_object.h"

namespace lb {

struct SubMesh {
    SubMesh(const std::string& name, long unsigned int material_id) : name(name), material_id(material_id) {}
    std::string name;
    long unsigned int material_id;
};

struct Mesh : public Object {
    enum class RenderSubMeshes { NONE, ALL };

    Mesh(const std::string& filename, long unsigned int material_id,
         RenderSubMeshes render_submeshes = RenderSubMeshes::ALL, Type type = Type::MESH)
        : Object(material_id, type), filename_(filename), render_submeshes(render_submeshes) {}

    std::string filename_;
    RenderSubMeshes render_submeshes;
    std::vector<SubMesh> submeshes;
};

}  // namespace lb

#endif  // LUA_BIND_LB_MESH_H
