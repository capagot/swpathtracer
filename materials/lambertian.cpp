#include "lambertian.h"

Lambertian::Lambertian(const glm::vec3& reflectance, std::unique_ptr<BSDFSampler> bsdf_sampler)
    : BSDF(BSDF::Type::OPAQUE),
      reflectance_(reflectance),
      brdf_(reflectance / PI_f),
      bsdf_sampler_(std::move(bsdf_sampler)) {}

glm::vec3 Lambertian::f(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;

    if (local_wi.y > 0.0f)
        return brdf_;
    else
        return glm::vec3(0.0f);
}

glm::vec3 Lambertian::getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;
    (void)local_wi;
    return reflectance_;
}

BSDFSample Lambertian::getSample(const glm::vec3& local_wo) const {
    return bsdf_sampler_->getSample(local_wo);
}
