#include "refraction_bsdf_sampler.h"

RefractionBSDFSampler::RefractionBSDFSampler(float ior_i, float ior_t) : ior_i_(ior_i), ior_t_(ior_t) {}

BSDFSample RefractionBSDFSampler::getSample(const glm::vec3 &local_wo) const {
    glm::vec3 n(0.0f, 1.0f, 0.0f);
    float ior_i = ior_i_;
    float ior_t = ior_t_;
    float signal = 1.0f;

    if (local_wo.y < 0.0f) {
        ior_i = ior_t_;
        ior_t = ior_i_;
        signal = -1.0f;
    }

    // Snell's law
    float a = 1.0f - ((ior_i * ior_i) / (ior_t * ior_t)) * (1.0f - local_wo.y * local_wo.y);

    // TODO: is this normalization necessary?? I don't think so... verify.
    glm::vec3 local_wi = glm::normalize((ior_i / ior_t) * (n * local_wo.y - local_wo) - signal * n * sqrtf(a));

    return BSDFSample(glm::vec3(local_wi), sample_probability_);
}
