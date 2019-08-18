#ifndef MATERIALS_SMOOTH_CONDUCTOR_H
#define MATERIALS_SMOOTH_CONDUCTOR_H

#include <glm/glm.hpp>

#include "bsdf.h"
#include "bsdf_sampler.h"
#include "fresnel.h"

class SmoothConductor : public BSDF {
   public:
    SmoothConductor(std::unique_ptr<Fresnel> fresnel, std::unique_ptr<BSDFSampler> bsdf_sampler);
    glm::vec3 f(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    glm::vec3 getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    BSDFSample getSample(const glm::vec3& local_wo) const override;

   private:
    std::unique_ptr<BSDFSampler> bsdf_sampler_;
    std::unique_ptr<Fresnel> fresnel_;
};

#endif  // MATERIALS_SMOOTH_CONDUCTOR_H
