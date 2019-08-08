#ifndef BSDF_SAMPLERS_UNIFORM_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_UNIFORM_BSDF_SAMPLER_H

#include <math.h>

#include "bsdf_sampler.h"
#include "prng.h"

class UniformBSDFSampler : public BSDFSampler {
   public:
    UniformBSDFSampler(PRNG<std::uniform_real_distribution, float, std::mt19937> &prng);
    BSDFSample getSample(const glm::vec3 &local_wo) const override;

   private:
    PRNG<std::uniform_real_distribution, float, std::mt19937> &prng_;
    static constexpr float sample_probability_ = 1.0f / (2.0f * PI_f);
};

#endif  // BSDF_SAMPLERS_UNIFORM_BSDF_SAMPLER_H
