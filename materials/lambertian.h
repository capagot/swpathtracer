#ifndef MATERIALS_LAMBERTIAN_H
#define MATERIALS_LAMBERTIAN_H

#include "bsdf.h"
#include "bsdf_sampler.h"
#include "glm/vec3.hpp"

class Lambertian : public BSDF {
   public:
    Lambertian(const glm::vec3& reflectance, std::unique_ptr<BSDFSampler> bsdf_sampler);
    glm::vec3 f(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    glm::vec3 getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    BSDFSample getSample(const glm::vec3& local_wo) const override;

   private:
    glm::vec3 reflectance_;
    glm::vec3 brdf_;
    std::unique_ptr<BSDFSampler> bsdf_sampler_;
};

#endif  // MATERIALS_LAMBERTIAN_H
