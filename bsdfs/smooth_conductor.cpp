#include "smooth_conductor.h"

SmoothConductor::SmoothConductor(std::unique_ptr<Fresnel> fresnel, std::unique_ptr<BSDFSampler> bsdf_sampler)
    : BSDF(BSDF::Type::OPAQUE), bsdf_sampler_(std::move(bsdf_sampler)), fresnel_(std::move(fresnel)) {}

glm::vec3 SmoothConductor::f(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;

    // The smooth conductor BRDF assumes that it will always be sampled along the reflection direction.
    if (local_wi.y > 0.0f)
        return fresnel_->getReflectance(local_wi.y) / local_wi.y;
    else
        return glm::vec3(0.0f);
}

glm::vec3 SmoothConductor::getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    (void)local_wo;

    return fresnel_->getReflectance(local_wi.y);
}

BSDFSample SmoothConductor::getSample(const glm::vec3& local_wo) const {
    return bsdf_sampler_->getSample(local_wo);
}
