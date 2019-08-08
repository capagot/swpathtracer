#ifndef GLOBAL_SETTINGS_H
#define GLOBAL_SETTINGS_H

#include <glm/vec3.hpp>

class GlobalSettings {
   public:
    enum class AccelerationStructureType { NONE, BVH_SAH, SBVH_SAH };

    GlobalSettings(const glm::vec3& background_color, AccelerationStructureType acceleration_structure)
        : background_color_(background_color), acceleration_structure_(acceleration_structure) {}
    AccelerationStructureType getAccelerationStructureType() const { return acceleration_structure_; };

   private:
    glm::vec3 background_color_;
    AccelerationStructureType acceleration_structure_;
};

#endif  // GLOBAL_SETTINGS_H
