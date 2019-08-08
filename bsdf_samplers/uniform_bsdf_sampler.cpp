#include "uniform_bsdf_sampler.h"

UniformBSDFSampler::UniformBSDFSampler(PRNG<std::uniform_real_distribution, float, std::mt19937>& prng) : prng_(prng) {}

BSDFSample UniformBSDFSampler::getSample(const glm::vec3& local_wo) const {
    (void)local_wo;

    float r1 = prng_();
    float r2 = prng_();
    float phi = 2.0f * PI_f * r2;
    float sqrtf_sin_theta = sqrtf(1.0f - r1 * r1);

    return BSDFSample(glm::vec3(cos(phi) * sqrtf_sin_theta, r1, sin(phi) * sqrtf_sin_theta), sample_probability_);
}
