#ifndef LUA_BIND_LAYERED_BSDF_H
#define LUA_BIND_LAYERED_BSDF_H

#include <vector>

namespace lb {
struct LayeredBSDF {
    LayeredBSDF() {}

    std::vector<unsigned int> bsdf_id_list_;
};
}  // namespace lb

#endif  // LUA_BIND_LAYERED_BSDF_H
