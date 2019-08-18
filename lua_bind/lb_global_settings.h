#ifndef LUA_BIND_LB_GLOBAL_SETTINGS_H
#define LUA_BIND_LB_GLOBAL_SETTINGS_H

namespace lb {

struct GlobalSettings {
    enum class AccelStructureType { NONE, BVH_SAH, SBVH_SAH };

    GlobalSettings(const glm::vec3& background_color, AccelStructureType accel_structure, float overlap_threshold)
        : background_color_(background_color),
          accel_structure_(accel_structure),
          overlap_threshold_(overlap_threshold) {}

    glm::vec3 background_color_;
    AccelStructureType accel_structure_;
    float overlap_threshold_;
};

}  // namespace lb

#endif  // LUA_BIND_LB_GLOBAL_SETTINGS_H
