#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

#include "layered_bsdf.h"

class Material {
   public:
    Material(const LayeredBSDF& layered_bsdf, const glm::vec3& emission);
    void evalBSDF(const glm::vec3& local_wo, glm::vec3& bsdf, glm::vec3& reflectance, float& pdf,
                  glm::vec3& local_wi) const;
    inline const glm::vec3& getEmission() const {
        return emission_;
    }

   private:
    const LayeredBSDF& layered_bsdf_;
    const glm::vec3& emission_;
};

#endif  // MATERIAL_H
