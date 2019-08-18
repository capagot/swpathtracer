#ifndef LUA_BIND_LB_MATERIAL_H
#define LUA_BIND_LB_MATERIAL_H

namespace lb {

struct Material {
    Material(unsigned int layered_bsdf_id, unsigned int emission_id)
        : layered_bsdf_id_(layered_bsdf_id), emission_id_(emission_id) {}

    unsigned int layered_bsdf_id_;
    unsigned int emission_id_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_MATERIAL_H
