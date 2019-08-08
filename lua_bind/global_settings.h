#ifndef LUA_BIND_GLOBAL_SETTINGS_H
#define LUA_BIND_GLOBAL_SETTINGS_H

namespace lb {
struct GlobalSettings {
    enum class AccelerationStructureType { NONE, BVH_SAH, SBVH_SAH };

    GlobalSettings(const glm::vec3& background_color, AccelerationStructureType acceleration_structure,
                   float overlap_threshold)
        : background_color_(background_color),
          acceleration_structure_(acceleration_structure),
          overlap_threshold_(overlap_threshold) {}

    glm::vec3 background_color_;
    AccelerationStructureType acceleration_structure_;
    float overlap_threshold_;
};
}  // namespace lb

#endif  // LUA_BIND_GLOBAL_SETTINGS_H
