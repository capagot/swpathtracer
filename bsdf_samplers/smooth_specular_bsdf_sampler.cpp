#include "smooth_specular_bsdf_sampler.h"

SmoothSpecularBSDFSampler::SmoothSpecularBSDFSampler() {}

BSDFSample SmoothSpecularBSDFSampler::getSample(const glm::vec3 &local_wo) const {
    return BSDFSample(glm::vec3(-local_wo.x, local_wo.y, -local_wo.z), sample_probability_);
}
