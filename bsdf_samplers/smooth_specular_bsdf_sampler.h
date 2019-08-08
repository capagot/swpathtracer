#ifndef BSDF_SAMPLERS_SMOOTH_SPECULAR_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_SMOOTH_SPECULAR_BSDF_SAMPLER_H

#include <math.h>
#include <glm/glm.hpp>

#include "bsdf_sampler.h"
#include "prng.h"

class SmoothSpecularBSDFSampler : public BSDFSampler {
   public:
    SmoothSpecularBSDFSampler();
    BSDFSample getSample(const glm::vec3 &local_wo) const override;

   private:
    static constexpr float sample_probability_ = 1.0f;
};

#endif  // BSDF_SAMPLERS_SMOOTH_SPECULAR_BSDF_SAMPLER_H
