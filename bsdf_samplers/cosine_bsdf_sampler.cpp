#include "cosine_bsdf_sampler.h"

CosineBSDFSampler::CosineBSDFSampler(PRNG<std::uniform_real_distribution, float, std::mt19937>& prng) : prng_(prng) {}

BSDFSample CosineBSDFSampler::getSample(const glm::vec3& local_wo) const {
    (void)local_wo;

    float r1 = prng_();
    float r2 = prng_();
    float phi = 2.0f * PI_f * r2;
    float cos_theta = sqrtf(r1);
    float sqrtf_sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
    glm::vec3 local_wi = glm::vec3(cos(phi) * sqrtf_sin_theta, cos_theta, sin(phi) * sqrtf_sin_theta);
    float sample_probability = cos_theta / PI_f;

    return BSDFSample(local_wi, sample_probability);
}
