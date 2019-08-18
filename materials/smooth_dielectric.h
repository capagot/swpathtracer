#ifndef MATERIALS_SMOOTH_DIELECTRIC_H
#define MATERIALS_SMOOTH_DIELECTRIC_H

#include <glm/glm.hpp>

#include "bsdf.h"
#include "bsdf_sampler.h"
#include "fresnel.h"
#include "prng.h"

class SmoothDielectric : public BSDF {
   public:
    SmoothDielectric(float ior_i, float ior_t, std::unique_ptr<Fresnel> fresnel,
                     std::unique_ptr<BSDFSampler> reflection_bsdf_sampler,
                     std::unique_ptr<BSDFSampler> refraction_bsdf_sampler,
                     PRNG<std::uniform_real_distribution, float, std::mt19937>& prng);
    glm::vec3 f(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    glm::vec3 getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    BSDFSample getSample(const glm::vec3& local_wo) const override;

   private:
    float ior_i_;
    float ior_t_;
    std::unique_ptr<BSDFSampler> reflection_bsdf_sampler_;
    std::unique_ptr<BSDFSampler> refraction_bsdf_sampler_;
    std::unique_ptr<Fresnel> fresnel_;
    PRNG<std::uniform_real_distribution, float, std::mt19937>& prng_;
};

#endif  // MATERIALS_SMOOTH_DIELECTRIC_H
