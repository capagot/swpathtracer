#ifndef BSDF_SAMPLERS_REFRACTION_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_REFRACTION_BSDF_SAMPLER_H

#include <math.h>
#include <glm/glm.hpp>

#include "bsdf_sampler.h"
#include "prng.h"

class RefractionBSDFSampler : public BSDFSampler {
   public:
    RefractionBSDFSampler(float ior_i, float ior_t);
    BSDFSample getSample(const glm::vec3 &local_wo) const override;

   private:
    static constexpr float sample_probability_ = 1.0f;
    float ior_i_;
    float ior_t_;
};

#endif  // BSDF_SAMPLERS_REFRACTION_BSDF_SAMPLER_H
