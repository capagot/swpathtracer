#include "smooth_dielectric.h"

SmoothDielectric::SmoothDielectric(float ior_i, float ior_t, std::unique_ptr<Fresnel> fresnel,
                                   std::unique_ptr<BSDFSampler> reflection_bsdf_sampler,
                                   std::unique_ptr<BSDFSampler> refraction_bsdf_sampler,
                                   PRNG<std::uniform_real_distribution, float, std::mt19937>& prng)
    : BSDF(BSDF::Type::TRANSMISSIVE),
      ior_i_(ior_i),
      ior_t_(ior_t),
      reflection_bsdf_sampler_(std::move(reflection_bsdf_sampler)),
      refraction_bsdf_sampler_(std::move(refraction_bsdf_sampler)),
      fresnel_(std::move(fresnel)),
      prng_(prng) {}

glm::vec3 SmoothDielectric::f(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;

    if (local_wi.y == 0.0) return glm::vec3(0.0f);

    return glm::vec3(1.0f) / local_wi.y;
}

glm::vec3 SmoothDielectric::getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;
    (void)local_wi;

    return glm::vec3(1.0f);
}

BSDFSample SmoothDielectric::getSample(const glm::vec3& local_wo) const {
    float ior_i = ior_i_;
    float ior_t = ior_t_;

    if (local_wo.y < 0.0f) {
        ior_i = ior_t_;
        ior_t = ior_i_;
    }

    float a = ((ior_i * ior_i) / (ior_t * ior_t)) * (1.0f - local_wo.y * local_wo.y);

    if (a > 1.0f)  // Total internal reflection (TIR)
        return reflection_bsdf_sampler_->getSample(local_wo);

    glm::vec3 fresnel = fresnel_->getReflectance(local_wo.y);
    float max_reflectance = std::max(std::max(fresnel[0], fresnel[1]), fresnel[2]);

    if (prng_() < max_reflectance)
        return reflection_bsdf_sampler_->getSample(local_wo);
    else
        return refraction_bsdf_sampler_->getSample(local_wo);
}
