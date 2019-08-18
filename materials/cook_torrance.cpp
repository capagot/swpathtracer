#include "cook_torrance.h"

CookTorrance::CookTorrance(float roughness, std::unique_ptr<Fresnel> fresnel, std::unique_ptr<BSDFSampler> bsdf_sampler)
    : BSDF(BSDF::Type::OPAQUE),
      bsdf_sampler_(std::move(bsdf_sampler)),
      roughness_(roughness),
      fresnel_(std::move(fresnel)) {}

glm::vec3 CookTorrance::f(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    if (local_wi.y <= 0.0f) return glm::vec3(0.0f);

    glm::vec3 h = local_wo + local_wi;

    h = glm::normalize(h);

    float nv = std::max(0.0f, local_wo.y);
    float nl = std::max(0.0f, local_wi.y);
    float vh = std::max(0.0f, glm::dot(local_wo, h));
    float nh = std::max(0.0f, h.y);

    if ((nv == 0.0f) || (nl == 0.0f) || (vh == 0.0f) || (nh == 0.0f) || (roughness_ == 0.0f)) {
        return glm::vec3(0.0f);
    }

    float lh = std::max(0.0f, glm::dot(local_wi, h));

    // Beckmann normal distribution function
    float nh2 = nh * nh;
    float roughness2 = roughness_ * roughness_;
    float d1 = 1.0f / (PI_f * roughness2 * nh2 * nh2);
    float d2 = (nh2 - 1.0f) / (roughness2 * nh2);
    float d = d1 * exp(d2);

    // Cook-Torrance geometric attenuation
    float g1 = 2.0f * nh / vh;
    float g = std::min(1.0f, std::min(g1 * nv, g1 * nl));

    // Fresnel term
    glm::vec3 f = fresnel_->getReflectance(lh);

    glm::vec3 rough_specular_term = (f * d * g) / (4.0f * nv * nl);

    return rough_specular_term;
}

glm::vec3 CookTorrance::getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    glm::vec3 h = local_wo + local_wi;
    h = glm::normalize(h);
    float lh = std::max(0.0f, glm::dot(local_wi, h));
    return fresnel_->getReflectance(lh);
}

BSDFSample CookTorrance::getSample(const glm::vec3& local_wo) const {
    return bsdf_sampler_->getSample(local_wo);
}
