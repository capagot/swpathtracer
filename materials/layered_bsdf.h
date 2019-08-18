#ifndef MATERIALS_LAYERED_BSDF_H
#define MATERIALS_LAYERED_BSDF_H

#include <vector>

#include "bsdf.h"

class LayeredBSDF {
   public:
    LayeredBSDF(){};
    void addBSDF(const BSDF* bsdf);
    void evalBSDF(const glm::vec3& local_wo, glm::vec3& bsdf, glm::vec3& reflectance, float& pdf,
                  glm::vec3& local_wi) const;

   private:
    std::vector<const BSDF*> bsdfs_;
};

#endif  // MATERIALS_LAYERED_BSDF_H
