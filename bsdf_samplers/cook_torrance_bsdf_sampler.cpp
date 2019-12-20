#include "cook_torrance_bsdf_sampler.h"

CookTorranceBSDFSampler::CookTorranceBSDFSampler(float roughness,
                                                 PRNG<std::uniform_real_distribution, float, std::mt19937>& prng)
    : roughness_(roughness), prng_(prng) {}

BSDFSample CookTorranceBSDFSampler::getSample(const glm::vec3& local_wo) const {
    float r1 = prng_();
    float r2 = prng_();
    float phi = 2.0f * PI_f * r2;
    float theta = atan(sqrtf(-(roughness_ * roughness_) * log(1.0f - r1)));

    // 'm' is the normal vector of the microfacet
    glm::vec3 m = glm::vec3(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta));

    glm::vec3 local_wi =
        2.0f * m * glm::dot(m, local_wo) - local_wo;  // reflect local_wo about 'm' (the microfacet normal))

    return BSDFSample(local_wi, getProbability(local_wo, local_wi));
}

float CookTorranceBSDFSampler::getProbability(const glm::vec3& local_wo, const glm::vec3& local_wi) const {
    if (local_wi.y <= 0.0f) return 1.0f;  // dummy

    glm::vec3 h = local_wo + local_wi; // h is the normal vector of the microfacet

    h = glm::normalize(h);

    float nh = std::max(0.0f, h.y); // nh is the dot between h and the normal of the average surface

    if (nh == 0.0f) return 1e-5f;  // dummy value

    float nh_2 = nh * nh;
    float roughness_2 = roughness_ * roughness_;
    float d1 = 1.0f / (PI_f * roughness_2 * nh * nh * nh * nh);
    float d2 = (nh_2 - 1.0f) / (roughness_2 * nh_2);
    float om = glm::dot(local_wi, h);

    if (om <= 0.0f) return 1e+5f;  // dummy value

    return (d1 * exp(d2) * nh) / (4.0f * om);
}
